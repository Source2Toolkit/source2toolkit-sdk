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

#include "source2toolkit/schema/entity/classes/CCSGameRules.h"

#include "source2toolkit/IToolkitAddresses.h"
#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitEntities.h"
#include "source2toolkit/IToolkitGameConfig.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entity/classes/CCSPlayerController.h"
#include "source2toolkit/schema/entity/classes/CCSPlayerPawn.h"
#include "source2toolkit/utils/virtual.h"
TOOLKIT_GLOBALVARS();

void CCSGameRules::TerminateRound(float flDelay, int32_t eRoundEndReason, uint32 nTeamId, HookChain eChain)
{
    auto pfnTerminateRound = ADDR_TERMINATE_ROUND();
    if (!pfnTerminateRound)
        return;

    // The team is taken by pointer, and null is what "no winning team" looks
    // like -- passing a pointer to zero is not the same thing.
    uint32 nTeam = nTeamId;
    uint32* pnTeamId = nTeamId > 0 ? &nTeam : nullptr;

    // Bypass by default, for the same reason as CCSPlayerController::TakeDamage:
    // ending the round because the server was told to is not the game ending
    // the round, and a gamemode's TerminateRound hook exists to police the
    // latter. HookChain::Run lets those hooks see (and veto) this one too.
    //
    // See the note on CCSGameRules_TerminateRound_t: the argument order is not
    // the same on both platforms.
    auto pfnOriginal = ResolveHookChain(pfnTerminateRound, eChain);
#ifdef _WIN32
    pfnOriginal(this, flDelay, static_cast<uint32>(eRoundEndReason), pnTeamId);
#else
    pfnOriginal(this, static_cast<uint32>(eRoundEndReason), pnTeamId, flDelay);
#endif
}

CBaseEntity* CCSGameRules::FindPickerEntity(CBasePlayerController* pPlayer)
{
    return g_pToolkitEntities->FindPickerEntity(pPlayer, this);
}

CCSPlayerController* CCSGameRules::GetClientAimTarget(CCSPlayerController* pPlayer)
{
    auto* pPawn = static_cast<CCSPlayerPawn*>(FindPickerEntity(pPlayer));
    if (!pPawn) return nullptr;

    return V_strcmp(pPawn->GetClassname(), "player") == 0 ? pPawn->m_hOriginalController().Get() : nullptr;
}

void CCSGameRules::GoToIntermission(bool bAbortedMatch, HookChain eChain)
{
    static int offset = g_pToolkitGameConfig->GetOffset("CCSGameRules::GoToIntermission");
    CALL_VIRTUAL_CHAIN(void, offset, eChain, this, bAbortedMatch);
}
