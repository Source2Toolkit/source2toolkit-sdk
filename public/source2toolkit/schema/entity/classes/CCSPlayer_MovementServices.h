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

#ifndef _INCLUDE_CCSPLAYER_MOVEMENTSERVICES_H
#define _INCLUDE_CCSPLAYER_MOVEMENTSERVICES_H
#ifdef _WIN32
#pragma once
#endif

#include "CCSPlayerAnimationState.h"
#include "CCSPlayerLegacyJump.h"
#include "CCSPlayerModernJump.h"
#include "CPlayer_MovementServices_Humanoid.h"
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

class CCSPlayer_MovementServices : public CPlayer_MovementServices_Humanoid
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CCSPlayer_MovementServices);

    SCHEMA_FIELD(CCSPlayerAnimationState, m_AnimationState);
    SCHEMA_FIELD(bool, m_bUsingGroundTopologyOffset);
    SCHEMA_FIELD(float, m_flUsingGroundTopologyOffsetTransitionSmoothing);
    SCHEMA_FIELD(Vector, m_vecLadderNormal);
    SCHEMA_FIELD(int32_t, m_nLadderSurfacePropIndex);
    SCHEMA_FIELD(bool, m_bDucked);
    SCHEMA_FIELD(float, m_flDuckAmount);
    SCHEMA_FIELD(float, m_flDuckSpeed);
    SCHEMA_FIELD(bool, m_bDuckOverride);
    SCHEMA_FIELD(bool, m_bDesiresDuck);
    SCHEMA_FIELD(bool, m_bDucking);
    SCHEMA_FIELD(float, m_flDuckRootOffset);
    SCHEMA_FIELD(float, m_flDuckViewOffset);
    SCHEMA_FIELD(float, m_flLastDuckTime);
    SCHEMA_FIELD(float, m_flBombPlantViewOffset);
    SCHEMA_FIELD(Vector2D, m_vecLastPositionAtFullCrouchSpeed);
    SCHEMA_FIELD(bool, m_duckUntilOnGround);
    SCHEMA_FIELD(bool, m_bHasWalkMovedSinceLastJump);
    SCHEMA_FIELD(bool, m_bInStuckTest);
    SCHEMA_FIELD(int32_t, m_nTraceCount);
    SCHEMA_FIELD(int32_t, m_StuckLast);
    SCHEMA_FIELD(bool, m_bSpeedCropped);
    SCHEMA_FIELD(int32_t, m_nOldWaterLevel);
    SCHEMA_FIELD(float, m_flWaterEntryTime);
    SCHEMA_FIELD(Vector, m_vecForward);
    SCHEMA_FIELD(Vector, m_vecLeft);
    SCHEMA_FIELD(Vector, m_vecUp);
    SCHEMA_FIELD(int32_t, m_nGameCodeHasMovedPlayerAfterCommand);
    SCHEMA_FIELD(bool, m_bMadeFootstepNoise);
    SCHEMA_FIELD(int32_t, m_iFootsteps);
    SCHEMA_FIELD(float, m_fStashGrenadeParameterWhen);
    SCHEMA_FIELD(bool, m_bUseFrictionStashedSpeed);
    SCHEMA_FIELD(float, m_flUseFrictionStashedSpeedUntilFrac);
    SCHEMA_FIELD(float, m_flFrictionStashedSpeed);
    SCHEMA_FIELD(float, m_flStamina);
    SCHEMA_FIELD(float, m_flHeightAtJumpStart);
    SCHEMA_FIELD(float, m_flMaxJumpHeightThisJump);
    SCHEMA_FIELD(float, m_flMaxJumpHeightLastJump);
    SCHEMA_FIELD(float, m_flStaminaAtJumpStart);
    SCHEMA_FIELD(float, m_flVelMulAtJumpStart);
    SCHEMA_FIELD(float, m_flAccumulatedJumpError);
    SCHEMA_FIELD(CCSPlayerLegacyJump, m_LegacyJump);
    SCHEMA_FIELD(CCSPlayerModernJump, m_ModernJump);
    SCHEMA_FIELD(int32_t, m_nLastJumpTick);
    SCHEMA_FIELD(float, m_flLastJumpFrac);
    SCHEMA_FIELD(float, m_flLastJumpVelocityZ);
    SCHEMA_FIELD(bool, m_bJumpApexPending);
    SCHEMA_FIELD(float, m_flTicksSinceLastSurfingDetected);
    SCHEMA_FIELD(Vector2D, m_vecWalkWishVel);
    SCHEMA_FIELD(bool, m_bHasEverProcessedCommand);
};

#endif // _INCLUDE_CCSPLAYER_MOVEMENTSERVICES_H
