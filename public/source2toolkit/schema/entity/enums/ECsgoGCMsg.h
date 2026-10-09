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

#ifndef _INCLUDE_ECSGOGCMSG_H
#define _INCLUDE_ECSGOGCMSG_H

#pragma once

#include <cstdint>

enum class ECsgoGCMsg : uint32_t
{
    k_EMsgGCCStrike15_v2_Base = 9100,
    k_EMsgGCCStrike15_v2_MatchmakingStart = 9101,
    k_EMsgGCCStrike15_v2_MatchmakingStop = 9102,
    k_EMsgGCCStrike15_v2_MatchmakingClient2ServerPing = 9103,
    k_EMsgGCCStrike15_v2_MatchmakingGC2ClientUpdate = 9104,
    k_EMsgGCCStrike15_v2_MatchmakingServerReservationResponse = 9106,
    k_EMsgGCCStrike15_v2_MatchmakingGC2ClientReserve = 9107,
    k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello = 9109,
    k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello = 9110,
    k_EMsgGCCStrike15_v2_MatchmakingGC2ClientAbandon = 9112,
    k_EMsgGCCStrike15_v2_MatchmakingOperator2GCBlogUpdate = 9117,
    k_EMsgGCCStrike15_v2_ServerNotificationForUserPenalty = 9118,
    k_EMsgGCCStrike15_v2_ClientReportPlayer = 9119,
    k_EMsgGCCStrike15_v2_ClientReportServer = 9120,
    k_EMsgGCCStrike15_v2_ClientCommendPlayer = 9121,
    k_EMsgGCCStrike15_v2_ClientReportResponse = 9122,
    k_EMsgGCCStrike15_v2_ClientCommendPlayerQuery = 9123,
    k_EMsgGCCStrike15_v2_ClientCommendPlayerQueryResponse = 9124,
    k_EMsgGCCStrike15_v2_WatchInfoUsers = 9126,
    k_EMsgGCCStrike15_v2_ClientRequestPlayersProfile = 9127,
    k_EMsgGCCStrike15_v2_PlayersProfile = 9128,
    k_EMsgGCCStrike15_v2_PlayerOverwatchCaseUpdate = 9131,
    k_EMsgGCCStrike15_v2_PlayerOverwatchCaseAssignment = 9132,
    k_EMsgGCCStrike15_v2_PlayerOverwatchCaseStatus = 9133,
    k_EMsgGCCStrike15_v2_GC2ClientTextMsg = 9134,
    k_EMsgGCCStrike15_v2_Client2GCTextMsg = 9135,
    k_EMsgGCCStrike15_v2_MatchEndRunRewardDrops = 9136,
    k_EMsgGCCStrike15_v2_MatchEndRewardDropsNotification = 9137,
    k_EMsgGCCStrike15_v2_ClientRequestWatchInfoFriends2 = 9138,
    k_EMsgGCCStrike15_v2_MatchList = 9139,
    k_EMsgGCCStrike15_v2_MatchListRequestCurrentLiveGames = 9140,
    k_EMsgGCCStrike15_v2_MatchListRequestRecentUserGames = 9141,
    k_EMsgGCCStrike15_v2_GC2ServerReservationUpdate = 9142,
    k_EMsgGCCStrike15_v2_ClientVarValueNotificationInfo = 9144,
    k_EMsgGCCStrike15_v2_MatchListRequestTournamentGames = 9146,
    k_EMsgGCCStrike15_v2_MatchListRequestFullGameInfo = 9147,
    k_EMsgGCCStrike15_v2_GiftsLeaderboardRequest = 9148,
    k_EMsgGCCStrike15_v2_GiftsLeaderboardResponse = 9149,
    k_EMsgGCCStrike15_v2_ServerVarValueNotificationInfo = 9150,
    k_EMsgGCCStrike15_v2_ClientSubmitSurveyVote = 9152,
    k_EMsgGCCStrike15_v2_Server2GCClientValidate = 9153,
    k_EMsgGCCStrike15_v2_MatchListRequestLiveGameForUser = 9154,
    k_EMsgGCCStrike15_v2_Client2GCEconPreviewDataBlockRequest = 9156,
    k_EMsgGCCStrike15_v2_Client2GCEconPreviewDataBlockResponse = 9157,
    k_EMsgGCCStrike15_v2_AccountPrivacySettings = 9158,
    k_EMsgGCCStrike15_v2_SetMyActivityInfo = 9159,
    k_EMsgGCCStrike15_v2_MatchListRequestTournamentPredictions = 9160,
    k_EMsgGCCStrike15_v2_MatchListUploadTournamentPredictions = 9161,
    k_EMsgGCCStrike15_v2_DraftSummary = 9162,
    k_EMsgGCCStrike15_v2_ClientRequestJoinFriendData = 9163,
    k_EMsgGCCStrike15_v2_ClientRequestJoinServerData = 9164,
    k_EMsgGCCStrike15_v2_GC2ClientTournamentInfo = 9167,
    k_EMsgGC_GlobalGame_Subscribe = 9168,
    k_EMsgGC_GlobalGame_Unsubscribe = 9169,
    k_EMsgGC_GlobalGame_Play = 9170,
    k_EMsgGCCStrike15_v2_AcknowledgePenalty = 9171,
    k_EMsgGCCStrike15_v2_Client2GCRequestPrestigeCoin = 9172,
    k_EMsgGCCStrike15_v2_GC2ClientGlobalStats = 9173,
    k_EMsgGCCStrike15_v2_Client2GCStreamUnlock = 9174,
    k_EMsgGCCStrike15_v2_FantasyRequestClientData = 9175,
    k_EMsgGCCStrike15_v2_FantasyUpdateClientData = 9176,
    k_EMsgGCCStrike15_v2_GCToClientSteamdatagramTicket = 9177,
    k_EMsgGCCStrike15_v2_ClientToGCRequestTicket = 9178,
    k_EMsgGCCStrike15_v2_ClientToGCRequestElevate = 9179,
    k_EMsgGCCStrike15_v2_GlobalChat = 9180,
    k_EMsgGCCStrike15_v2_GlobalChat_Subscribe = 9181,
    k_EMsgGCCStrike15_v2_GlobalChat_Unsubscribe = 9182,
    k_EMsgGCCStrike15_v2_ClientAuthKeyCode = 9183,
    k_EMsgGCCStrike15_v2_GotvSyncPacket = 9184,
    k_EMsgGCCStrike15_v2_ClientPlayerDecalSign = 9185,
    k_EMsgGCCStrike15_v2_ClientLogonFatalError = 9187,
    k_EMsgGCCStrike15_v2_ClientPollState = 9188,
    k_EMsgGCCStrike15_v2_Party_Register = 9189,
    k_EMsgGCCStrike15_v2_Party_Unregister = 9190,
    k_EMsgGCCStrike15_v2_Party_Search = 9191,
    k_EMsgGCCStrike15_v2_Party_Invite = 9192,
    k_EMsgGCCStrike15_v2_Account_RequestCoPlays = 9193,
    k_EMsgGCCStrike15_v2_ClientGCRankUpdate = 9194,
    k_EMsgGCCStrike15_v2_ClientRequestOffers = 9195,
    k_EMsgGCCStrike15_v2_ClientAccountBalance = 9196,
    k_EMsgGCCStrike15_v2_ClientPartyJoinRelay = 9197,
    k_EMsgGCCStrike15_v2_ClientPartyWarning = 9198,
    k_EMsgGCCStrike15_v2_SetEventFavorite = 9200,
    k_EMsgGCCStrike15_v2_GetEventFavorites_Request = 9201,
    k_EMsgGCCStrike15_v2_ClientPerfReport = 9202,
    k_EMsgGCCStrike15_v2_GetEventFavorites_Response = 9203,
    k_EMsgGCCStrike15_v2_ClientRequestSouvenir = 9204,
    k_EMsgGCCStrike15_v2_ClientRedeemMissionReward = 9209,
    k_EMsgGCCStrike15_ClientDeepStats = 9210,
    k_EMsgGCCStrike15_StartAgreementSessionInGame = 9211,
    k_EMsgGCCStrike15_v2_PrivateQueues = 9214,
    k_EMsgGCCStrike15_v2_MatchListTournamentOperatorMgmt = 9215,
    k_EMsgGCCStrike15_v2_BetaEnrollment = 9217,
    k_EMsgGCCStrike15_v2_SetPlayerLeaderboardSafeName = 9218,
    k_EMsgGCCStrike15_v2_ClientRedeemFreeReward = 9219,
    k_EMsgGCCStrike15_v2_ClientNetworkConfig = 9220,
    k_EMsgGCCStrike15_v2_GC2ClientNotifyXPShop = 9221,
    k_EMsgGCCStrike15_v2_Client2GcAckXPShopTracks = 9222,
    k_EMsgGCCStrike15_v2_MatchmakingGC2ClientSearchStats = 9223,
    k_EMsgGCCStrike15_v2_PremierSeasonSummary = 9224,
    k_EMsgGCCStrike15_v2_RequestRecurringMissionSchedule = 9225,
    k_EMsgGCCStrike15_v2_RecurringMissionSchema = 9226,
    k_EMsgGCCStrike15_v2_VolatileItemClaimReward = 9227,
    k_EMsgGCCStrike15_v2_VolatileShopSubscribe = 9228,
    k_EMsgGCCStrike15_v2_SetClanId = 9229,
};

#endif // _INCLUDE_ECSGOGCMSG_H
