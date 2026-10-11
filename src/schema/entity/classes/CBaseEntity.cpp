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

#include "source2toolkit/schema/entity/classes/CBaseEntity.h"

#include "source2toolkit/IToolkitAddresses.h"
#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitEntities.h"
#include "source2toolkit/IToolkitGameConfig.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entity/classes/CBodyComponent.h"
#include "source2toolkit/schema/entity/classes/CCollisionProperty.h"
#include "source2toolkit/schema/entity/classes/CEntitySubclassVDataBase.h"
#include "source2toolkit/schema/entity/classes/CGameSceneNode.h"
#include "source2toolkit/utils/virtual.h"
TOOLKIT_GLOBALVARS();

CBaseEntity* CBaseEntity::CreateEntityByName(const char* pszClassName)
{
    return g_pToolkitEntities->CreateEntityByName(pszClassName);
}

// Called here rather than through IToolkitEntities, which has no way to say
// which chain to take; this is the same call that interface makes.
void CBaseEntity::AcceptInput(const char* pszInput, CEntityInstance* pActivator, CEntityInstance* pCaller, const char* pszValue, HookChain eChain)
{
    const auto fn = ResolveHookChain(ADDR_ACCEPT_INPUT(), eChain);
    fn(this, pszInput, pActivator, pCaller, variant_t(pszValue));
}

void CBaseEntity::AddEntityIOEvent(const char* pszInput, CEntityInstance* pActivator, CEntityInstance* pCaller, const char* pszValue, float flDelay, HookChain eChain)
{
    CGameEntitySystem* pEntitySystem = GetEntitySystem();
    if (!pEntitySystem)
        return;

    const auto fn = ResolveHookChain(ADDR_ADD_ENTITY_IO_EVENT(), eChain);
    fn(reinterpret_cast<CEntitySystem*>(pEntitySystem), this, pszInput, pActivator, pCaller, variant_t(pszValue), flDelay, nullptr, nullptr);
}

CEntityIOListenerHandle* CBaseEntity::AddSingleEntityIOListener(const char* pszOutput, std::function<Action(const char*, CEntityInstance*, CEntityInstance*, float, bool)> callback, bool post)
{
    auto* listener = new CSingleEntityIOListener(this, std::move(callback));

    const char* classname = GetClassname();

    // The listener and the callback it holds are in whichever plugin called
    // this, so that plugin has to be the one the toolkit drops it with.
    g_pToolkitEntities->AddEntityIOListener(listener, classname, pszOutput, post);

    auto* handle = new CEntityIOListenerHandle();
    handle->m_pListener = listener;
    handle->m_szClassname = classname ? classname : "*";
    handle->m_szOutput = pszOutput ? pszOutput : "*";
    handle->m_bPost = post;

    return handle;
}

Vector CBaseEntity::GetAbsOrigin()
{
    return m_CBodyComponent->m_pSceneNode->m_vecAbsOrigin();
}

QAngle CBaseEntity::GetAngRotation()
{
    return m_CBodyComponent->m_pSceneNode->m_angRotation();
}

QAngle CBaseEntity::GetAbsRotation()
{
    return m_CBodyComponent->m_pSceneNode->m_angAbsRotation();
}

Vector CBaseEntity::GetAbsVelocity()
{
    return m_vecAbsVelocity;
}

void CBaseEntity::SetAbsOrigin(Vector vecOrigin)
{
    m_CBodyComponent->m_pSceneNode->m_vecAbsOrigin = vecOrigin;
}

void CBaseEntity::SetAbsRotation(QAngle angAbsRotation)
{
    m_CBodyComponent->m_pSceneNode->m_angAbsRotation = angAbsRotation;
}

void CBaseEntity::SetAngRotation(QAngle angRotation)
{
    m_CBodyComponent->m_pSceneNode->m_angRotation = angRotation;
}

void CBaseEntity::SetAbsVelocity(Vector vecVelocity)
{
    m_vecAbsVelocity = vecVelocity;
}

void CBaseEntity::SetBaseVelocity(Vector vecVelocity)
{
    m_vecBaseVelocity = vecVelocity;
}

CEntitySubclassVDataBase* CBaseEntity::GetVData()
{
    return *reinterpret_cast<CEntitySubclassVDataBase**>(reinterpret_cast<uint8*>(m_nSubclassID()) + 4);
}

void CBaseEntity::DispatchSpawn(CEntityKeyValues* pEntityKeyValues, HookChain eChain)
{
    ResolveHookChain(ADDR_DISPATCH_SPAWN(), eChain)(this, pEntityKeyValues);
}

void CBaseEntity::Teleport(const Vector* pPosition, const QAngle* pAngles, const Vector* pVelocity, HookChain eChain)
{
    static int offset = g_pToolkitGameConfig->GetOffset("CBaseEntity::Teleport");
    CALL_VIRTUAL_CHAIN(void, offset, eChain, this, pPosition, pAngles, pVelocity);
}

void CBaseEntity::SetMoveType(MoveType_t nMoveType)
{
    m_MoveType = nMoveType;
    m_nActualMoveType = nMoveType;
}

uint8 CBaseEntity::GetCollisionGroup()
{
    if (!m_pCollision())
        return 0;

    return m_pCollision->m_collisionAttribute().m_nCollisionGroup();
}

void CBaseEntity::SetCollisionGroup(uint8 nCollisionGroup)
{
    if (!m_pCollision())
        return;

    m_pCollision->m_collisionAttribute().m_nCollisionGroup = nCollisionGroup;
    m_pCollision->m_CollisionGroup = nCollisionGroup;
    CollisionRulesChanged();
}

void CBaseEntity::CollisionRulesChanged()
{
    static int offset = g_pToolkitGameConfig->GetOffset("CBaseEntity::CollisionRulesChanged");
    CALL_VIRTUAL(void, offset, this);
}

int CBaseEntity::GetIndex()
{
    return GetEntityIndex().Get();
}

const char* CBaseEntity::GetName() const
{
    return m_pEntity->m_name.String();
}

void CBaseEntity::Remove(HookChain eChain)
{
    ResolveHookChain(ADDR_UTIL_REMOVE(), eChain)(this);
}

void CBaseEntity::DispatchParticleEffect(const char* pszParticleName, ParticleAttachment_t nAttachType, uint8 nAttachmentPoint,
                                         bool bResetAllParticlesOnEntity, IRecipientFilter* pFilter, HookChain eChain)
{
    const auto fn = ResolveHookChain(ADDR_DISPATCH_PARTICLE_EFFECT(), eChain);

    // No attachment name token and the default split-screen slot, as the
    // game's own entity-attached calls pass them.
    fn(pszParticleName, static_cast<int>(nAttachType), this, nAttachmentPoint, 0, bResetAllParticlesOnEntity, -1, pFilter, 0);
}
