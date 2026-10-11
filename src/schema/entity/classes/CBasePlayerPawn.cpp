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

#include "source2toolkit/schema/entity/classes/CBasePlayerPawn.h"

#include "source2toolkit/IToolkitAddresses.h"
#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitGameConfig.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entity/classes/CCSPlayerController.h"
#include "source2toolkit/utils/virtual.h"
TOOLKIT_GLOBALVARS();

void CBasePlayerPawn::CommitSuicide(bool bExplode, bool bForce, HookChain eChain) {
    static int offset = g_pToolkitGameConfig->GetOffset("CBasePlayerPawn::CommitSuicide");
    CALL_VIRTUAL_CHAIN(void, offset, eChain, this, bExplode, bForce);
}

void CBasePlayerPawn::SnapViewAngles(const QAngle& angEyeAngles, HookChain eChain) {
    const auto fn = ResolveHookChain(ADDR_SNAP_VIEW_ANGLES(), eChain);
    // Only resolved where a signature exists for the platform -- currently Linux.
    if (!fn) return;

    // The engine takes the angles by pointer and does not modify them.
    QAngle ang = angEyeAngles;
    fn(this, &ang);
}

// m_hController is typed CHandle<CBasePlayerController>, but every controller
// this game creates is a CCSPlayerController -- the downcast is what callers
// would otherwise have to write at each use.
CCSPlayerController* CBasePlayerPawn::GetController()
{
    if (auto handle = m_hController(); handle.IsValid())
        return static_cast<CCSPlayerController*>(handle.Get());
    return nullptr;
}

CCSPlayerController* CBasePlayerPawn::GetDefaultController()
{
    if (auto handle = m_hDefaultController(); handle.IsValid())
        return static_cast<CCSPlayerController*>(handle.Get());
    return nullptr;
}
