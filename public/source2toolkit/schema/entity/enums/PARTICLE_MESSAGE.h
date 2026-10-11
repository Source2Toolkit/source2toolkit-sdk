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

#ifndef _INCLUDE_PARTICLE_MESSAGE_H
#define _INCLUDE_PARTICLE_MESSAGE_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class PARTICLE_MESSAGE : uint32_t
{
    GAME_PARTICLE_MANAGER_EVENT_CREATE = 0,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE = 1,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE_FORWARD = 2,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE_ORIENTATION = 3,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE_FALLBACK = 4,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENT = 5,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE_OFFSET = 6,
    GAME_PARTICLE_MANAGER_EVENT_DESTROY = 7,
    GAME_PARTICLE_MANAGER_EVENT_DESTROY_INVOLVING = 8,
    GAME_PARTICLE_MANAGER_EVENT_RELEASE = 9,
    GAME_PARTICLE_MANAGER_EVENT_LATENCY = 10,
    GAME_PARTICLE_MANAGER_EVENT_SHOULD_DRAW = 11,
    GAME_PARTICLE_MANAGER_EVENT_FROZEN = 12,
    GAME_PARTICLE_MANAGER_EVENT_CHANGE_CONTROL_POINT_ATTACHMENT = 13,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE_ENTITY_POSITION = 14,
    GAME_PARTICLE_MANAGER_EVENT_SET_FOW_PROPERTIES = 15,
    GAME_PARTICLE_MANAGER_EVENT_SET_TEXT = 16,
    GAME_PARTICLE_MANAGER_EVENT_SET_SHOULD_CHECK_FOW = 17,
    GAME_PARTICLE_MANAGER_EVENT_SET_CONTROL_POINT_MODEL = 18,
    GAME_PARTICLE_MANAGER_EVENT_SET_CONTROL_POINT_SNAPSHOT = 19,
    GAME_PARTICLE_MANAGER_EVENT_SET_TEXTURE_ATTRIBUTE = 20,
    GAME_PARTICLE_MANAGER_EVENT_SET_SCENE_OBJECT_GENERIC_FLAG = 21,
    GAME_PARTICLE_MANAGER_EVENT_SET_SCENE_OBJECT_TINT_AND_DESAT = 22,
    GAME_PARTICLE_MANAGER_EVENT_DESTROY_NAMED = 23,
    GAME_PARTICLE_MANAGER_EVENT_SKIP_TO_TIME = 24,
    GAME_PARTICLE_MANAGER_EVENT_CAN_FREEZE = 25,
    GAME_PARTICLE_MANAGER_EVENT_SET_NAMED_VALUE_CONTEXT = 26,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE_TRANSFORM = 27,
    GAME_PARTICLE_MANAGER_EVENT_FREEZE_TRANSITION_OVERRIDE = 28,
    GAME_PARTICLE_MANAGER_EVENT_FREEZE_INVOLVING = 29,
    GAME_PARTICLE_MANAGER_EVENT_ADD_MODELLIST_OVERRIDE_ELEMENT = 30,
    GAME_PARTICLE_MANAGER_EVENT_CLEAR_MODELLIST_OVERRIDE = 31,
    GAME_PARTICLE_MANAGER_EVENT_CREATE_PHYSICS_SIM = 32,
    GAME_PARTICLE_MANAGER_EVENT_DESTROY_PHYSICS_SIM = 33,
    GAME_PARTICLE_MANAGER_EVENT_SET_VDATA = 34,
    GAME_PARTICLE_MANAGER_EVENT_SET_MATERIAL_OVERRIDE = 35,
    GAME_PARTICLE_MANAGER_EVENT_ADD_FAN = 36,
    GAME_PARTICLE_MANAGER_EVENT_UPDATE_FAN = 37,
    GAME_PARTICLE_MANAGER_EVENT_SET_CLUSTER_GROWTH = 38,
    GAME_PARTICLE_MANAGER_EVENT_REMOVE_FAN = 39,
    GAME_PARTICLE_MANAGER_EVENT_CREATE_SMOKE_GRID = 40,
    GAME_PARTICLE_MANAGER_EVENT_SET_OVERRIDE_TEXTURE = 41,
};

#endif // _INCLUDE_PARTICLE_MESSAGE_H
