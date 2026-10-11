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

#ifndef _INCLUDE_CHOSTAGE_H
#define _INCLUDE_CHOSTAGE_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CHostageExpresserShim.h"
#include "CountdownTimer.h"
#include "EntitySpottedState_t.h"
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

class CBaseEntity;
class CCSPlayerPawn;
class CCSPlayerPawnBase;

class CHostage : public CHostageExpresserShim
{
public:
    DECLARE_SCHEMA_CLASS(CHostage);

    SCHEMA_FIELD(CEntityIOOutput, m_OnHostageBeginGrab);
    SCHEMA_FIELD(CEntityIOOutput, m_OnFirstPickedUp);
    SCHEMA_FIELD(CEntityIOOutput, m_OnDroppedNotRescued);
    SCHEMA_FIELD(CEntityIOOutput, m_OnRescued);
    SCHEMA_FIELD(EntitySpottedState_t, m_entitySpottedState);
    SCHEMA_FIELD(int32_t, m_nSpotRules);
    SCHEMA_FIELD(uint32_t, m_uiHostageSpawnExclusionGroupMask);
    SCHEMA_FIELD(uint32_t, m_nHostageSpawnRandomFactor);
    SCHEMA_FIELD(bool, m_bRemove);
    SCHEMA_FIELD(Vector, m_vel);
    SCHEMA_FIELD(bool, m_isRescued);
    SCHEMA_FIELD(bool, m_jumpedThisFrame);
    SCHEMA_FIELD(int32_t, m_nHostageState);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_leader);
    SCHEMA_FIELD(CHandle<CCSPlayerPawnBase>, m_lastLeader);
    SCHEMA_FIELD(CountdownTimer, m_reuseTimer);
    SCHEMA_FIELD(bool, m_hasBeenUsed);
    SCHEMA_FIELD(Vector, m_accel);
    SCHEMA_FIELD(bool, m_isRunning);
    SCHEMA_FIELD(bool, m_isCrouching);
    SCHEMA_FIELD(CountdownTimer, m_jumpTimer);
    SCHEMA_FIELD(bool, m_isWaitingForLeader);
    SCHEMA_FIELD(CountdownTimer, m_repathTimer);
    SCHEMA_FIELD(CountdownTimer, m_inhibitDoorTimer);
    SCHEMA_FIELD(CountdownTimer, m_inhibitObstacleAvoidanceTimer);
    SCHEMA_FIELD(CountdownTimer, m_wiggleTimer);
    SCHEMA_FIELD(bool, m_isAdjusted);
    SCHEMA_FIELD(bool, m_bHandsHaveBeenCut);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_hHostageGrabber);
    SCHEMA_FIELD(float, m_fLastGrabTime);
    SCHEMA_FIELD(Vector, m_vecPositionWhenStartedDroppingToGround);
    SCHEMA_FIELD(Vector, m_vecGrabbedPos);
    SCHEMA_FIELD(float, m_flRescueStartTime);
    SCHEMA_FIELD(float, m_flGrabSuccessTime);
    SCHEMA_FIELD(float, m_flDropStartTime);
    SCHEMA_FIELD(int32_t, m_nApproachRewardPayouts);
    SCHEMA_FIELD(int32_t, m_nPickupEventCount);
    SCHEMA_FIELD(Vector, m_vecSpawnGroundPos);
    SCHEMA_FIELD(Vector, m_vecHostageResetPosition);

public:
    static CHostage* New(const char* className)
    {
        return CBaseEntity::New<CHostage>(className);
    }

    static CHostage* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CHostage>(iIndex);
    }

    static CHostage* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CHostage> GetHandle()
    {
        return CBaseEntity::GetHandle<CHostage>();
    }
};

#endif // _INCLUDE_CHOSTAGE_H
