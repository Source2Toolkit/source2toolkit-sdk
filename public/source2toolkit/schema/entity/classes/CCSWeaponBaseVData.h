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

#ifndef _INCLUDE_CCSWEAPONBASEVDATA_H
#define _INCLUDE_CCSWEAPONBASEVDATA_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/CSWeaponCategory.h"
#include "../enums/CSWeaponSilencerType.h"
#include "../enums/CSWeaponType.h"
#include "../enums/gear_slot_t.h"
#include "../enums/loadout_slot_t.h"
#include "CBasePlayerWeaponVData.h"
#include "CFiringModeFloat.h"
#include "CFiringModeInt.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"

#include <ehandle.h>
#include <entityhandle.h>
#include <igameevents.h>
#include <utlbinaryblock.h>
#include <utlstring.h>
#include <utlstringtoken.h>
#include <utlsymbol.h>
#include <utlsymbollarge.h>
#include <vector.h>

#include <cstdint>

class CCSWeaponBase;

class CCSWeaponBaseVData : public CBasePlayerWeaponVData
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CCSWeaponBaseVData);

    SCHEMA_FIELD(CSWeaponType, m_WeaponType);
    SCHEMA_FIELD(CSWeaponCategory, m_WeaponCategory);
    SCHEMA_FIELD(Vector, m_vecMuzzlePos0);
    SCHEMA_FIELD(Vector, m_vecMuzzlePos1);
    SCHEMA_FIELD(gear_slot_t, m_GearSlot);
    SCHEMA_FIELD(int32_t, m_GearSlotPosition);
    SCHEMA_FIELD(loadout_slot_t, m_DefaultLoadoutSlot);
    SCHEMA_FIELD(int32_t, m_nPrice);
    SCHEMA_FIELD(int32_t, m_nKillAward);
    SCHEMA_FIELD(int32_t, m_nPrimaryReserveAmmoMax);
    SCHEMA_FIELD(int32_t, m_nSecondaryReserveAmmoMax);
    SCHEMA_FIELD(bool, m_bMeleeWeapon);
    SCHEMA_FIELD(bool, m_bHasBurstMode);
    SCHEMA_FIELD(bool, m_bIsRevolver);
    SCHEMA_FIELD(bool, m_bCannotShootUnderwater);
    SCHEMA_FIELD(CGlobalSymbol, m_szName);
    SCHEMA_FIELD(CSWeaponSilencerType, m_eSilencerType);
    SCHEMA_FIELD(bool, m_bShowCrosshair);
    SCHEMA_FIELD(bool, m_bIsFullAuto);
    SCHEMA_FIELD(int32_t, m_nNumBullets);
    SCHEMA_FIELD(bool, m_bReloadsSingleShells);
    SCHEMA_FIELD(CFiringModeFloat, m_flCycleTime);
    SCHEMA_FIELD(float, m_flCycleTimeWhenInBurstMode);
    SCHEMA_FIELD(float, m_flTimeBetweenBurstShots);
    SCHEMA_FIELD(CFiringModeFloat, m_flMaxSpeed);
    SCHEMA_FIELD(CFiringModeFloat, m_flSpread);
    SCHEMA_FIELD(CFiringModeFloat, m_flInaccuracyCrouch);
    SCHEMA_FIELD(CFiringModeFloat, m_flInaccuracyStand);
    SCHEMA_FIELD(CFiringModeFloat, m_flInaccuracyJump);
    SCHEMA_FIELD(CFiringModeFloat, m_flInaccuracyLand);
    SCHEMA_FIELD(CFiringModeFloat, m_flInaccuracyLadder);
    SCHEMA_FIELD(CFiringModeFloat, m_flInaccuracyFire);
    SCHEMA_FIELD(CFiringModeFloat, m_flInaccuracyMove);
    SCHEMA_FIELD(CFiringModeFloat, m_flRecoilAngle);
    SCHEMA_FIELD(CFiringModeFloat, m_flRecoilAngleVariance);
    SCHEMA_FIELD(CFiringModeFloat, m_flRecoilMagnitude);
    SCHEMA_FIELD(CFiringModeFloat, m_flRecoilMagnitudeVariance);
    SCHEMA_FIELD(CFiringModeInt, m_nTracerFrequency);
    SCHEMA_FIELD(float, m_flInaccuracyJumpInitial);
    SCHEMA_FIELD(float, m_flInaccuracyJumpApex);
    SCHEMA_FIELD(float, m_flInaccuracyReload);
    SCHEMA_FIELD(float, m_flDeployDuration);
    SCHEMA_FIELD(float, m_flDisallowAttackAfterReloadStartDuration);
    SCHEMA_FIELD(int32_t, m_nBurstShotCount);
    SCHEMA_FIELD(bool, m_bAllowBurstHolster);
    SCHEMA_FIELD(int32_t, m_nRecoilSeed);
    SCHEMA_FIELD(int32_t, m_nSpreadSeed);
    SCHEMA_FIELD(float, m_flAttackMovespeedFactor);
    SCHEMA_FIELD(float, m_flInaccuracyPitchShift);
    SCHEMA_FIELD(float, m_flInaccuracyAltSoundThreshold);
    SCHEMA_FIELD(CUtlString, m_szUseRadioSubtitle);
    SCHEMA_FIELD(bool, m_bUnzoomsAfterShot);
    SCHEMA_FIELD(bool, m_bHideViewModelWhenZoomed);
    SCHEMA_FIELD(int32_t, m_nZoomLevels);
    SCHEMA_FIELD(int32_t, m_nZoomFOV1);
    SCHEMA_FIELD(int32_t, m_nZoomFOV2);
    SCHEMA_FIELD(float, m_flZoomTime0);
    SCHEMA_FIELD(float, m_flZoomTime1);
    SCHEMA_FIELD(float, m_flZoomTime2);
    SCHEMA_FIELD(float, m_flIronSightPullUpSpeed);
    SCHEMA_FIELD(float, m_flIronSightPutDownSpeed);
    SCHEMA_FIELD(float, m_flIronSightFOV);
    SCHEMA_FIELD(float, m_flIronSightPivotForward);
    SCHEMA_FIELD(float, m_flIronSightLooseness);
    SCHEMA_FIELD(int32_t, m_nDamage);
    SCHEMA_FIELD(float, m_flHeadshotMultiplier);
    SCHEMA_FIELD(float, m_flArmorRatio);
    SCHEMA_FIELD(float, m_flPenetration);
    SCHEMA_FIELD(float, m_flRange);
    SCHEMA_FIELD(float, m_flRangeModifier);
    SCHEMA_FIELD(float, m_flFlinchVelocityModifierLarge);
    SCHEMA_FIELD(float, m_flFlinchVelocityModifierSmall);
    SCHEMA_FIELD(float, m_flRecoveryTimeCrouch);
    SCHEMA_FIELD(float, m_flRecoveryTimeStand);
    SCHEMA_FIELD(float, m_flRecoveryTimeCrouchFinal);
    SCHEMA_FIELD(float, m_flRecoveryTimeStandFinal);
    SCHEMA_FIELD(int32_t, m_nRecoveryTransitionStartBullet);
    SCHEMA_FIELD(int32_t, m_nRecoveryTransitionEndBullet);
    SCHEMA_FIELD(float, m_flThrowVelocity);
    SCHEMA_FIELD(Vector, m_vSmokeColor);
    SCHEMA_FIELD(CGlobalSymbol, m_szAnimClass);

public:
    /// <summary>Looks weapon VData up by its key, e.g. "weapon_ak47". A weapon type other than -1 also requires the data to be of that type. Null when not found.</summary>
    static CCSWeaponBaseVData* FromKey(const char* pszKey, int nWeaponType = -1);
};

#endif // _INCLUDE_CCSWEAPONBASEVDATA_H
