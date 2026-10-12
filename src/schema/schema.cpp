/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
 * All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 * gives you permission to link the code of this program
 * (as well as its derivative works) to "Counter-Strike 2," "Source 2,"
 * "Steam," and any Game MODs or server software running on software by
 * Valve Corporation. You must obey the GNU General Public License in all
 * respects for all other code used.
 *
 * Additionally, this exception applies to all derivative works unless
 * otherwise stated in LICENSE.txt.
 *
 * Authors:
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

#include "source2toolkit/schema/schema.h"

#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitModule.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/utils/virtual.h"
TOOLKIT_GLOBALVARS();

#include "platform.h"
#include "edict.h"
#include "engine2/networkgameserverbase.h"
#include "engine2/inetworkserverservice.h"
#include "networksystem/inetworkmessages.h"
#include "schemasystem/schemasystem.h"
#include "entity2/entityidentity.h"
#include "entity2/entityinstance.h"
#include "tier0/memdbgon.h"
#include "tier0/utlmap.h"
#include <map>
#include <mutex>
#include <cstring>
#include <unordered_map>
#include <string>

#ifdef _WIN32
#define MODULE_PREFIX ""
#define MODULE_EXT ".dll"
#else
#define MODULE_PREFIX "lib"
#define MODULE_EXT ".so"
#endif

IGameEventManager2* GetGameEventManager()
{
    return g_ToolkitAPI->GetGameEventManager();
}

CGlobalVars* GetGlobalVars()
{
    return g_ToolkitAPI->GetGlobalVars();
}

ICvar* GetCVar()
{
    return g_ToolkitAPI->GetCVar();
}

ISource2Server* GetSource2Server()
{
    return g_ToolkitAPI->GetSource2Server();
}

IVEngineServer2* GetEngineServer()
{
    return g_ToolkitAPI->GetEngineServer();
}

IGameEventSystem* GetGameEventSystem()
{
    return g_ToolkitAPI->GetGameEventSystem();
}
INetworkMessages* GetNetworkMessages()
{
    return g_ToolkitAPI->GetNetworkMessages();
}

INetworkServerService* GetNetworkServerService()
{
    return g_ToolkitAPI->GetNetworkServerService();
}

CGameEntitySystem* GetEntitySystem()
{
    return g_ToolkitAPI->GetEntitySystem();
}

CSchemaSystem* GetSchemaSystem()
{
    return g_ToolkitAPI->GetSchemaSystem();
}

namespace
{
    constexpr uint32_t g_ChainKey = hash_32_fnv1a_const("__m_pChainEntity");

    struct SchemaClassEntry
    {
        std::map<uint32_t, SchemaKey> fields;

        /// Null when the class is not in the server's schema; the entry then stays
        /// empty, so a lookup warns once instead of re-searching every time.
        SchemaClassInfoData_t* pClassInfo = nullptr;

        /// False while the networked flags were computed without an entity system
        /// (so all of them read false). The entry is rebuilt once one exists.
        bool networkResolved = false;
    };

    std::map<uint32_t, SchemaClassEntry> g_SchemaClasses;
    std::mutex g_SchemaMutex;
}

static void SchemaWarn(const char* pszFormat, const char* pszArg1, const char* pszArg2 = "")
{
    char szMessage[512];
    V_snprintf(szMessage, sizeof(szMessage), pszFormat, pszArg1, pszArg2);
#ifdef SOURCE2TOOLKIT_CORE
    FP_WARN("{}", szMessage);
#else
    Warning("%s\n", szMessage);
#endif
}

static SchemaClassInfoData_t* FindServerClass(const char* className)
{
    CSchemaSystem* pSchemaSystem = GetSchemaSystem();
    if (!pSchemaSystem)
        return nullptr;

    CSchemaSystemTypeScope* pType = pSchemaSystem->FindTypeScopeForModule(MODULE_PREFIX "server" MODULE_EXT);
    return pType ? pType->FindDeclaredClass(className).Get() : nullptr;
}

// The serializer database knows which fields the engine actually replicates.
// Any entity class reaches it (some schema classes have no entity of their own).
// Null until the entity system exists.
static CNetworkSerializerCodeGenDatabase* GetSerializerDatabase()
{
    CGameEntitySystem* pEntitySystem = GetEntitySystem();
    if (!pEntitySystem)
        return nullptr;

    CEntityClass* pClass = pEntitySystem->FindClassByName("CBaseEntity");
    if (!pClass || !pClass->m_NetworkSerializerInfo)
        return nullptr;

    return pClass->m_NetworkSerializerInfo->m_pDatabase;
}

static bool IsFieldNetworked(CNetworkSerializerCodeGenDatabase* pDatabase, const char* className, const char* fieldName)
{
    if (!pDatabase)
        return false;

    int index = pDatabase->m_ClassInfos.Find(className);
    if (index == pDatabase->m_ClassInfos.InvalidIndex())
        return false;

    return pDatabase->m_ClassInfos[index]->FindField(fieldName) != nullptr;
}

// __m_pChainEntity is often declared on a base class
// (e.g. CCSGameRules -> CTeamplayRules -> CMultiplayRules -> CGameRules, in this case it's in CGameRules)
static SchemaClassFieldData_t* FindChainField(SchemaClassInfoData_t* pClassInfo)
{
    for (; pClassInfo; pClassInfo = pClassInfo->m_nBaseClassCount ? pClassInfo->m_pBaseClasses[0].m_pClass : nullptr)
    {
        for (int i = 0; i < pClassInfo->m_nFieldCount; ++i)
        {
            if (V_strcmp(pClassInfo->m_pFields[i].m_pszName, "__m_pChainEntity") == 0)
                return &pClassInfo->m_pFields[i];
        }
    }

    return nullptr;
}

static void BuildClassEntry(SchemaClassEntry& entry)
{
    CNetworkSerializerCodeGenDatabase* pDatabase = GetSerializerDatabase();
    SchemaClassInfoData_t* pClassInfo = entry.pClassInfo;

    entry.fields.clear();

    // The class' own fields, then its base classes' (single inheritance offsets stay
    // valid for the derived class), so a derived type can look up inherited fields.
    for (SchemaClassInfoData_t* pDeclaring = pClassInfo; pDeclaring;
         pDeclaring = pDeclaring->m_nBaseClassCount ? pDeclaring->m_pBaseClasses[0].m_pClass : nullptr)
    for (int i = 0; i < pDeclaring->m_nFieldCount; ++i)
    {
        SchemaClassFieldData_t& field = pDeclaring->m_pFields[i];

        SchemaKey key;
        key.offset = field.m_nSingleInheritanceOffset;
        // Asked on the class that declares the field.
        key.networked = IsFieldNetworked(pDatabase, pDeclaring->m_pszName, field.m_pszName);

        // Atomic collections publish their own element accessor; without it a
        // CUtlVectorEmbeddedNetworkVar cannot be indexed correctly.
        CSchemaType* pType = field.m_pType;
        if (pType && pType->m_eTypeCategory == SCHEMA_TYPE_ATOMIC
            && pType->m_eAtomicCategory == SCHEMA_ATOMIC_COLLECTION_OF_T)
        {
            key.manipulator = static_cast<CSchemaType_Atomic_CollectionOfT*>(pType)->m_pfnManipulator;
        }

        // Field names are unique within a class, so a clash there is a hash collision.
        // A base class field is only skipped when the derived class already has the key.
        if (!entry.fields.emplace(hash_32_fnv1a_const(field.m_pszName), key).second && pDeclaring == pClassInfo)
            SchemaWarn("schema: hash collision on '%s' in '%s', the field resolves to another one!", field.m_pszName, pClassInfo->m_pszName);
    }

    // Does not overwrite the class' own __m_pChainEntity, when it has one.
    if (SchemaClassFieldData_t* pChain = FindChainField(pClassInfo))
        entry.fields.emplace(g_ChainKey, SchemaKey{pChain->m_nSingleInheritanceOffset, false});

    entry.networkResolved = pDatabase != nullptr;
}

int16_t schema::FindChainOffset(const char* className, uint32_t classNameHash)
{
    return schema::GetOffset(className, classNameHash, "__m_pChainEntity", g_ChainKey).offset;
}

int16_t schema::FindChainOffset(const char* className)
{
    return FindChainOffset(className, hash_32_fnv1a_const(className));
}

SchemaKey schema::GetOffset(const char* className, uint32_t classKey, const char* memberName, uint32_t memberKey)
{
    std::lock_guard<std::mutex> lock(g_SchemaMutex);

    auto it = g_SchemaClasses.find(classKey);

    if (it == g_SchemaClasses.end())
    {
        // Too early to tell whether the class exists; do not cache anything.
        if (!GetSchemaSystem())
            return {};

        it = g_SchemaClasses.emplace(classKey, SchemaClassEntry()).first;
        it->second.pClassInfo = FindServerClass(className);

        if (it->second.pClassInfo)
            BuildClassEntry(it->second);
        else
            SchemaWarn("schema::GetOffset(): class '%s' was not found!", className);
    }
    else if (it->second.pClassInfo && !it->second.networkResolved && GetSerializerDatabase())
    {
        BuildClassEntry(it->second);
    }

    const auto field = it->second.fields.find(memberKey);

    if (field == it->second.fields.end())
    {
        // A class without a chain entity is normal; a missing class was reported above.
        if (memberKey != g_ChainKey && it->second.pClassInfo)
            SchemaWarn("schema::GetOffset(): '%s' was not found in '%s' -- the field does nothing until the plugin is rebuilt against a current schema", memberName, className);

        // Not offset 0: a write there would land on the object's vtable. The
        // accessors route a key marked invalid to MissingFieldStorage().
        SchemaKey missing{};
        missing.valid = false;
        return missing;
    }

    return field->second;
}

void* schema::MissingFieldStorage(size_t size)
{
    // Big enough for any schema field a plugin declares; zeroed so a read
    // gives the type's "nothing", and a write lands here and nowhere else.
    alignas(64) static unsigned char s_storage[4096] = {};
    (void)size;
    return s_storage;
}

int32_t schema::GetServerOffset(const char* pszClassName, const char* pszPropName)
{
    // The class, then up its base classes.
    for (SchemaClassInfoData_t* pClassInfo = FindServerClass(pszClassName); pClassInfo;
         pClassInfo = pClassInfo->m_nBaseClassCount ? pClassInfo->m_pBaseClasses[0].m_pClass : nullptr)
    {
        for (int i = 0; i < pClassInfo->m_nFieldCount; i++)
        {
            if (V_strcmp(pClassInfo->m_pFields[i].m_pszName, pszPropName) == 0)
                return pClassInfo->m_pFields[i].m_nSingleInheritanceOffset;
        }
    }

    return -1;
}

int32_t schema::GetClassSize(const char* className)
{
    SchemaClassInfoData_t* pClassInfo = FindServerClass(className);
    return pClassInfo ? pClassInfo->m_nSize : -1;
}

void schema::SetStateChanged(CEntityInstance* entity, const char* className, const char* propName)
{
    if (!entity || !className || !propName)
        return;

    const uint32_t classHash = hash_32_fnv1a_const(className);

    SchemaKey key = GetOffset(className, classHash, propName, hash_32_fnv1a_const(propName));

    if (!key.networked || key.offset == 0)
        return;

    const int16_t chainOffset = FindChainOffset(className, classHash);

    auto pEntity = reinterpret_cast<uintptr_t>(entity);

    if (chainOffset != 0)
        ChainNetworkStateChanged(pEntity + chainOffset, key.offset);
    else
        EntityNetworkStateChanged(pEntity, key.offset);
}

// Whether p points into the .text section of the module pModuleAddress lives in.
// The range is read once; the module is temporary, as it holds a copy of the
// whole section read from disk.
static bool IsInModuleText(const void* p, const void* pModuleAddress)
{
    static const IToolkitModule::SectionInfo s_text = [pModuleAddress] {
        IToolkitModule* pModule = IToolkitModule::New(const_cast<void*>(pModuleAddress));
        if (!pModule)
            return IToolkitModule::SectionInfo{0, 0};

        const IToolkitModule::SectionInfo text = pModule->GetSectionByName(".text");
        g_ToolkitAPI->FreeModule(pModule);
        return text;
    }();

    const auto address = reinterpret_cast<uintptr_t>(p);
    return address >= s_text.base && address < s_text.base + s_text.size;
}

// The NetworkVar_<field> wrapper the engine puts around an embedded object
// forwards NetworkStateChanged to the owner. That slot is the one whose first
// 32 bytes check the data's path index, `cmp dword ptr [reg+0x38], -1`
// (NetworkStateChangedData::m_nPathIndex): 83 /7 with a disp8 of 0x38 and an
// imm8 of -1, whatever register holds the data. Same test as s2binlib's
// find_networkvar_vtable_statechanged, no signature needed.
static bool ChecksPathIndex(const unsigned char* pCode)
{
    for (int i = 0; i + 4 <= 32; ++i)
    {
        const unsigned char modrm = pCode[i + 1];
        // mod 01 (disp8), reg 111 (/7 = cmp), rm not 100 (that one takes a SIB byte)
        if (pCode[i] == 0x83 && (modrm & 0xF8) == 0x78 && modrm != 0x7C && pCode[i + 2] == 0x38 && pCode[i + 3] == 0xFF)
            return true;
    }

    return false;
}

static int FindNetworkStateChangedIndex(void** pVtable)
{
    // The vtable ends where its slots stop pointing into code (Linux: the next
    // vtable's offset-to-top, Windows: its RTTI locator).
    for (int i = 0; i < 256 && IsInModuleText(pVtable[i], pVtable); ++i)
    {
        if (ChecksPathIndex(static_cast<const unsigned char*>(pVtable[i])))
            return i;
    }

    return -1;
}

// The slot is found at runtime from the object's own vtable, so no index has to
// be kept per class (nNetworkStateChangedOffset only marks the class as inline).
// A plain object that is not embedded in a NetworkVar has no such slot: no-op.
void NetworkVarStateChanged(uintptr_t pNetworkVar, uint32_t nOffset, uint32 /*nNetworkStateChangedOffset*/)
{
    static std::unordered_map<void**, int> s_indices;
    static std::mutex s_mutex;

    void** pVtable = *reinterpret_cast<void***>(pNetworkVar);

    int index;
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        auto it = s_indices.find(pVtable);
        if (it == s_indices.end())
            it = s_indices.emplace(pVtable, FindNetworkStateChangedIndex(pVtable)).first;
        index = it->second;
    }

    if (index < 0)
        return;

    NetworkStateChangedData data(nOffset);
    CALL_VIRTUAL(void, index, (void*)pNetworkVar, &data);
}

void EntityNetworkStateChanged(uintptr_t pEntity, uint nOffset)
{
    NetworkStateChangedData data(nOffset);
    reinterpret_cast<CEntityInstance*>(pEntity)->NetworkStateChanged(data);
}

// s2sdk's chainer passes it on to its entity with its own path index, and
// drops it while the entity is still being constructed.
void ChainNetworkStateChanged(uintptr_t pNetworkVarChainer, uint nLocalOffset)
{
    NetworkStateChangedData data(nLocalOffset);
    reinterpret_cast<CNetworkVarChainer*>(pNetworkVarChainer)->NetworkStateChanged(data);
}
