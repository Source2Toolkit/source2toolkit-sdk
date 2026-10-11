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

#include "source2toolkit/schema/entity/classes/CBasePlayerController.h"

#include "source2toolkit/IToolkitAddresses.h"
#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitGameConfig.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entity/classes/CCSPlayerPawn.h"

#include <engine2/serversideclient.h>
#include <tier0/dbg.h>
TOOLKIT_GLOBALVARS();

#include "engine2/networkgameserverbase.h"
#include "engine2/inetworkserverservice.h"
#include "networksystem/inetworkmessages.h"
#include "usermessages.pb.h"

enum class HudDestination
{
    Notify = 1,
    Console = 2,
    Chat = 3,
    Center = 4,
    Alert = 6
};

static void ClientPrint(int slot, int hudDestination, const char* message)
{
    INetworkMessageInternal* pNetMsg = GetNetworkMessages()->FindNetworkMessagePartial("TextMsg");
    auto data = pNetMsg->AllocateMessage()->ToPB<CUserMessageTextMsg>();

    data->set_dest(hudDestination);
    data->add_param(message);

    CPlayerBitVec recipients;
    recipients.Set(slot);

    GetGameEventSystem()->PostEventAbstract(CSplitScreenSlot(-1), false, ABSOLUTE_PLAYER_LIMIT,
                                                reinterpret_cast<const uint64*>(recipients.Base()), pNetMsg, data, 0,
                                                NetChannelBufType_t::BUF_RELIABLE);

    delete data;
}

void CBasePlayerController::SetPawn(CBasePlayerPawn* pPawn, HookChain eChain){
    ResolveHookChain(ADDR_SET_PAWN(), eChain)(this, pPawn, true, false, false, false);
}

void CBasePlayerController::PrintToConsole(const char* pszMessage)
{
    std::string pszSanitizedMessage;
    pszSanitizedMessage.reserve(strlen(pszMessage) + 2);
    pszSanitizedMessage = pszMessage;
    pszSanitizedMessage += '\n';
    ClientPrint(GetSlot(), (int)HudDestination::Console, pszSanitizedMessage.c_str());
}

void CBasePlayerController::PrintToChat(const char* pszMessage)
{
    ClientPrint(GetSlot(), (int)HudDestination::Chat, pszMessage);
}

void CBasePlayerController::PrintToCenter(const char* pszMessage)
{
    ClientPrint(GetSlot(), (int)HudDestination::Center, pszMessage);
}

void CBasePlayerController::PrintToCenterAlert(const char* pszMessage)
{
    ClientPrint(GetSlot(), (int)HudDestination::Alert, pszMessage);
}

bool CBasePlayerController::IsBot()
{
    return (m_fFlags & FL_FAKECLIENT) != 0;
}

void CBasePlayerController::Disconnect(ENetworkDisconnectionReason eReason, const char* pszInternalReason)
{
    if (m_iConnected() != PlayerConnectedState::Connected)
        return;

    // The disconnect happens next frame, by which time a reason built on the
    // spot -- fmt::format(...).c_str() -- is gone. The lambda keeps its own copy.
    const bool bHasReason = pszInternalReason != nullptr;
    std::string sInternalReason = bHasReason ? pszInternalReason : "";

    CLOSE_ACTIVE_MENU(reinterpret_cast<CCSPlayerController*>(this));
    g_pToolkitScheduler->NextFrame([hPlayer = this->GetHandle(), eReason, bHasReason, sInternalReason = std::move(sInternalReason)]
    {
        if (!hPlayer || hPlayer->m_iConnected() != PlayerConnectedState::Connected)
            return;

        GetEngineServer()->DisconnectClient(hPlayer->GetSlot(), eReason, bHasReason ? sInternalReason.c_str() : nullptr);
    });
}

void CBasePlayerController::ExecuteClientCommand(const char* pszCommand)
{
    GetEngineServer()->ClientCommand(GetPlayerSlot(), "%s", pszCommand);
}

void CBasePlayerController::ExecuteClientCommandFromServer(const char* pszCommand)
{
    CCommand args;
    args.Tokenize(pszCommand);

    auto handle = GetCVar()->FindConCommand(args.Arg(0));
    if (!handle.IsValidRef()) return;

    CCommandContext context(CommandTarget_t::CT_NO_TARGET, GetPlayerSlot());

    GetCVar()->DispatchConCommand(handle, context, args);
}

CServerSideClient* CBasePlayerController::GetServerSideClient()
{
    CNetworkGameServerBase* pServer = GetNetworkServerService()->GetIGameServer();
    if (!pServer)
        return nullptr;

    static int offset = g_pToolkitGameConfig->GetOffset("CNetworkGameServer::ClientList");

    auto* pClients = reinterpret_cast<CUtlVector<CServerSideClient*>*>(reinterpret_cast<uint8_t*>(pServer) + offset);

    const int index = GetSlot();
    if (index < 0 || index >= pClients->Count())
        return nullptr;

    return pClients->Element(index);
}

CEntityIndex CBasePlayerController::GetPlayerIndex()
{
    return GetEntityIndex();
}

int CBasePlayerController::GetSlot()
{
    return GetIndex() - 1;
}

CPlayerSlot CBasePlayerController::GetPlayerSlot()
{
    return CPlayerSlot(GetSlot());
}

int CBasePlayerController::GetUserID()
{
    return GetEngineServer()->GetPlayerUserId(GetPlayerSlot()).Get();
}

CPlayerUserId CBasePlayerController::GetPlayerUserID()
{
    return GetEngineServer()->GetPlayerUserId(GetPlayerSlot());
}

uint64 CBasePlayerController::GetSteamID()
{
    return m_steamID();
}

CSteamID CBasePlayerController::GetPlayerSteamID()
{
    return CSteamID(static_cast<uint64>(m_steamID()));
}

const char* CBasePlayerController::GetPlayerName()
{
    return m_iszPlayerName();
}

CUtlString CBasePlayerController::GetIpAddress()
{
    if (const auto* netInfo = GetEngineServer()->GetPlayerNetInfo(GetPlayerSlot()))
    {
        uint32_t ip = netInfo->GetRemoteAddress().GetIP();

        ip = ((ip & 0x000000FF) << 24) |
             ((ip & 0x0000FF00) << 8)  |
             ((ip & 0x00FF0000) >> 8)  |
             ((ip & 0xFF000000) >> 24);

        char buffer[64];

        std::snprintf(
            buffer,
            sizeof(buffer),
            "%u.%u.%u.%u",
            (ip >> 24) & 0xFF,
            (ip >> 16) & 0xFF,
            (ip >> 8) & 0xFF,
            ip & 0xFF
        );

        return CUtlString(buffer);
    }

    return CUtlString();
}

void CBasePlayerController::ReplicateConVar(const char* pszConVar, const char* pszValue)
{
    INetChannel* pNetChannel = reinterpret_cast<INetChannel*>(GetEngineServer()->GetPlayerNetInfo(GetPlayerSlot()));
    if (pNetChannel)
    {
        static INetworkMessageInternal* pMsg = GetNetworkMessages()->FindNetworkMessagePartial("CNETMsg_SetConVar");

        CNetMessagePB<CNETMsg_SetConVar>* msg = pMsg->AllocateMessage()->ToPB<CNETMsg_SetConVar>();
        auto cvar = msg->mutable_convars()->add_cvars();
        cvar->set_name(pszConVar);
        cvar->set_value(pszValue);

        pNetChannel->SendNetMessage(msg, BUF_DEFAULT);

        delete msg;
    }
}

void CBasePlayerController::FireEventToClient(IGameEvent* pEvent)
{
    if (!pEvent) return;

    IGameEventListener2* pListener = ADDR_LEGACY_GAME_EVENT_LISTENER()(GetPlayerSlot());
    if (!pListener) return;

    pListener->FireGameEvent(pEvent);
}

// m_hPawn is typed CHandle<CBasePlayerPawn>, but every pawn this game puts on a
// controller is a CCSPlayerPawn -- the downcast is what callers would otherwise
// have to write at each use.
CCSPlayerPawn* CBasePlayerController::GetPawn()
{
    if (auto handle = m_hPawn(); handle.IsValid())
        return static_cast<CCSPlayerPawn*>(handle.Get());
    return nullptr;
}
