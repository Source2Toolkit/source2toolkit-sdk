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

#include "source2toolkit/schema/entity/classes/CPlayer_WeaponServices.h"

#include "source2toolkit/IToolkitAddresses.h"
#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitGameConfig.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/utils/virtual.h"
TOOLKIT_GLOBALVARS();

// The gamedata keys still name CCSPlayer_WeaponServices: that is the class the
// vtable indices and the Destroy signature were read off. Every weapon services
// object the game hands out is one, so the calls are valid through the base
// pointer CBasePlayerPawn stores.

bool CPlayer_WeaponServices::CanUse(CBasePlayerWeapon* pWeapon)
{
    static int offset = g_pToolkitGameConfig->GetOffset("CCSPlayer_WeaponServices::CanUse");
    return CALL_VIRTUAL(bool, offset, this, pWeapon);
}

void CPlayer_WeaponServices::DropWeapon(CBasePlayerWeapon *pWeapon, Vector *pVecTarget, Vector *pVelocity, HookChain eChain)
{
    static int offset = g_pToolkitGameConfig->GetOffset("CCSPlayer_WeaponServices::DropWeapon");
    CALL_VIRTUAL_CHAIN(void, offset, eChain, this, pWeapon, pVecTarget, pVelocity);
}

int CPlayer_WeaponServices::BumpWeapon(CBasePlayerWeapon* pWeapon, HookChain eChain)
{
    static int offset = g_pToolkitGameConfig->GetOffset("CCSPlayer_WeaponServices::BumpWeapon");
    return CALL_VIRTUAL_CHAIN(int, offset, eChain, this, pWeapon);
}

void CPlayer_WeaponServices::SelectItem(CBasePlayerWeapon* pWeapon, int unk1, HookChain eChain)
{
    static int offset = g_pToolkitGameConfig->GetOffset("CCSPlayer_WeaponServices::SelectItem");
    CALL_VIRTUAL_CHAIN(void, offset, eChain, this, pWeapon, unk1);
}

void CPlayer_WeaponServices::Destroy(CBasePlayerWeapon* pWeapon, HookChain eChain)
{
#ifndef _WIN32
    if(pWeapon)
#endif
    {
        ResolveHookChain(ADDR_WEAPON_SERVICES_DESTROY(), eChain)(this, pWeapon);
    }
}
