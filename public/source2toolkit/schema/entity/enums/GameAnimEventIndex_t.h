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

#ifndef _INCLUDE_GAMEANIMEVENTINDEX_T_H
#define _INCLUDE_GAMEANIMEVENTINDEX_T_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class GameAnimEventIndex_t : uint32_t
{
    AE_EMPTY = 0,
    AE_CL_PLAYSOUND = 1,
    AE_CL_PLAYSOUND_ATTACHMENT = 2,
    AE_CL_PLAYSOUND_POSITION = 3,
    AE_SV_PLAYSOUND = 4,
    AE_CL_STOPSOUND = 5,
    AE_CL_PLAYSOUND_LOOPING = 6,
    AE_CL_CREATE_PARTICLE_EFFECT = 7,
    AE_CL_STOP_PARTICLE_EFFECT = 8,
    AE_CL_CREATE_PARTICLE_EFFECT_CFG = 9,
    AE_SV_CREATE_PARTICLE_EFFECT_CFG = 10,
    AE_SV_STOP_PARTICLE_EFFECT = 11,
    AE_FOOTSTEP = 12,
    AE_CL_STOP_RAGDOLL_CONTROL = 13,
    AE_BODYGROUP_SET_VALUE = 14,
    AE_WEAPON_PERFORM_ATTACK = 15,
    AE_FIRE_INPUT = 16,
    AE_CL_CLOTH_ATTR = 17,
    AE_CL_CLOTH_GROUND_OFFSET = 18,
    AE_CL_CLOTH_STIFFEN = 19,
    AE_CL_CLOTH_EFFECT = 20,
    AE_CL_CREATE_ANIM_SCOPE_PROP = 21,
    AE_SV_IKLOCK = 22,
    AE_PULSE_GRAPH = 23,
    AE_DISABLE_PLATFORM = 24,
    AE_ENABLE_PLATFORM_PLAYER_FOLLOWS_YAW = 25,
    AE_ENABLE_PLATFORM_PLAYER_IGNORES_YAW = 26,
    AE_DESTRUCTIBLE_PART_DESTROY = 27,
    AE_VEHICLE_ENTER_FINISHED = 28,
    AE_VEHICLE_EXIT_FINISHED = 29,
    AE_SCRIPT_FIRE_EVENT_01 = 30,
    AE_SCRIPT_FIRE_EVENT_02 = 31,
    AE_SCRIPT_FIRE_EVENT_03 = 32,
    AE_SCRIPT_FIRE_EVENT_04 = 33,
    AE_SCRIPT_FIRE_EVENT_05 = 34,
    AE_SCRIPT_FIRE_EVENT_06 = 35,
    AE_SCRIPT_FIRE_EVENT_07 = 36,
    AE_SCRIPT_FIRE_EVENT_08 = 37,
    AE_SCRIPT_FIRE_EVENT_09 = 38,
    AE_SCRIPT_FIRE_EVENT_10 = 39,
    AE_CL_WEAPON_TRANSITION_INTO_HAND = 40,
    AE_SV_ATTACH_SILENCER_COMPLETE = 41,
    AE_SV_DETACH_SILENCER_COMPLETE = 42,
    AE_CL_EJECT_MAG = 43,
    AE_WPN_COMPLETE_RELOAD = 44,
    AE_WPN_HEALTHSHOT_INJECT = 45,
    AE_GRENADE_THROW_COMPLETE = 46,
    AE_COUNT = 47,
};

#endif // _INCLUDE_GAMEANIMEVENTINDEX_T_H
