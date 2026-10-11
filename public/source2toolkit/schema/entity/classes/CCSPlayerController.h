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

#ifndef _INCLUDE_CCSPLAYERCONTROLLER_H
#define _INCLUDE_CCSPLAYERCONTROLLER_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/QuestProgress__Reason.h"
#include "CBaseEntity.h"
#include "CBasePlayerController.h"
#include "IntervalTimer.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"

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

class CCSObserverPawn;
class CCSPlayerController_ActionTrackingServices;
class CCSPlayerController_DamageServices;
class CCSPlayerController_InGameMoneyServices;
class CCSPlayerController_InventoryServices;
class CCSPlayerPawn;

class CCSPlayerController : public CBasePlayerController
{
public:
    DECLARE_SCHEMA_CLASS(CCSPlayerController);

    SCHEMA_FIELD(CCSPlayerController_InGameMoneyServices*, m_pInGameMoneyServices);
    SCHEMA_FIELD(CCSPlayerController_InventoryServices*, m_pInventoryServices);
    SCHEMA_FIELD(CCSPlayerController_ActionTrackingServices*, m_pActionTrackingServices);
    SCHEMA_FIELD(CCSPlayerController_DamageServices*, m_pDamageServices);
    SCHEMA_FIELD(uint32_t, m_iPing);
    SCHEMA_FIELD(bool, m_bHasCommunicationAbuseMute);
    SCHEMA_FIELD(uint32_t, m_uiCommunicationMuteFlags);
    SCHEMA_FIELD(CUtlSymbolLarge, m_szCrosshairCodes);
    SCHEMA_FIELD(uint8_t, m_iPendingTeamNum);
    SCHEMA_FIELD(float, m_flForceTeamTime);
    SCHEMA_FIELD(int32_t, m_iCompTeammateColor);
    SCHEMA_FIELD(bool, m_bEverPlayedOnTeam);
    SCHEMA_FIELD(bool, m_bAttemptedToGetColor);
    SCHEMA_FIELD(int32_t, m_iTeammatePreferredColor);
    SCHEMA_FIELD(bool, m_bTeamChanged);
    SCHEMA_FIELD(bool, m_bInSwitchTeam);
    SCHEMA_FIELD(bool, m_bHasSeenJoinGame);
    SCHEMA_FIELD(bool, m_bJustBecameSpectator);
    SCHEMA_FIELD(bool, m_bSwitchTeamsOnNextRoundReset);
    SCHEMA_FIELD(bool, m_bRemoveAllItemsOnNextRoundReset);
    SCHEMA_FIELD(float, m_flLastJoinTeamTime);
    SCHEMA_FIELD(CUtlSymbolLarge, m_szClan);
    SCHEMA_FIELD(uint32_t, m_unClanId32bit);
    SCHEMA_FIELD(int32_t, m_iCoachingTeam);
    SCHEMA_FIELD(uint64_t, m_nPlayerDominated);
    SCHEMA_FIELD(uint64_t, m_nPlayerDominatingMe);
    SCHEMA_FIELD(int32_t, m_iCompetitiveRanking);
    SCHEMA_FIELD(int32_t, m_iCompetitiveWins);
    SCHEMA_FIELD(int8_t, m_iCompetitiveRankType);
    SCHEMA_FIELD(int32_t, m_iCompetitiveRankingPredicted_Win);
    SCHEMA_FIELD(int32_t, m_iCompetitiveRankingPredicted_Loss);
    SCHEMA_FIELD(int32_t, m_iCompetitiveRankingPredicted_Tie);
    SCHEMA_FIELD(int32_t, m_nEndMatchNextMapVote);
    SCHEMA_FIELD(uint16_t, m_unActiveQuestId);
    SCHEMA_FIELD(uint32_t, m_rtActiveMissionPeriod);
    SCHEMA_FIELD(QuestProgress__Reason, m_nQuestProgressReason);
    SCHEMA_FIELD(uint32_t, m_unPlayerTvControlFlags);
    SCHEMA_FIELD(int32_t, m_iDraftIndex);
    SCHEMA_FIELD(uint32_t, m_msQueuedModeDisconnectionTimestamp);
    SCHEMA_FIELD(uint32_t, m_uiAbandonRecordedReason);
    SCHEMA_FIELD(uint32_t, m_eNetworkDisconnectionReason);
    SCHEMA_FIELD(bool, m_bCannotBeKicked);
    SCHEMA_FIELD(bool, m_bEverFullyConnected);
    SCHEMA_FIELD(bool, m_bAbandonAllowsSurrender);
    SCHEMA_FIELD(bool, m_bAbandonOffersInstantSurrender);
    SCHEMA_FIELD(bool, m_bDisconnection1MinWarningPrinted);
    SCHEMA_FIELD(bool, m_bScoreReported);
    SCHEMA_FIELD(int32_t, m_nDisconnectionTick);
    SCHEMA_FIELD(bool, m_bControllingBot);
    SCHEMA_FIELD(bool, m_bHasControlledBotThisRound);
    SCHEMA_FIELD(bool, m_bHasBeenControlledByPlayerThisRound);
    SCHEMA_FIELD(int32_t, m_nBotsControlledThisRound);
    SCHEMA_FIELD(bool, m_bCanControlObservedBot);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_hPlayerPawn);
    SCHEMA_FIELD(CHandle<CCSObserverPawn>, m_hObserverPawn);
    SCHEMA_FIELD(int32_t, m_DesiredObserverMode);
    SCHEMA_FIELD(CEntityHandle, m_hDesiredObserverTarget);
    SCHEMA_FIELD(bool, m_bPawnIsAlive);
    SCHEMA_FIELD(uint32_t, m_iPawnHealth);
    SCHEMA_FIELD(int32_t, m_iPawnArmor);
    SCHEMA_FIELD(bool, m_bPawnHasDefuser);
    SCHEMA_FIELD(bool, m_bPawnHasHelmet);
    SCHEMA_FIELD(uint16_t, m_nPawnCharacterDefIndex);
    SCHEMA_FIELD(int32_t, m_iPawnLifetimeStart);
    SCHEMA_FIELD(int32_t, m_iPawnLifetimeEnd);
    SCHEMA_FIELD(int32_t, m_iPawnBotDifficulty);
    SCHEMA_FIELD(CHandle<CCSPlayerController>, m_hOriginalControllerOfCurrentPawn);
    SCHEMA_FIELD(int32_t, m_iScore);
    SCHEMA_FIELD(int32_t, m_iRoundScore);
    SCHEMA_FIELD(int32_t, m_iRoundsWon);
    SCHEMA_FIELD_POINTER(uint8_t, m_recentKillQueue);
    SCHEMA_FIELD(uint8_t, m_nFirstKill);
    SCHEMA_FIELD(uint8_t, m_nKillCount);
    SCHEMA_FIELD(bool, m_bMvpNoMusic);
    SCHEMA_FIELD(int32_t, m_eMvpReason);
    SCHEMA_FIELD(int32_t, m_iMusicKitID);
    SCHEMA_FIELD(int32_t, m_iMusicKitMVPs);
    SCHEMA_FIELD(int32_t, m_iMVPs);
    SCHEMA_FIELD(int32_t, m_nUpdateCounter);
    SCHEMA_FIELD(float, m_flSmoothedPing);
    SCHEMA_FIELD(IntervalTimer, m_lastHeldVoteTimer);
    SCHEMA_FIELD(bool, m_bShowHints);
    SCHEMA_FIELD(int32_t, m_iNextTimeCheck);
    SCHEMA_FIELD(bool, m_bJustDidTeamKill);
    SCHEMA_FIELD(bool, m_bPunishForTeamKill);
    SCHEMA_FIELD(bool, m_bGaveTeamDamageWarning);
    SCHEMA_FIELD(bool, m_bGaveTeamDamageWarningThisRound);
    SCHEMA_FIELD(double, m_dblLastReceivedPacketPlatFloatTime);
    SCHEMA_FIELD(float, m_LastTeamDamageWarningTime);
    SCHEMA_FIELD(float, m_LastTimePlayerWasDisconnectedForPawnsRemove);
    SCHEMA_FIELD(uint32_t, m_nSuspiciousHitCount);
    SCHEMA_FIELD(uint32_t, m_nNonSuspiciousHitStreak);
    SCHEMA_FIELD(bool, m_bFireBulletsSeedSynchronized);

public:
    /// <summary>Get controller from pawn.</summary>
    static CCSPlayerController* FromPawn(CCSPlayerPawn* pPawn);
    /// <summary>Get controller from slot.</summary>
    static CCSPlayerController* FromSlot(int iSlot);
    /// <summary>Get controller from slot.</summary>
    static CCSPlayerController* FromSlot(CPlayerSlot slot);
    /// <summary>Get controller from user id.</summary>
    static CCSPlayerController* FromUserId(int iUserId);
    /// <summary>Get controller from user id.</summary>
    static CCSPlayerController* FromUserId(CPlayerUserId userId);
    /// <summary>Get controller from steam id.</summary>
    static CCSPlayerController* FromSteamId(uint64 uSteamId);
    /// <summary>Get controller from steam id.</summary>
    static CCSPlayerController* FromSteamId(CSteamID steamId);
    /// <summary>Print to center in HTML.</summary>
    void PrintToCenterHtml(const char* pszMessage, int iDuration = 5, bool bMenu = false);
    /// <summary>Take damage from player</summary>
    void TakeDamage(CCSPlayerController* pAttacker, int iDamage, DamageTypes_t bitsDamageType, HookChain eChain = HookChain::Bypass);
    /// <summary>Respawn player.</summary>
    void Respawn(HookChain eChain = HookChain::Run);
    /// <summary>Switch team without killing.</summary>
    void SwitchTeam(int nTeam, HookChain eChain = HookChain::Run);
    /// <summary>Change team like jointeam.</summary>
    void ChangeTeam(int nTeam, HookChain eChain = HookChain::Run);
    /// <summary>Get player pawn.</summary>
    CCSPlayerPawn* GetPlayerPawn();
    /// <summary>Get observer pawn.</summary>
    CCSObserverPawn* GetObserverPawn();

public:
    static CCSPlayerController* New(const char* className)
    {
        return CBaseEntity::New<CCSPlayerController>(className);
    }

    static CCSPlayerController* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CCSPlayerController>(iIndex);
    }

    static CCSPlayerController* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CCSPlayerController> GetHandle()
    {
        return CBaseEntity::GetHandle<CCSPlayerController>();
    }
};

#endif // _INCLUDE_CCSPLAYERCONTROLLER_H
