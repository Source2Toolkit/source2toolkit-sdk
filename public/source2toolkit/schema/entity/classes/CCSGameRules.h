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

#ifndef _INCLUDE_CCSGAMERULES_H
#define _INCLUDE_CCSGAMERULES_H
#ifdef _WIN32
#pragma once
#endif

#include "CRetakeGameRules.h"
#include "CTeamplayRules.h"
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
class CBasePlayerController;
class CCSGameModeRules;
class CCSPlayerController;
class SpawnPoint;

class CCSGameRules : public CTeamplayRules
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CCSGameRules);

    SCHEMA_FIELD(bool, m_bFreezePeriod);
    SCHEMA_FIELD(bool, m_bWarmupPeriod);
    SCHEMA_FIELD(float, m_fWarmupPeriodEnd);
    SCHEMA_FIELD(float, m_fWarmupPeriodStart);
    SCHEMA_FIELD(bool, m_bTerroristTimeOutActive);
    SCHEMA_FIELD(bool, m_bCTTimeOutActive);
    SCHEMA_FIELD(float, m_flTerroristTimeOutRemaining);
    SCHEMA_FIELD(float, m_flCTTimeOutRemaining);
    SCHEMA_FIELD(int32_t, m_nTerroristTimeOuts);
    SCHEMA_FIELD(int32_t, m_nCTTimeOuts);
    SCHEMA_FIELD(bool, m_bTechnicalTimeOut);
    SCHEMA_FIELD(bool, m_bMatchWaitingForResume);
    SCHEMA_FIELD(int32_t, m_iFreezeTime);
    SCHEMA_FIELD(int32_t, m_iRoundTime);
    SCHEMA_FIELD(float, m_fMatchStartTime);
    SCHEMA_FIELD(float, m_fRoundStartTime);
    SCHEMA_FIELD(float, m_flRestartRoundTime);
    SCHEMA_FIELD(bool, m_bGameRestart);
    SCHEMA_FIELD(float, m_flGameStartTime);
    SCHEMA_FIELD(float, m_timeUntilNextPhaseStarts);
    SCHEMA_FIELD(int32_t, m_gamePhase);
    SCHEMA_FIELD(int32_t, m_totalRoundsPlayed);
    SCHEMA_FIELD(int32_t, m_nRoundsPlayedThisPhase);
    SCHEMA_FIELD(int32_t, m_nOvertimePlaying);
    SCHEMA_FIELD(int32_t, m_iHostagesRemaining);
    SCHEMA_FIELD(bool, m_bAnyHostageReached);
    SCHEMA_FIELD(bool, m_bMapHasBombTarget);
    SCHEMA_FIELD(bool, m_bMapHasRescueZone);
    SCHEMA_FIELD(bool, m_bMapHasBuyZone);
    SCHEMA_FIELD(bool, m_bIsQueuedMatchmaking);
    SCHEMA_FIELD(int32_t, m_nQueuedMatchmakingMode);
    SCHEMA_FIELD(bool, m_bIsValveDS);
    SCHEMA_FIELD(bool, m_bLogoMap);
    SCHEMA_FIELD(bool, m_bPlayAllStepSoundsOnServer);
    SCHEMA_FIELD(int32_t, m_iSpectatorSlotCount);
    SCHEMA_FIELD(int32_t, m_MatchDevice);
    SCHEMA_FIELD(bool, m_bHasMatchStarted);
    SCHEMA_FIELD(int32_t, m_nNextMapInMapgroup);
    SCHEMA_FIELD_POINTER(char, m_szTournamentEventName);
    SCHEMA_FIELD_POINTER(char, m_szTournamentEventStage);
    SCHEMA_FIELD_POINTER(char, m_szMatchStatTxt);
    SCHEMA_FIELD_POINTER(char, m_szTournamentPredictionsTxt);
    SCHEMA_FIELD(int32_t, m_nTournamentPredictionsPct);
    SCHEMA_FIELD(float, m_flCMMItemDropRevealStartTime);
    SCHEMA_FIELD(float, m_flCMMItemDropRevealEndTime);
    SCHEMA_FIELD(bool, m_bIsDroppingItems);
    SCHEMA_FIELD(bool, m_bIsQuestEligible);
    SCHEMA_FIELD(bool, m_bIsHltvActive);
    SCHEMA_FIELD(bool, m_bBombPlanted);
    SCHEMA_FIELD_POINTER(uint16_t, m_arrProhibitedItemIndices);
    SCHEMA_FIELD_POINTER(uint32_t, m_arrTournamentActiveCasterAccounts);
    SCHEMA_FIELD(int32_t, m_numBestOfMaps);
    SCHEMA_FIELD(int32_t, m_nHalloweenMaskListSeed);
    SCHEMA_FIELD(bool, m_bBombDropped);
    SCHEMA_FIELD(int32_t, m_iRoundWinStatus);
    SCHEMA_FIELD(int32_t, m_eRoundWinReason);
    SCHEMA_FIELD(bool, m_bTCantBuy);
    SCHEMA_FIELD(bool, m_bCTCantBuy);
    SCHEMA_FIELD_POINTER(int32_t, m_iMatchStats_RoundResults);
    SCHEMA_FIELD_POINTER(int32_t, m_iMatchStats_PlayersAlive_CT);
    SCHEMA_FIELD_POINTER(int32_t, m_iMatchStats_PlayersAlive_T);
    SCHEMA_FIELD_POINTER(float, m_TeamRespawnWaveTimes);
    SCHEMA_FIELD_POINTER(float, m_flNextRespawnWave);
    SCHEMA_FIELD(Vector, m_vMinimapMins);
    SCHEMA_FIELD(Vector, m_vMinimapMaxs);
    SCHEMA_FIELD_POINTER(float, m_MinimapVerticalSectionHeights);
    SCHEMA_FIELD(uint64_t, m_ullLocalMatchID);
    SCHEMA_FIELD_POINTER(int32_t, m_nEndMatchMapGroupVoteTypes);
    SCHEMA_FIELD_POINTER(int32_t, m_nEndMatchMapGroupVoteOptions);
    SCHEMA_FIELD(int32_t, m_nEndMatchMapVoteWinner);
    SCHEMA_FIELD(int32_t, m_iNumConsecutiveCTLoses);
    SCHEMA_FIELD(int32_t, m_iNumConsecutiveTerroristLoses);
    SCHEMA_FIELD(bool, m_bHasHostageBeenTouched);
    SCHEMA_FIELD(float, m_flIntermissionStartTime);
    SCHEMA_FIELD(float, m_flIntermissionEndTime);
    SCHEMA_FIELD(bool, m_bLevelInitialized);
    SCHEMA_FIELD(int32_t, m_iTotalRoundsPlayed);
    SCHEMA_FIELD(int32_t, m_iUnBalancedRounds);
    SCHEMA_FIELD(bool, m_endMatchOnRoundReset);
    SCHEMA_FIELD(bool, m_endMatchOnThink);
    SCHEMA_FIELD(int32_t, m_iNumTerrorist);
    SCHEMA_FIELD(int32_t, m_iNumCT);
    SCHEMA_FIELD(int32_t, m_iNumSpawnableTerrorist);
    SCHEMA_FIELD(int32_t, m_iNumSpawnableCT);
    SCHEMA_FIELD(CUtlVector<int32_t>, m_arrSelectedHostageSpawnIndices);
    SCHEMA_FIELD(int32_t, m_nSpawnPointsRandomSeed);
    SCHEMA_FIELD(bool, m_bFirstConnected);
    SCHEMA_FIELD(bool, m_bCompleteReset);
    SCHEMA_FIELD(bool, m_bPickNewTeamsOnReset);
    SCHEMA_FIELD(bool, m_bScrambleTeamsOnRestart);
    SCHEMA_FIELD(bool, m_bSwapTeamsOnRestart);
    SCHEMA_FIELD(CUtlVector<int32_t>, m_nEndMatchTiedVotes);
    SCHEMA_FIELD(bool, m_bNeedToAskPlayersForContinueVote);
    SCHEMA_FIELD(uint32_t, m_numQueuedMatchmakingAccounts);
    SCHEMA_FIELD(float, m_fAvgPlayerRank);
    SCHEMA_FIELD(char*, m_pQueuedMatchmakingReservationString);
    SCHEMA_FIELD(uint32_t, m_numTotalTournamentDrops);
    SCHEMA_FIELD(uint32_t, m_numSpectatorsCountMax);
    SCHEMA_FIELD(uint32_t, m_numSpectatorsCountMaxTV);
    SCHEMA_FIELD(uint32_t, m_numSpectatorsCountMaxLnk);
    SCHEMA_FIELD(int32_t, m_nCTsAliveAtFreezetimeEnd);
    SCHEMA_FIELD(int32_t, m_nTerroristsAliveAtFreezetimeEnd);
    SCHEMA_FIELD(bool, m_bForceTeamChangeSilent);
    SCHEMA_FIELD(bool, m_bLoadingRoundBackupData);
    SCHEMA_FIELD(int32_t, m_nMatchInfoShowType);
    SCHEMA_FIELD(float, m_flMatchInfoDecidedTime);
    SCHEMA_FIELD(int32_t, mTeamDMLastWinningTeamNumber);
    SCHEMA_FIELD(float, mTeamDMLastThinkTime);
    SCHEMA_FIELD(float, m_flTeamDMLastAnnouncementTime);
    SCHEMA_FIELD(int32_t, m_iAccountTerrorist);
    SCHEMA_FIELD(int32_t, m_iAccountCT);
    SCHEMA_FIELD(int32_t, m_iSpawnPointCount_Terrorist);
    SCHEMA_FIELD(int32_t, m_iSpawnPointCount_CT);
    SCHEMA_FIELD(int32_t, m_iMaxNumTerrorists);
    SCHEMA_FIELD(int32_t, m_iMaxNumCTs);
    SCHEMA_FIELD(int32_t, m_iLoserBonusMostRecentTeam);
    SCHEMA_FIELD(float, m_tmNextPeriodicThink);
    SCHEMA_FIELD(bool, m_bVoiceWonMatchBragFired);
    SCHEMA_FIELD(float, m_fWarmupNextChatNoticeTime);
    SCHEMA_FIELD(int32_t, m_iHostagesRescued);
    SCHEMA_FIELD(int32_t, m_iHostagesTouched);
    SCHEMA_FIELD(float, m_flNextHostageAnnouncement);
    SCHEMA_FIELD(bool, m_bNoTerroristsKilled);
    SCHEMA_FIELD(bool, m_bNoCTsKilled);
    SCHEMA_FIELD(bool, m_bNoEnemiesKilled);
    SCHEMA_FIELD(bool, m_bCanDonateWeapons);
    SCHEMA_FIELD(float, m_firstKillTime);
    SCHEMA_FIELD(float, m_firstBloodTime);
    SCHEMA_FIELD(bool, m_hostageWasInjured);
    SCHEMA_FIELD(bool, m_hostageWasKilled);
    SCHEMA_FIELD(bool, m_bVoteCalled);
    SCHEMA_FIELD(bool, m_bServerVoteOnReset);
    SCHEMA_FIELD(float, m_flVoteCheckThrottle);
    SCHEMA_FIELD(bool, m_bBuyTimeEnded);
    SCHEMA_FIELD(int32_t, m_nLastFreezeEndBeep);
    SCHEMA_FIELD(bool, m_bTargetBombed);
    SCHEMA_FIELD(bool, m_bBombDefused);
    SCHEMA_FIELD(bool, m_bMapHasBombZone);
    SCHEMA_FIELD(Vector, m_vecMainCTSpawnPos);
    SCHEMA_FIELD(CUtlVector<CHandle<SpawnPoint>>, m_CTSpawnPointsMasterList);
    SCHEMA_FIELD(CUtlVector<CHandle<SpawnPoint>>, m_TerroristSpawnPointsMasterList);
    SCHEMA_FIELD(bool, m_bRespawningAllRespawnablePlayers);
    SCHEMA_FIELD(int32_t, m_iNextCTSpawnPoint);
    SCHEMA_FIELD(float, m_flCTSpawnPointUsedTime);
    SCHEMA_FIELD(int32_t, m_iNextTerroristSpawnPoint);
    SCHEMA_FIELD(float, m_flTerroristSpawnPointUsedTime);
    SCHEMA_FIELD(CUtlVector<CHandle<SpawnPoint>>, m_CTSpawnPoints);
    SCHEMA_FIELD(CUtlVector<CHandle<SpawnPoint>>, m_TerroristSpawnPoints);
    SCHEMA_FIELD(bool, m_bIsUnreservedGameServer);
    SCHEMA_FIELD(float, m_fAutobalanceDisplayTime);
    SCHEMA_FIELD(bool, m_bAllowWeaponSwitch);
    SCHEMA_FIELD(bool, m_bRoundTimeWarningTriggered);
    SCHEMA_FIELD(float, m_phaseChangeAnnouncementTime);
    SCHEMA_FIELD(float, m_fNextUpdateTeamClanNamesTime);
    SCHEMA_FIELD(float, m_flLastThinkTime);
    SCHEMA_FIELD(float, m_fAccumulatedRoundOffDamage);
    SCHEMA_FIELD(int32_t, m_nShorthandedBonusLastEvalRound);
    SCHEMA_FIELD(int32_t, m_nMatchAbortedEarlyReason);
    SCHEMA_FIELD(bool, m_bHasTriggeredRoundStartMusic);
    SCHEMA_FIELD(bool, m_bSwitchingTeamsAtRoundReset);
    SCHEMA_FIELD(CCSGameModeRules*, m_pGameModeRules);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hPlayerResource);
    SCHEMA_FIELD(CRetakeGameRules, m_RetakeRules);
    SCHEMA_FIELD_POINTER(CUtlVector<int32_t>, m_arrTeamUniqueKillWeaponsMatch);
    SCHEMA_FIELD_POINTER(bool, m_bTeamLastKillUsedUniqueWeaponMatch);
    SCHEMA_FIELD(uint8_t, m_nMatchEndCount);
    SCHEMA_FIELD(int32_t, m_nTTeamIntroVariant);
    SCHEMA_FIELD(int32_t, m_nCTTeamIntroVariant);
    SCHEMA_FIELD(bool, m_bTeamIntroPeriod);
    SCHEMA_FIELD(float, m_fTeamIntroPeriodEnd);
    SCHEMA_FIELD(bool, m_bPlayedTeamIntroVO);
    SCHEMA_FIELD(int32_t, m_iRoundEndWinnerTeam);
    SCHEMA_FIELD(int32_t, m_eRoundEndReason);
    SCHEMA_FIELD(bool, m_bRoundEndShowTimerDefend);
    SCHEMA_FIELD(int32_t, m_iRoundEndTimerTime);
    SCHEMA_FIELD(CUtlString, m_sRoundEndFunFactToken);
    SCHEMA_FIELD(int32_t, m_iRoundEndFunFactPlayerSlot);
    SCHEMA_FIELD(int32_t, m_iRoundEndFunFactData1);
    SCHEMA_FIELD(int32_t, m_iRoundEndFunFactData2);
    SCHEMA_FIELD(int32_t, m_iRoundEndFunFactData3);
    SCHEMA_FIELD(CUtlString, m_sRoundEndMessage);
    SCHEMA_FIELD(int32_t, m_iRoundEndPlayerCount);
    SCHEMA_FIELD(bool, m_bRoundEndNoMusic);
    SCHEMA_FIELD(int32_t, m_iRoundEndLegacy);
    SCHEMA_FIELD(uint8_t, m_nRoundEndCount);
    SCHEMA_FIELD(int32_t, m_iRoundStartRoundNumber);
    SCHEMA_FIELD(uint8_t, m_nRoundStartCount);
    SCHEMA_FIELD(double, m_flLastPerfSampleTime);

public:
    /// <summary>Ends the match and moves everyone to the intermission screen.</summary>
    void GoToIntermission(bool bAbortedMatch = false, HookChain eChain = HookChain::Run);
    /// <summary>Terminate round.</summary>
    void TerminateRound(float flDelay, int32_t eRoundEndReason, uint32 nTeamId = 0, HookChain eChain = HookChain::Bypass);
    /// <summary>Find entity player is aiming at.</summary>
    CBaseEntity* FindPickerEntity(CBasePlayerController* pPlayer);
    /// <summary>Get aim target.</summary>
    CCSPlayerController* GetClientAimTarget(CCSPlayerController* pPlayer);
};

#endif // _INCLUDE_CCSGAMERULES_H
