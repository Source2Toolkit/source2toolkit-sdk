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

#ifndef _INCLUDE_CCHICKEN_H
#define _INCLUDE_CCHICKEN_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/EChickenActivity.h"
#include "CAttributeContainer.h"
#include "CBaseEntity.h"
#include "CDynamicProp.h"
#include "CountdownTimer.h"
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
class CCSPlayerController;
class CCSPlayerPawn;

class CChicken : public CDynamicProp
{
public:
    DECLARE_SCHEMA_CLASS(CChicken);

    SCHEMA_FIELD(CAttributeContainer, m_AttributeManager);
    SCHEMA_FIELD(CountdownTimer, m_updateTimer);
    SCHEMA_FIELD(Vector, m_stuckAnchor);
    SCHEMA_FIELD(CountdownTimer, m_collisionStuckTimer);
    SCHEMA_FIELD(bool, m_isOnGround);
    SCHEMA_FIELD(Vector, m_vFallVelocity);
    SCHEMA_FIELD(EChickenActivity, m_desiredActivity);
    SCHEMA_FIELD(EChickenActivity, m_currentActivity);
    SCHEMA_FIELD(CountdownTimer, m_activityTimer);
    SCHEMA_FIELD(float, m_turnRate);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_fleeFrom);
    SCHEMA_FIELD(CountdownTimer, m_moveRateThrottleTimer);
    SCHEMA_FIELD(CountdownTimer, m_startleTimer);
    SCHEMA_FIELD(CountdownTimer, m_vocalizeTimer);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_leader);
    SCHEMA_FIELD(CHandle<CCSPlayerController>, m_owner);
    SCHEMA_FIELD(CountdownTimer, m_reuseTimer);
    SCHEMA_FIELD(CountdownTimer, m_jumpTimer);
    SCHEMA_FIELD(float, m_flLastJumpTime);
    SCHEMA_FIELD(CountdownTimer, m_repathTimer);
    SCHEMA_FIELD(Vector, m_vecPathGoal);
    SCHEMA_FIELD(float, m_flActiveFollowStartTime);
    SCHEMA_FIELD(CountdownTimer, m_followMinuteTimer);
    SCHEMA_FIELD(CountdownTimer, m_BlockDirectionTimer);
    SCHEMA_FIELD(bool, m_bSpawnDyingParticles);

public:
    static CChicken* New(const char* className)
    {
        return CBaseEntity::New<CChicken>(className);
    }

    static CChicken* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CChicken>(iIndex);
    }

    static CChicken* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CChicken> GetHandle()
    {
        return CBaseEntity::GetHandle<CChicken>();
    }
};

#endif // _INCLUDE_CCHICKEN_H
