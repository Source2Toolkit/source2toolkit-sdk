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

#ifndef _INCLUDE_CCSWEAPONBASE_H
#define _INCLUDE_CCSWEAPONBASE_H

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

#include "CBasePlayerWeapon.h"
#include "CIronSightController.h"
#include "../enums/CSWeaponMode.h"
#include "../enums/WeaponGameplayAnimState.h"

class CCSPlayerPawn;

class CCSWeaponBase : public CBasePlayerWeapon
{
public:
    DECLARE_SCHEMA_CLASS(CCSWeaponBase);

    SCHEMA_FIELD(bool, m_bRemoveable);
    SCHEMA_FIELD(bool, m_bPlayerAmmoStockOnPickup);
    SCHEMA_FIELD(bool, m_bRequireUseToTouch);
    SCHEMA_FIELD(WeaponGameplayAnimState, m_iWeaponGameplayAnimState);
    SCHEMA_FIELD(float, m_flWeaponGameplayAnimStateTimestamp);
    SCHEMA_FIELD(float, m_flInspectCancelCompleteTime);
    SCHEMA_FIELD(bool, m_bInspectPending);
    SCHEMA_FIELD(bool, m_bInspectShouldLoop);
    SCHEMA_FIELD(int32_t, m_nLastEmptySoundCmdNum);
    SCHEMA_FIELD(bool, m_bFireOnEmpty);
    SCHEMA_FIELD(CEntityIOOutput, m_OnPlayerPickup);
    SCHEMA_FIELD(CSWeaponMode, m_weaponMode);
    SCHEMA_FIELD(float, m_flTurningInaccuracyDelta);
    SCHEMA_FIELD(Vector, m_vecTurningInaccuracyEyeDirLast);
    SCHEMA_FIELD(float, m_flTurningInaccuracy);
    SCHEMA_FIELD(float, m_fAccuracyPenalty);
    SCHEMA_FIELD(float, m_flLastAccuracyUpdateTime);
    SCHEMA_FIELD(float, m_fAccuracySmoothedForZoom);
    SCHEMA_FIELD(int32_t, m_iRecoilIndex);
    SCHEMA_FIELD(float, m_flRecoilIndex);
    SCHEMA_FIELD(bool, m_bBurstMode);
    SCHEMA_FIELD(int32_t, m_nPostponeFireReadyTicks);
    SCHEMA_FIELD(float, m_flPostponeFireReadyFrac);
    SCHEMA_FIELD(bool, m_bInReload);
    SCHEMA_FIELD(int32_t, m_nDeployTick);
    SCHEMA_FIELD(float, m_flAttackHoldStartTime);
    SCHEMA_FIELD(float, m_flDroppedAtTime);
    SCHEMA_FIELD(bool, m_bIsHauledBack);
    SCHEMA_FIELD(bool, m_bSilencerOn);
    SCHEMA_FIELD(float, m_flTimeSilencerSwitchComplete);
    SCHEMA_FIELD(bool, m_bStealthy);
    SCHEMA_FIELD(bool, m_bInSilentReloadSection);
    SCHEMA_FIELD(bool, m_bSilentReloadStatCounted);
    SCHEMA_FIELD(bool, m_bSilentReloadStatPending);
    SCHEMA_FIELD(float, m_flStealthHoldStartTime);
    SCHEMA_FIELD(bool, m_bReloadHeldSinceStart);
    SCHEMA_FIELD(float, m_flWeaponActionPlaybackRate);
    SCHEMA_FIELD(int32_t, m_iOriginalTeamNumber);
    SCHEMA_FIELD(int32_t, m_iMostRecentTeamNumber);
    SCHEMA_FIELD(bool, m_bDroppedNearBuyZone);
    SCHEMA_FIELD(float, m_flNextAttackRenderTimeOffset);
    SCHEMA_FIELD(bool, m_bCanBePickedUp);
    SCHEMA_FIELD(bool, m_bUseCanOverrideNextOwnerTouchTime);
    SCHEMA_FIELD(float, m_nextOwnerTouchTime);
    SCHEMA_FIELD(float, m_nextPrevOwnerTouchTime);
    SCHEMA_FIELD(float, m_nextPrevOwnerUseTime);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_hPrevOwner);
    SCHEMA_FIELD(int32_t, m_nDropTick);
    SCHEMA_FIELD(bool, m_bWasActiveWeaponWhenDropped);
    SCHEMA_FIELD(bool, m_donated);
    SCHEMA_FIELD(float, m_fLastShotTime);
    SCHEMA_FIELD(bool, m_bWasOwnedByCT);
    SCHEMA_FIELD(bool, m_bWasOwnedByTerrorist);
    SCHEMA_FIELD(int32_t, m_numRemoveUnownedWeaponThink);
    SCHEMA_FIELD(CIronSightController, m_IronSightController);
    SCHEMA_FIELD(int32_t, m_iIronSightMode);
    SCHEMA_FIELD(float, m_flLastLOSTraceFailureTime);
    SCHEMA_FIELD(float, m_flWatTickOffset);
    SCHEMA_FIELD(float, m_flLastShakeTime);

public:
    static CCSWeaponBase* New(const char* className)
    {
        return CBaseEntity::New<CCSWeaponBase>(className);
    }

    static CCSWeaponBase* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CCSWeaponBase>(iIndex);
    }

    static CCSWeaponBase* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CCSWeaponBase> GetHandle()
    {
        return CBaseEntity::GetHandle<CCSWeaponBase>();
    }
};

#endif // _INCLUDE_CCSWEAPONBASE_H
