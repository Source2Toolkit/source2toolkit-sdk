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

#ifndef _INCLUDE_CCSPLAYERPAWN_H
#define _INCLUDE_CCSPLAYERPAWN_H

#pragma once

#include "CBaseEntity.h"
#include "igameevents.h"
#include "ehandle.h"
#include "entityhandle.h"
#include "vector.h"
#include "utlbinaryblock.h"
#include "utlsymbol.h"
#include "utlsymbollarge.h"
#include "utlstring.h"
#include "utlstringtoken.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"
#include <cstdint>

#include "CCSPlayerPawnBase.h"
#include "CEconItemView.h"
#include "../enums/CSPlayerBlockingUseAction_t.h"
#include "EntitySpottedState_t.h"
#include "../enums/loadout_slot_t.h"

class CBaseEntity;
class CCSBot;
class CCSMinimapVolume;
class CCSPlayer_ActionTrackingServices;
class CCSPlayer_AimPunchServices;
class CCSPlayer_BulletServices;
class CCSPlayer_BuyServices;
class CCSPlayer_DamageReactServices;
class CCSPlayer_HostageServices;
class CCSPlayer_RadioServices;

class CCSPlayerPawn : public CCSPlayerPawnBase
{
public:
    DECLARE_SCHEMA_CLASS(CCSPlayerPawn);

    SCHEMA_FIELD(CCSPlayer_BulletServices*, m_pBulletServices);
    SCHEMA_FIELD(CCSPlayer_HostageServices*, m_pHostageServices);
    SCHEMA_FIELD(CCSPlayer_BuyServices*, m_pBuyServices);
    SCHEMA_FIELD(CCSPlayer_ActionTrackingServices*, m_pActionTrackingServices);
    SCHEMA_FIELD(CCSPlayer_AimPunchServices*, m_pAimPunchServices);
    SCHEMA_FIELD(CCSPlayer_RadioServices*, m_pRadioServices);
    SCHEMA_FIELD(CCSPlayer_DamageReactServices*, m_pDamageReactServices);
    SCHEMA_FIELD(uint16_t, m_nCharacterDefIndex);
    SCHEMA_FIELD(bool, m_bHasFemaleVoice);
    SCHEMA_FIELD(CUtlString, m_strVOPrefix);
    SCHEMA_FIELD_POINTER(char, m_szLastPlaceName);
    SCHEMA_FIELD(bool, m_bInHostageResetZone);
    SCHEMA_FIELD(bool, m_bInBuyZone);
    SCHEMA_FIELD(CUtlVector<CHandle<CBaseEntity>>, m_TouchingBuyZones);
    SCHEMA_FIELD(bool, m_bWasInBuyZone);
    SCHEMA_FIELD(bool, m_bInHostageRescueZone);
    SCHEMA_FIELD(bool, m_bInBombZone);
    SCHEMA_FIELD(bool, m_bWasInHostageRescueZone);
    SCHEMA_FIELD(int32_t, m_iRetakesOffering);
    SCHEMA_FIELD(int32_t, m_iRetakesOfferingCard);
    SCHEMA_FIELD(bool, m_bRetakesHasDefuseKit);
    SCHEMA_FIELD(bool, m_bRetakesMVPLastRound);
    SCHEMA_FIELD(int32_t, m_iRetakesMVPBoostItem);
    SCHEMA_FIELD(loadout_slot_t, m_RetakesMVPBoostExtraUtility);
    SCHEMA_FIELD(float, m_flHealthShotBoostExpirationTime);
    SCHEMA_FIELD(float, m_flLandingTimeSeconds);
    SCHEMA_FIELD(bool, m_bIsBuyMenuOpen);
    SCHEMA_FIELD(float, m_lastLandTime);
    SCHEMA_FIELD(bool, m_bOnGroundLastTick);
    SCHEMA_FIELD(int32_t, m_iPlayerLocked);
    SCHEMA_FIELD(float, m_flTimeOfLastInjury);
    SCHEMA_FIELD(float, m_flNextSprayDecalTime);
    SCHEMA_FIELD(bool, m_bNextSprayDecalTimeExpedited);
    SCHEMA_FIELD(int32_t, m_nRagdollDamageBone);
    SCHEMA_FIELD(Vector, m_vRagdollDamageForce);
    SCHEMA_FIELD_POINTER(char, m_szRagdollDamageWeaponName);
    SCHEMA_FIELD(bool, m_bRagdollDamageHeadshot);
    SCHEMA_FIELD(Vector, m_vRagdollServerOrigin);
    SCHEMA_FIELD(CEconItemView, m_EconGloves);
    SCHEMA_FIELD(uint8_t, m_nEconGlovesChanged);
    SCHEMA_FIELD(CUtlVector<CHandle<CCSMinimapVolume>>, m_vecCurrentMinimapVolumes);
    SCHEMA_FIELD(CHandle<CCSMinimapVolume>, m_hActiveMinimapVolume);
    SCHEMA_FIELD(QAngle, m_qDeathEyeAngles);
    SCHEMA_FIELD(bool, m_bLeftHanded);
    SCHEMA_FIELD(float, m_fSwitchedHandednessTime);
    SCHEMA_FIELD(float, m_flViewmodelOffsetX);
    SCHEMA_FIELD(float, m_flViewmodelOffsetY);
    SCHEMA_FIELD(float, m_flViewmodelOffsetZ);
    SCHEMA_FIELD(float, m_flViewmodelFOV);
    SCHEMA_FIELD(bool, m_bIsWalking);
    SCHEMA_FIELD(float, m_fLastGivenDefuserTime);
    SCHEMA_FIELD(float, m_fLastGivenBombTime);
    SCHEMA_FIELD(float, m_flDealtDamageToEnemyMostRecentTimestamp);
    SCHEMA_FIELD(uint32_t, m_iDisplayHistoryBits);
    SCHEMA_FIELD(float, m_flLastAttackedTeammate);
    SCHEMA_FIELD(float, m_allowAutoFollowTime);
    SCHEMA_FIELD(bool, m_bResetArmorNextSpawn);
    SCHEMA_FIELD(CEntityIndex, m_nLastKillerIndex);
    SCHEMA_FIELD(EntitySpottedState_t, m_entitySpottedState);
    SCHEMA_FIELD(int32_t, m_nSpotRules);
    SCHEMA_FIELD(bool, m_bIsScoped);
    SCHEMA_FIELD(bool, m_bResumeZoom);
    SCHEMA_FIELD(bool, m_bIsDefusing);
    SCHEMA_FIELD(bool, m_bIsGrabbingHostage);
    SCHEMA_FIELD(CSPlayerBlockingUseAction_t, m_iBlockingUseActionInProgress);
    SCHEMA_FIELD(float, m_flEmitSoundTime);
    SCHEMA_FIELD(bool, m_bInNoDefuseArea);
    SCHEMA_FIELD(CEntityIndex, m_iBombSiteIndex);
    SCHEMA_FIELD(int32_t, m_nWhichBombZone);
    SCHEMA_FIELD(bool, m_bInBombZoneTrigger);
    SCHEMA_FIELD(bool, m_bWasInBombZoneTrigger);
    SCHEMA_FIELD(int32_t, m_iShotsFired);
    SCHEMA_FIELD(float, m_flFlinchStack);
    SCHEMA_FIELD(float, m_flVelocityModifier);
    SCHEMA_FIELD(Vector, m_vecTotalBulletForce);
    SCHEMA_FIELD(bool, m_bWaitForNoAttack);
    SCHEMA_FIELD(float, m_ignoreLadderJumpTime);
    SCHEMA_FIELD(bool, m_bKilledByHeadshot);
    SCHEMA_FIELD(int32_t, m_LastHitBox);
    SCHEMA_FIELD(CCSBot*, m_pBot);
    SCHEMA_FIELD(bool, m_bBotAllowActive);
    SCHEMA_FIELD(int32_t, m_nLastPickupPriority);
    SCHEMA_FIELD(float, m_flLastPickupPriorityTime);
    SCHEMA_FIELD(int32_t, m_ArmorValue);
    SCHEMA_FIELD(uint16_t, m_unCurrentEquipmentValue);
    SCHEMA_FIELD(uint16_t, m_unRoundStartEquipmentValue);
    SCHEMA_FIELD(uint16_t, m_unFreezetimeEndEquipmentValue);
    SCHEMA_FIELD(int32_t, m_iLastWeaponFireUsercmd);
    SCHEMA_FIELD(bool, m_bIsSpawning);
    SCHEMA_FIELD(int32_t, m_iDeathFlags);
    SCHEMA_FIELD(bool, m_bHasDeathInfo);
    SCHEMA_FIELD(float, m_flDeathInfoTime);
    SCHEMA_FIELD(Vector, m_vecDeathInfoOrigin);
    SCHEMA_FIELD_POINTER(uint32_t, m_vecPlayerPatchEconIndices);
    SCHEMA_FIELD(Color, m_GunGameImmunityColor);
    SCHEMA_FIELD(float, m_grenadeParameterStashTime);
    SCHEMA_FIELD(bool, m_bGrenadeParametersStashed);
    SCHEMA_FIELD(QAngle, m_angStashedShootAngles);
    SCHEMA_FIELD(Vector, m_vecStashedGrenadeThrowPosition);
    SCHEMA_FIELD(Vector, m_vecStashedGrenadeThrowPawnCenter);
    SCHEMA_FIELD(Vector, m_vecStashedVelocity);
    SCHEMA_FIELD(bool, m_bCommittingSuicideOnTeamChange);
    SCHEMA_FIELD(bool, m_wasNotKilledNaturally);
    SCHEMA_FIELD(float, m_fImmuneToGunGameDamageTime);
    SCHEMA_FIELD(bool, m_bGunGameImmunity);
    SCHEMA_FIELD(float, m_flModifier0);
    SCHEMA_FIELD(float, m_fMolotovDamageTime);
    SCHEMA_FIELD(QAngle, m_angEyeAngles);

public:
    /// <summary>Get the angles the pawn is actually looking along.</summary>
    QAngle GetEyeAngles();
    /// <summary>Runs CCSPlayerPawn::PostThink.</summary>
    void PostThink(HookChain eChain = HookChain::Run);

public:
    static CCSPlayerPawn* New(const char* className)
    {
        return CBaseEntity::New<CCSPlayerPawn>(className);
    }

    static CCSPlayerPawn* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CCSPlayerPawn>(iIndex);
    }

    static CCSPlayerPawn* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CCSPlayerPawn> GetHandle()
    {
        return CBaseEntity::GetHandle<CCSPlayerPawn>();
    }
};

#endif // _INCLUDE_CCSPLAYERPAWN_H
