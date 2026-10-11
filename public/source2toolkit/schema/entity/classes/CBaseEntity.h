/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
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
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
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
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

#ifndef _INCLUDE_CBASEENTITY_H
#define _INCLUDE_CBASEENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/BloodType.h"
#include "../enums/EntityPlatformTypes_t.h"
#include "../enums/ParticleAttachment_t.h"
#include "CNetworkTransmitComponent.h"
#include "CNetworkVelocityVector.h"
#include "ResponseContext_t.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"
#include "thinkfunc_t.h"

#include <ehandle.h>
#include <entityhandle.h>
#include <igameevents.h>
#include <shareddefs.h>
#include <utlbinaryblock.h>
#include <utlstring.h>
#include <utlstringtoken.h>
#include <utlsymbol.h>
#include <utlsymbollarge.h>
#include <vector.h>

#include <cstdint>

class CBaseFilter;
class CBodyComponent;
class CCSPlayerController;
class CCollisionProperty;
class CEntityInstance;
class CEntityKeyValues;
class CEntitySubclassVDataBase;
class CPulseGraphInstance_ServerEntity;
class IRecipientFilter;

class CBaseEntity : public CEntityInstance
{
public:
    DECLARE_SCHEMA_CLASS(CBaseEntity);

    SCHEMA_FIELD(CBodyComponent*, m_CBodyComponent);
    SCHEMA_FIELD(CNetworkTransmitComponent, m_NetworkTransmitComponent);
    SCHEMA_FIELD(CUtlVector<thinkfunc_t>, m_aThinkFunctions);
    SCHEMA_FIELD(int32_t, m_iCurrentThinkContext);
    SCHEMA_FIELD(int32_t, m_nLastThinkTick);
    SCHEMA_FIELD(bool, m_bDisabledContextThinks);
    SCHEMA_FIELD(CTypedBitVec<64>, m_isSteadyState);
    SCHEMA_FIELD(float, m_lastNetworkChange);
    SCHEMA_FIELD(BASEPTR, m_think);
    SCHEMA_FIELD(CUtlVector<ResponseContext_t>, m_ResponseContexts);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszResponseContext);
    SCHEMA_FIELD(ENTITYFUNCPTR, m_pfnTouch);
    SCHEMA_FIELD(USEPTR, m_pfnUse);
    SCHEMA_FIELD(ENTITYFUNCPTR, m_pfnBlocked);
    SCHEMA_FIELD(BASEPTR, m_pfnMoveDone);
    SCHEMA_FIELD(int32_t, m_iHealth);
    SCHEMA_FIELD(int32_t, m_iMaxHealth);
    SCHEMA_FIELD(uint8_t, m_lifeState);
    SCHEMA_FIELD(float, m_flDamageAccumulator);
    SCHEMA_FIELD(bool, m_bTakesDamage);
    SCHEMA_FIELD(TakeDamageFlags_t, m_nTakeDamageFlags);
    SCHEMA_FIELD(EntityPlatformTypes_t, m_nPlatformType);
    SCHEMA_FIELD(MoveCollide_t, m_MoveCollide);
    SCHEMA_FIELD(MoveType_t, m_MoveType);
    SCHEMA_FIELD(MoveType_t, m_nPreviouslySetMoveType);
    SCHEMA_FIELD(MoveType_t, m_nActualMoveType);
    SCHEMA_FIELD(uint8_t, m_nWaterTouch);
    SCHEMA_FIELD(uint8_t, m_nSlimeTouch);
    SCHEMA_FIELD(bool, m_bRestoreInHierarchy);
    SCHEMA_FIELD(CUtlSymbolLarge, m_target);
    SCHEMA_FIELD(CHandle<CBaseFilter>, m_hDamageFilter);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszDamageFilterName);
    SCHEMA_FIELD(float, m_flMoveDoneTime);
    SCHEMA_FIELD_POINTER(CUtlStringToken, m_nSubclassID);
    SCHEMA_FIELD(float, m_flAnimTime);
    SCHEMA_FIELD(float, m_flSimulationTime);
    SCHEMA_FIELD(float, m_flCreateTime);
    SCHEMA_FIELD(bool, m_bClientSideRagdoll);
    SCHEMA_FIELD(uint8_t, m_ubInterpolationFrame);
    SCHEMA_FIELD(Vector, m_vPrevVPhysicsUpdatePos);
    SCHEMA_FIELD(uint8_t, m_iTeamNum);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iGlobalname);
    SCHEMA_FIELD(int32_t, m_iSentToClients);
    SCHEMA_FIELD(CUtlString, m_sUniqueHammerID);
    SCHEMA_FIELD(uint32_t, m_spawnflags);
    SCHEMA_FIELD(int32_t, m_nNextThinkTick);
    SCHEMA_FIELD(int32_t, m_nSimulationTick);
    SCHEMA_FIELD(CEntityIOOutput, m_OnKilled);
    SCHEMA_FIELD(uint32_t, m_fFlags);
    SCHEMA_FIELD(Vector, m_vecAbsVelocity);
    SCHEMA_FIELD(CNetworkVelocityVector, m_vecVelocity);
    SCHEMA_FIELD(Vector, m_vecBaseVelocity);
    SCHEMA_FIELD(int32_t, m_nPushEnumCount);
    SCHEMA_FIELD(CCollisionProperty*, m_pCollision);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hEffectEntity);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hOwnerEntity);
    SCHEMA_FIELD(uint32_t, m_fEffects);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hGroundEntity);
    SCHEMA_FIELD(int32_t, m_nGroundBodyIndex);
    SCHEMA_FIELD(float, m_flFriction);
    SCHEMA_FIELD(float, m_flElasticity);
    SCHEMA_FIELD(float, m_flGravityScale);
    SCHEMA_FIELD(float, m_flTimeScale);
    SCHEMA_FIELD(float, m_flWaterLevel);
    SCHEMA_FIELD(bool, m_bGravityDisabled);
    SCHEMA_FIELD(bool, m_bAnimatedEveryTick);
    SCHEMA_FIELD(float, m_flActualGravityScale);
    SCHEMA_FIELD(bool, m_bGravityActuallyDisabled);
    SCHEMA_FIELD(bool, m_bDisableLowViolence);
    SCHEMA_FIELD(uint8_t, m_nWaterType);
    SCHEMA_FIELD(int32_t, m_iEFlags);
    SCHEMA_FIELD(CEntityIOOutput, m_OnUser1);
    SCHEMA_FIELD(CEntityIOOutput, m_OnUser2);
    SCHEMA_FIELD(CEntityIOOutput, m_OnUser3);
    SCHEMA_FIELD(CEntityIOOutput, m_OnUser4);
    SCHEMA_FIELD(int32_t, m_iInitialTeamNum);
    SCHEMA_FIELD(float, m_flNavIgnoreUntilTime);
    SCHEMA_FIELD(QAngle, m_vecAngVelocity);
    SCHEMA_FIELD(bool, m_bNetworkQuantizeOriginAndAngles);
    SCHEMA_FIELD(bool, m_bLagCompensate);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_pBlocker);
    SCHEMA_FIELD(float, m_flLocalTime);
    SCHEMA_FIELD(float, m_flVPhysicsUpdateLocalTime);
    SCHEMA_FIELD(BloodType, m_nBloodType);
    SCHEMA_FIELD(CPulseGraphInstance_ServerEntity*, m_pPulseGraphInstance);

public:
    /// <summary>Creates entity by classname.</summary>
    static CBaseEntity* CreateEntityByName(const char* pszClassName);
    /// <summary>Create entity by classname.</summary>
    template<typename T>
    static T* New(const char* pszClassName)
    {    
        return reinterpret_cast<T*>(CreateEntityByName(pszClassName));
    }
    /// <summary>Get entity by index.</summary>
    template<typename T>
    static T* FromIndex(int iIndex)
    {    
        return reinterpret_cast<T*>(GetEntitySystem()->GetEntityInstance(CEntityIndex(iIndex)));
    }
    /// <summary>Get entity by entity index.</summary>
    template<typename T>
    static T* FromIndex(CEntityIndex index)
    {    
        return FromIndex<T>(index.Get());
    }
    /// <summary>Accepts entity input.</summary>
    void AcceptInput(const char* pszInput, CEntityInstance* pActivator = nullptr, CEntityInstance* pCaller = nullptr, const char* pszValue = "", HookChain eChain = HookChain::Run);
    /// <summary>Add delayed entity IO event.</summary>
    void AddEntityIOEvent(const char* pszInput, CEntityInstance* pActivator = nullptr, CEntityInstance* pCaller = nullptr, const char* pszValue = "", float flDelay = 0.0f, HookChain eChain = HookChain::Run);
    /// <summary>Add signle entity IO listener.</summary>
    CEntityIOListenerHandle* AddSingleEntityIOListener(const char* pszOutput, std::function<Action(const char*, CEntityInstance*, CEntityInstance*, float, bool)> callback, bool post);
    /// <summary>Get absolute origin.</summary>
    Vector GetAbsOrigin();
    /// <summary>Get local rotation.</summary>
    QAngle GetAngRotation();
    /// <summary>Get absolute rotation.</summary>
    QAngle GetAbsRotation();
    /// <summary>Get absolute velocity.</summary>
    Vector GetAbsVelocity();
    /// <summary>Set absolute origin.</summary>
    void SetAbsOrigin(Vector vecOrigin);
    /// <summary>Set absolute rotation.</summary>
    void SetAbsRotation(QAngle angAbsRotation);
    /// <summary>Set local rotation.</summary>
    void SetAngRotation(QAngle angRotation);
    /// <summary>Set absolute velocity.</summary>
    void SetAbsVelocity(Vector vecVelocity);
    /// <summary>Set base velocity.</summary>
    void SetBaseVelocity(Vector vecVelocity);
    /// <summary>Get entity VData.</summary>
    CEntitySubclassVDataBase* GetVData();
    /// <summary>Spawn entity.</summary>
    void DispatchSpawn(CEntityKeyValues* pEntityKeyValues = nullptr, HookChain eChain = HookChain::Run);
    /// <summary>Teleport entity.</summary>
    void Teleport(const Vector* pPosition, const QAngle* pAngles, const Vector* pVelocity, HookChain eChain = HookChain::Run);
    /// <summary>Set move type.</summary>
    void SetMoveType(MoveType_t nMoveType);
    /// <summary>Get collision group.</summary>
    uint8 GetCollisionGroup();
    /// <summary>Set collision group.</summary>
    void SetCollisionGroup(uint8 nCollisionGroup);
    /// <summary>Notify collision rules changed.</summary>
    void CollisionRulesChanged();
    /// <summary>Get entity index.</summary>
    int GetIndex();
    /// <summary>Get entity handle. Templated the same way New() is: every generated
    /// entity class shadows it with a plain GetHandle() returning CHandle of its own
    /// type, so player->GetHandle() on a CCSPlayerController* is a CHandle<CCSPlayerController>.</summary>
    template<typename T = CBaseEntity>
    CHandle<T> GetHandle()
    {    
        return CHandle<T>(GetRefEHandle());
    }
    /// <summary>Get entity name.</summary>
    const char* GetName() const;
    /// <summary>Removes the entity (UTIL_Remove).</summary>
    void Remove(HookChain eChain = HookChain::Run);
    /// <summary>Dispatches a particle effect on this entity. A null filter sends it to everyone.</summary>
    void DispatchParticleEffect(const char* pszParticleName, ParticleAttachment_t nAttachType = ParticleAttachment_t::PATTACH_ABSORIGIN_FOLLOW, uint8 nAttachmentPoint = 0, bool bResetAllParticlesOnEntity = false, IRecipientFilter* pFilter = nullptr, HookChain eChain = HookChain::Run);
};

#endif // _INCLUDE_CBASEENTITY_H
