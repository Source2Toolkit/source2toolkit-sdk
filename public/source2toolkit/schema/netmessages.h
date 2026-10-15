/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
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
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
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
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

#ifndef SCHEMA_NETMESSAGES_H
#define SCHEMA_NETMESSAGES_H
#ifdef _WIN32
#pragma once
#endif

/**

* @file netmessages.h
* @brief Network message wrappers for Source2 networking system.
*
* This file defines strongly-typed wrappers around protobuf-based
* network messages used by the engine.
*
* Messages are grouped into categories:
* * net_* -> core networking
* * clc_* -> client -> server
* * svc_* -> server -> client
*
* @note All classes inherit from CNetMessagePB.
* @note Template parameters define message ID, protobuf type, group and flags.
  */




#include <netmessages.pb.h>
#include <networkbasetypes.pb.h>
#include <networksystem/inetchannel.h>
#include <networksystem/inetworkmessages.h>
#include <networksystem/inetworkserializer.h>
#include <networksystem_protomessages.pb.h>

#undef min
#undef max

/* =========================
Signon groups
========================= */

// s2sdk does not declare these. The values are the engine's own, checked
// against the RTTI of libengine2.so/libserver.so and the group name each
// binding returns from GetGroup(); 1-3 are unused by any message.
enum SignonGroup_t : int32_t
{
    SG_INVALID      = -1,
    SG_GENERIC      = 0,
    SG_LOCALPLAYER  = 1,
    SG_OTHERPLAYER  = 2,
    SG_ENTITIES     = 3,
    SG_SOUNDS       = 4,
    SG_EVENTS       = 5,
    SG_VOICE        = 6,
    SG_STRINGTABLE  = 7,
    SG_MOVE         = 8,
    SG_STRINGCMD    = 9,
    SG_SIGNON       = 10,
    SG_SYSTEM       = 11,
    SG_DECALS       = 12,
    SG_USERMSG      = 13,
    SG_CLIENTMSG    = 14,
    SG_SPAWNGROUPS  = 15,
    SG_ENGINE       = 16,
    SG_HLTVREPLAY   = 17,

    SG_TOTAL        = 18,
};

inline constexpr const char* k_pszNetGroupNames[ SG_TOTAL ] =
{
    "Generic", "Local Player", "Other Players", "Entities", "Sounds", "Events",
    "Voice", "String Table", "Move", "String Command", "Signon", "System",
    "Decals", "User Messages", "Client Messages", "Spawn Groups",
    "Game Engine", "Hltv Replay"
};

/* =========================
Typed message wrappers
========================= */

// s2sdk's CNetMessagePB is a one-parameter stub, and a class template
// cannot be redeclared with a different parameter list, so the message id,
// signon group and buffer type ride along here instead.
//
// No data members and no virtuals of its own, so the layout stays
// CNetMessagePB<T>'s. These wrappers are abstract, like the stub they derive
// from -- fine everywhere they are only cast onto engine memory.
template< int32_t ID, typename PROTO_TYPE, SignonGroup_t SIGNON = SG_GENERIC,
          NetChannelBufType_t BUF_TYPE = BUF_RELIABLE, bool IS_FOR_SERVER = false >
class CNetMessagePBTyped : public CNetMessagePB< PROTO_TYPE >
{
public:
    using MyType_t = CNetMessagePBTyped< ID, PROTO_TYPE, SIGNON, BUF_TYPE, IS_FOR_SERVER >;
    using PBType_t = PROTO_TYPE;

    static constexpr int32_t             kMsgId       = ID;
    static constexpr SignonGroup_t       kSignonGroup = SIGNON;
    static constexpr NetChannelBufType_t kBufType     = BUF_TYPE;
    static constexpr bool                kIsForServer = IS_FOR_SERVER;

    static constexpr const char* GetGroupName()
    {
        return ( kSignonGroup >= 0 && kSignonGroup < SG_TOTAL )
            ? k_pszNetGroupNames[ kSignonGroup ] : "Unknown";
    }
};

template< int32_t ID, typename PROTO_TYPE, NetChannelBufType_t BUF_TYPE = BUF_RELIABLE >
class CUserMessagePB : public CNetMessagePBTyped< ID, PROTO_TYPE, SG_USERMSG, BUF_TYPE > {};

// Ids, signon groups, buffer types and the redispatch flag are the ones the
// engine instantiates its CNetMessagePB with (RTTI of libengine2.so). Messages
// it does not register are left out.

/* =========================
System messages
========================= */

class NetMessagePacketStart_t :                 public CNetMessagePBTyped< 0x40000000, NetMessagePacketStart, SG_SYSTEM > {};
class NetMessagePacketEnd_t :                   public CNetMessagePBTyped< 0x40000001, NetMessagePacketEnd, SG_SYSTEM > {};
class NetMessageConnectionClosed_t :            public CNetMessagePBTyped< -1, NetMessageConnectionClosed, SG_SYSTEM > {};
class NetMessageConnectionCrashed_t :           public CNetMessagePBTyped< -1, NetMessageConnectionCrashed, SG_SYSTEM > {};
class NetMessageSplitscreenUserChanged_t :      public CNetMessagePBTyped< -1, NetMessageSplitscreenUserChanged, SG_SYSTEM > {};

/* =========================
Engine (net_*)
========================= */

class CNETMsg_Tick_t :                          public CNetMessagePBTyped< net_Tick, CNETMsg_Tick, SG_ENGINE, BUF_UNRELIABLE > {};
class CNETMsg_StringCmd_t :                     public CNetMessagePBTyped< net_StringCmd, CNETMsg_StringCmd, SG_STRINGCMD > {};
class CNETMsg_SetConVar_t :                     public CNetMessagePBTyped< net_SetConVar, CNETMsg_SetConVar, SG_STRINGCMD > {};
class CNETMsg_SignonState_t :                   public CNetMessagePBTyped< net_SignonState, CNETMsg_SignonState, SG_SIGNON > {};
class CNETMsg_SpawnGroup_Load_t :               public CNetMessagePBTyped< net_SpawnGroup_Load, CNETMsg_SpawnGroup_Load, SG_SPAWNGROUPS > {};
class CNETMsg_SpawnGroup_ManifestUpdate_t :     public CNetMessagePBTyped< net_SpawnGroup_ManifestUpdate, CNETMsg_SpawnGroup_ManifestUpdate, SG_SPAWNGROUPS > {};
class CNETMsg_SpawnGroup_SetCreationTick_t :    public CNetMessagePBTyped< net_SpawnGroup_SetCreationTick, CNETMsg_SpawnGroup_SetCreationTick, SG_SPAWNGROUPS > {};
class CNETMsg_SpawnGroup_Unload_t :             public CNetMessagePBTyped< net_SpawnGroup_Unload, CNETMsg_SpawnGroup_Unload, SG_SPAWNGROUPS > {};
class CNETMsg_SpawnGroup_LoadCompleted_t :      public CNetMessagePBTyped< net_SpawnGroup_LoadCompleted, CNETMsg_SpawnGroup_LoadCompleted, SG_SPAWNGROUPS > {};
class CNETMsg_DebugOverlay_t :                  public CNetMessagePBTyped< net_DebugOverlay, CNETMsg_DebugOverlay, SG_ENGINE > {};

/* =========================
Bidirectional (bi_*)
========================= */

class CBidirMsg_RebroadcastGameEvent_t :        public CNetMessagePBTyped< bi_RebroadcastGameEvent, CBidirMsg_RebroadcastGameEvent, SG_EVENTS, BUF_RELIABLE, true > {};
class CBidirMsg_RebroadcastSource_t :           public CNetMessagePBTyped< bi_RebroadcastSource, CBidirMsg_RebroadcastSource, SG_EVENTS, BUF_RELIABLE, true > {};
class CBidirMsg_PredictionEvent_t :             public CNetMessagePBTyped< bi_PredictionEvent, CBidirMsg_PredictionEvent, SG_GENERIC, BUF_UNRELIABLE > {};

/* =========================
Client -> Server (clc_*)
========================= */

class CCLCMsg_ClientInfo_t :                    public CNetMessagePBTyped< clc_ClientInfo, CCLCMsg_ClientInfo, SG_SIGNON > {};
class CCLCMsg_Move_t :                          public CNetMessagePBTyped< clc_Move, CCLCMsg_Move, SG_MOVE, BUF_UNRELIABLE > {};
class CCLCMsg_VoiceData_t :                     public CNetMessagePBTyped< clc_VoiceData, CCLCMsg_VoiceData, SG_VOICE, BUF_UNRELIABLE > {};
class CCLCMsg_BaselineAck_t :                   public CNetMessagePBTyped< clc_BaselineAck, CCLCMsg_BaselineAck, SG_ENGINE > {};
class CCLCMsg_RespondCvarValue_t :              public CNetMessagePBTyped< clc_RespondCvarValue, CCLCMsg_RespondCvarValue > {};
class CCLCMsg_LoadingProgress_t :               public CNetMessagePBTyped< clc_LoadingProgress, CCLCMsg_LoadingProgress, SG_SIGNON > {};
class CCLCMsg_SplitPlayerConnect_t :            public CNetMessagePBTyped< clc_SplitPlayerConnect, CCLCMsg_SplitPlayerConnect, SG_SIGNON > {};
class CCLCMsg_SplitPlayerDisconnect_t :         public CNetMessagePBTyped< clc_SplitPlayerDisconnect, CCLCMsg_SplitPlayerDisconnect, SG_SIGNON > {};
class CCLCMsg_ServerStatus_t :                  public CNetMessagePBTyped< clc_ServerStatus, CCLCMsg_ServerStatus > {};
class CCLCMsg_RequestPause_t :                  public CNetMessagePBTyped< clc_RequestPause, CCLCMsg_RequestPause > {};
class CCLCMsg_CmdKeyValues_t :                  public CNetMessagePBTyped< clc_CmdKeyValues, CCLCMsg_CmdKeyValues, SG_STRINGCMD > {};
class CCLCMsg_RconServerDetails_t :             public CNetMessagePBTyped< clc_RconServerDetails, CCLCMsg_RconServerDetails > {};
class CCLCMsg_HltvReplay_t :                    public CNetMessagePBTyped< clc_HltvReplay, CCLCMsg_HltvReplay, SG_HLTVREPLAY > {};
class CCLCMsg_Diagnostic_t :                    public CNetMessagePBTyped< clc_Diagnostic, CCLCMsg_Diagnostic > {};
// 75 is svc_HltvFixupOperatorStatus in netmessages.proto
class CCLCMsg_HltvFixupOperatorTick_t :         public CNetMessagePBTyped< 75, CCLCMsg_HltvFixupOperatorTick, SG_HLTVREPLAY > {};

/* =========================
Server -> Client (svc_*)
========================= */

class CSVCMsg_ServerInfo_t :                    public CNetMessagePBTyped< svc_ServerInfo, CSVCMsg_ServerInfo, SG_SIGNON > {};
class CSVCMsg_FlattenedSerializer_t :           public CNetMessagePBTyped< svc_FlattenedSerializer, CSVCMsg_FlattenedSerializer, SG_SIGNON > {};
class CSVCMsg_ClassInfo_t :                     public CNetMessagePBTyped< svc_ClassInfo, CSVCMsg_ClassInfo, SG_SIGNON > {};
class CSVCMsg_SetPause_t :                      public CNetMessagePBTyped< svc_SetPause, CSVCMsg_SetPause > {};
class CSVCMsg_CreateStringTable_t :             public CNetMessagePBTyped< svc_CreateStringTable, CSVCMsg_CreateStringTable, SG_SIGNON > {};
class CSVCMsg_UpdateStringTable_t :             public CNetMessagePBTyped< svc_UpdateStringTable, CSVCMsg_UpdateStringTable, SG_STRINGTABLE > {};
class CSVCMsg_VoiceInit_t :                     public CNetMessagePBTyped< svc_VoiceInit, CSVCMsg_VoiceInit, SG_SIGNON > {};
class CSVCMsg_VoiceData_t :                     public CNetMessagePBTyped< svc_VoiceData, CSVCMsg_VoiceData, SG_VOICE, BUF_UNRELIABLE > {};
class CSVCMsg_Print_t :                         public CNetMessagePBTyped< svc_Print, CSVCMsg_Print > {};
class CSVCMsg_Sounds_t :                        public CNetMessagePBTyped< svc_Sounds, CSVCMsg_Sounds, SG_SOUNDS > {};
class CSVCMsg_SetView_t :                       public CNetMessagePBTyped< svc_SetView, CSVCMsg_SetView > {};
class CSVCMsg_ClearAllStringTables_t :          public CNetMessagePBTyped< svc_ClearAllStringTables, CSVCMsg_ClearAllStringTables, SG_SIGNON > {};
class CSVCMsg_CmdKeyValues_t :                  public CNetMessagePBTyped< svc_CmdKeyValues, CSVCMsg_CmdKeyValues, SG_STRINGCMD > {};
class CSVCMsg_SplitScreen_t :                   public CNetMessagePBTyped< svc_SplitScreen, CSVCMsg_SplitScreen, SG_SIGNON > {};
class CSVCMsg_PacketEntities_t :                public CNetMessagePBTyped< svc_PacketEntities, CSVCMsg_PacketEntities, SG_INVALID > {};
class CSVCMsg_Prefetch_t :                      public CNetMessagePBTyped< svc_Prefetch, CSVCMsg_Prefetch, SG_SOUNDS > {};
class CSVCMsg_Menu_t :                          public CNetMessagePBTyped< svc_Menu, CSVCMsg_Menu > {};
class CSVCMsg_GetCvarValue_t :                  public CNetMessagePBTyped< svc_GetCvarValue, CSVCMsg_GetCvarValue > {};
class CSVCMsg_StopSound_t :                     public CNetMessagePBTyped< svc_StopSound, CSVCMsg_StopSound, SG_SOUNDS > {};
class CSVCMsg_PeerList_t :                      public CNetMessagePBTyped< svc_PeerList, CSVCMsg_PeerList > {};
class CSVCMsg_PacketReliable_t :                public CNetMessagePBTyped< svc_PacketReliable, CSVCMsg_PacketReliable > {};
class CSVCMsg_HLTVStatus_t :                    public CNetMessagePBTyped< svc_HLTVStatus, CSVCMsg_HLTVStatus, SG_GENERIC, BUF_RELIABLE, true > {};
class CSVCMsg_ServerSteamID_t :                 public CNetMessagePBTyped< svc_ServerSteamID, CSVCMsg_ServerSteamID, SG_GENERIC, BUF_RELIABLE, true > {};
class CSVCMsg_FullFrameSplit_t :                public CNetMessagePBTyped< svc_FullFrameSplit, CSVCMsg_FullFrameSplit > {};
class CSVCMsg_RconServerDetails_t :             public CNetMessagePBTyped< svc_RconServerDetails, CSVCMsg_RconServerDetails > {};
class CSVCMsg_UserMessage_t :                   public CNetMessagePBTyped< svc_UserMessage, CSVCMsg_UserMessage, SG_USERMSG > {};
// 74 is svc_Broadcast_Command in netmessages.proto
class CSVCMsg_HltvReplay_t :                    public CNetMessagePBTyped< 74, CSVCMsg_HltvReplay, SG_HLTVREPLAY > {};
class CSVCMsg_UserCommands_t :                  public CNetMessagePBTyped< svc_UserCmds, CSVCMsg_UserCommands, SG_HLTVREPLAY > {};
class CSVCMsg_NextMsgPredicted_t :              public CNetMessagePBTyped< svc_NextMsgPredicted, CSVCMsg_NextMsgPredicted, SG_HLTVREPLAY > {};
class CSVCMsg_EncryptedData_t :                 public CNetMessagePBTyped< svc_EncryptedData, CSVCMsg_EncryptedData > {};

#endif // SCHEMA_NETMESSAGES_H
