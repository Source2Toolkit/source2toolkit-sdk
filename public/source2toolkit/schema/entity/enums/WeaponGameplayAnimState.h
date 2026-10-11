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

#ifndef _INCLUDE_WEAPONGAMEPLAYANIMSTATE_H
#define _INCLUDE_WEAPONGAMEPLAYANIMSTATE_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class WeaponGameplayAnimState : uint16_t
{
    WPN_ANIMSTATE_UNINITIALIZED = 0,
    WPN_ANIMSTATE_DROPPED = 1,
    WPN_ANIMSTATE_HOLSTERED = 10,
    WPN_ANIMSTATE_DEPLOY = 11,
    WPN_ANIMSTATE_IDLE = 50,
    WPN_ANIMSTATE_SHOOT_PRIMARY = 100,
    WPN_ANIMSTATE_SHOOT_SECONDARY = 101,
    WPN_ANIMSTATE_SHOOT_DRYFIRE = 102,
    WPN_ANIMSTATE_CHARGE = 103,
    WPN_ANIMSTATE_GRENADE_PULL_PIN = 200,
    WPN_ANIMSTATE_GRENADE_READY = 201,
    WPN_ANIMSTATE_GRENADE_THROW = 202,
    WPN_ANIMSTATE_C4_PLANT = 300,
    WPN_ANIMSTATE_HEALTHSHOT_INJECT = 400,
    WPN_ANIMSTATE_KNIFE_PRIMARY_HIT = 500,
    WPN_ANIMSTATE_KNIFE_PRIMARY_MISS = 501,
    WPN_ANIMSTATE_KNIFE_SECONDARY_HIT = 502,
    WPN_ANIMSTATE_KNIFE_SECONDARY_MISS = 503,
    WPN_ANIMSTATE_KNIFE_PRIMARY_STAB = 504,
    WPN_ANIMSTATE_KNIFE_SECONDARY_STAB = 505,
    WPN_ANIMSTATE_SILENCER_APPLY = 600,
    WPN_ANIMSTATE_SILENCER_REMOVE = 601,
    WPN_ANIMSTATE_RELOAD = 800,
    WPN_ANIMSTATE_RELOAD_OUTRO = 801,
    WPN_ANIMSTATE_INSPECT = 1000,
    WPN_ANIMSTATE_INSPECT_OUTRO = 1001,
    WPN_ANIMSTATE_INVENTORY_UI_TUMBLE = 1500,
    WPN_ANIMSTATE_INVENTORY_UI_KEYCHAIN_APPLY = 1501,
    WPN_ANIMSTATE_END_VALID = 2000,
};

#endif // _INCLUDE_WEAPONGAMEPLAYANIMSTATE_H
