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

#ifndef _INCLUDE_EWEAPONTYPE_H
#define _INCLUDE_EWEAPONTYPE_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class EWeaponType : uint32_t
{
    EWT_Knife = 0,
    EWT_Pistol = 1,
    EWT_SubMachineGun = 2,
    EWT_Rifle = 3,
    EWT_Shotgun = 4,
    EWT_SniperRifle = 5,
    EWT_MachineGun = 6,
    EWT_C4 = 7,
    EWT_Grenade = 8,
    EWT_Equipment = 9,
    EWT_StackableItem = 10,
    EWT_Unknown = 11,
};

#endif // _INCLUDE_EWEAPONTYPE_H
