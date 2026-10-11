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

#ifndef _INCLUDE_CCSBOT_H
#define _INCLUDE_CCSBOT_H
#ifdef _WIN32
#pragma once
#endif

#include "CBot.h"
#include "CountdownTimer.h"
#include "IntervalTimer.h"
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

class CCSBot : public CBot
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CCSBot);

    SCHEMA_FIELD(Vector, m_eyePosition);
    SCHEMA_FIELD_POINTER(char, m_name);
    SCHEMA_FIELD(float, m_combatRange);
    SCHEMA_FIELD(bool, m_isRogue);
    SCHEMA_FIELD(CountdownTimer, m_rogueTimer);
    SCHEMA_FIELD(bool, m_diedLastRound);
    SCHEMA_FIELD(float, m_safeTime);
    SCHEMA_FIELD(bool, m_wasSafe);
    SCHEMA_FIELD(bool, m_blindFire);
    SCHEMA_FIELD(CountdownTimer, m_surpriseTimer);
    SCHEMA_FIELD(bool, m_bAllowActive);
    SCHEMA_FIELD(bool, m_isFollowing);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_leader);
    SCHEMA_FIELD(float, m_followTimestamp);
    SCHEMA_FIELD(float, m_allowAutoFollowTime);
    SCHEMA_FIELD(CountdownTimer, m_hurryTimer);
    SCHEMA_FIELD(CountdownTimer, m_alertTimer);
    SCHEMA_FIELD(CountdownTimer, m_sneakTimer);
    SCHEMA_FIELD(CountdownTimer, m_panicTimer);
    SCHEMA_FIELD(float, m_stateTimestamp);
    SCHEMA_FIELD(bool, m_isAttacking);
    SCHEMA_FIELD(bool, m_isOpeningDoor);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_taskEntity);
    SCHEMA_FIELD(Vector, m_goalPosition);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_goalEntity);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_avoid);
    SCHEMA_FIELD(float, m_avoidTimestamp);
    SCHEMA_FIELD(bool, m_isStopping);
    SCHEMA_FIELD(bool, m_hasVisitedEnemySpawn);
    SCHEMA_FIELD(IntervalTimer, m_stillTimer);
    SCHEMA_FIELD(bool, m_bEyeAnglesUnderPathFinderControl);
    SCHEMA_FIELD(int32_t, m_pathIndex);
    SCHEMA_FIELD(float, m_areaEnteredTimestamp);
    SCHEMA_FIELD(CountdownTimer, m_repathTimer);
    SCHEMA_FIELD(CountdownTimer, m_avoidFriendTimer);
    SCHEMA_FIELD(bool, m_isFriendInTheWay);
    SCHEMA_FIELD(CountdownTimer, m_politeTimer);
    SCHEMA_FIELD(bool, m_isWaitingBehindFriend);
    SCHEMA_FIELD(float, m_pathLadderEnd);
    SCHEMA_FIELD(CountdownTimer, m_mustRunTimer);
    SCHEMA_FIELD(CountdownTimer, m_waitTimer);
    SCHEMA_FIELD(CountdownTimer, m_updateTravelDistanceTimer);
    SCHEMA_FIELD_POINTER(float, m_playerTravelDistance);
    SCHEMA_FIELD(uint8_t, m_travelDistancePhase);
    SCHEMA_FIELD(uint8_t, m_hostageEscortCount);
    SCHEMA_FIELD(float, m_hostageEscortCountTimestamp);
    SCHEMA_FIELD(int32_t, m_desiredTeam);
    SCHEMA_FIELD(bool, m_hasJoined);
    SCHEMA_FIELD(bool, m_isWaitingForHostage);
    SCHEMA_FIELD(CountdownTimer, m_inhibitWaitingForHostageTimer);
    SCHEMA_FIELD(CountdownTimer, m_waitForHostageTimer);
    SCHEMA_FIELD(Vector, m_noisePosition);
    SCHEMA_FIELD(float, m_noiseTravelDistance);
    SCHEMA_FIELD(float, m_noiseTimestamp);
    SCHEMA_FIELD(CCSPlayerPawn*, m_noiseSource);
    SCHEMA_FIELD(CountdownTimer, m_noiseBendTimer);
    SCHEMA_FIELD(Vector, m_bentNoisePosition);
    SCHEMA_FIELD(bool, m_bendNoisePositionValid);
    SCHEMA_FIELD(float, m_lookAroundStateTimestamp);
    SCHEMA_FIELD(float, m_lookAheadAngle);
    SCHEMA_FIELD(float, m_lookUpAngle);
    SCHEMA_FIELD(float, m_forwardAngle);
    SCHEMA_FIELD(float, m_inhibitLookAroundTimestamp);
    SCHEMA_FIELD(Vector, m_lookAtSpot);
    SCHEMA_FIELD(float, m_lookAtSpotDuration);
    SCHEMA_FIELD(float, m_lookAtSpotTimestamp);
    SCHEMA_FIELD(float, m_lookAtSpotAngleTolerance);
    SCHEMA_FIELD(bool, m_lookAtSpotClearIfClose);
    SCHEMA_FIELD(bool, m_lookAtSpotAttack);
    SCHEMA_FIELD(char*, m_lookAtDesc);
    SCHEMA_FIELD(float, m_peripheralTimestamp);
    SCHEMA_FIELD(uint8_t, m_approachPointCount);
    SCHEMA_FIELD(Vector, m_approachPointViewPosition);
    SCHEMA_FIELD(IntervalTimer, m_viewSteadyTimer);
    SCHEMA_FIELD(CountdownTimer, m_tossGrenadeTimer);
    SCHEMA_FIELD(CountdownTimer, m_isAvoidingGrenade);
    SCHEMA_FIELD(float, m_spotCheckTimestamp);
    SCHEMA_FIELD(int32_t, m_checkedHidingSpotCount);
    SCHEMA_FIELD(float, m_lookPitch);
    SCHEMA_FIELD(float, m_lookPitchVel);
    SCHEMA_FIELD(float, m_lookYaw);
    SCHEMA_FIELD(float, m_lookYawVel);
    SCHEMA_FIELD(Vector, m_targetSpot);
    SCHEMA_FIELD(Vector, m_targetSpotVelocity);
    SCHEMA_FIELD(Vector, m_targetSpotPredicted);
    SCHEMA_FIELD(QAngle, m_aimError);
    SCHEMA_FIELD(QAngle, m_aimGoal);
    SCHEMA_FIELD(float, m_targetSpotTime);
    SCHEMA_FIELD(float, m_aimFocus);
    SCHEMA_FIELD(float, m_aimFocusInterval);
    SCHEMA_FIELD(float, m_aimFocusNextUpdate);
    SCHEMA_FIELD(CountdownTimer, m_ignoreEnemiesTimer);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_enemy);
    SCHEMA_FIELD(bool, m_isEnemyVisible);
    SCHEMA_FIELD(uint8_t, m_visibleEnemyParts);
    SCHEMA_FIELD(Vector, m_lastEnemyPosition);
    SCHEMA_FIELD(float, m_lastSawEnemyTimestamp);
    SCHEMA_FIELD(float, m_firstSawEnemyTimestamp);
    SCHEMA_FIELD(float, m_currentEnemyAcquireTimestamp);
    SCHEMA_FIELD(float, m_enemyDeathTimestamp);
    SCHEMA_FIELD(float, m_friendDeathTimestamp);
    SCHEMA_FIELD(bool, m_isLastEnemyDead);
    SCHEMA_FIELD(int32_t, m_nearbyEnemyCount);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_bomber);
    SCHEMA_FIELD(int32_t, m_nearbyFriendCount);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_closestVisibleFriend);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_closestVisibleHumanFriend);
    SCHEMA_FIELD(IntervalTimer, m_attentionInterval);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_attacker);
    SCHEMA_FIELD(float, m_attackedTimestamp);
    SCHEMA_FIELD(IntervalTimer, m_burnedByFlamesTimer);
    SCHEMA_FIELD(int32_t, m_lastVictimID);
    SCHEMA_FIELD(bool, m_isAimingAtEnemy);
    SCHEMA_FIELD(bool, m_isRapidFiring);
    SCHEMA_FIELD(IntervalTimer, m_equipTimer);
    SCHEMA_FIELD(CountdownTimer, m_zoomTimer);
    SCHEMA_FIELD(float, m_fireWeaponTimestamp);
    SCHEMA_FIELD(CountdownTimer, m_lookForWeaponsOnGroundTimer);
    SCHEMA_FIELD(bool, m_bIsSleeping);
    SCHEMA_FIELD(bool, m_isEnemySniperVisible);
    SCHEMA_FIELD(CountdownTimer, m_sawEnemySniperTimer);
    SCHEMA_FIELD(uint8_t, m_enemyQueueIndex);
    SCHEMA_FIELD(uint8_t, m_enemyQueueCount);
    SCHEMA_FIELD(uint8_t, m_enemyQueueAttendIndex);
    SCHEMA_FIELD(bool, m_isStuck);
    SCHEMA_FIELD(float, m_stuckTimestamp);
    SCHEMA_FIELD(Vector, m_stuckSpot);
    SCHEMA_FIELD(CountdownTimer, m_wiggleTimer);
    SCHEMA_FIELD(CountdownTimer, m_stuckJumpTimer);
    SCHEMA_FIELD(float, m_nextCleanupCheckTimestamp);
    SCHEMA_FIELD_POINTER(float, m_avgVel);
    SCHEMA_FIELD(int32_t, m_avgVelIndex);
    SCHEMA_FIELD(int32_t, m_avgVelCount);
    SCHEMA_FIELD(Vector, m_lastOrigin);
    SCHEMA_FIELD(float, m_lastRadioRecievedTimestamp);
    SCHEMA_FIELD(float, m_lastRadioSentTimestamp);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_radioSubject);
    SCHEMA_FIELD(Vector, m_radioPosition);
    SCHEMA_FIELD(float, m_voiceEndTimestamp);
    SCHEMA_FIELD(int32_t, m_lastValidReactionQueueFrame);
};

#endif // _INCLUDE_CCSBOT_H
