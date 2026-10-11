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

#ifndef _INCLUDE_PARTICLEVECTYPE_T_H
#define _INCLUDE_PARTICLEVECTYPE_T_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class ParticleVecType_t : int32_t
{
    PVEC_TYPE_INVALID = -1,
    PVEC_TYPE_LITERAL = 0,
    PVEC_TYPE_LITERAL_COLOR = 1,
    PVEC_TYPE_NAMED_VALUE = 2,
    PVEC_TYPE_PARTICLE_VECTOR = 3,
    PVEC_TYPE_PARTICLE_INITIAL_VECTOR = 4,
    PVEC_TYPE_PARTICLE_VELOCITY = 5,
    PVEC_TYPE_PARTICLE_GRAVITY = 6,
    PVEC_TYPE_CP_VALUE = 7,
    PVEC_TYPE_CP_RELATIVE_POSITION = 8,
    PVEC_TYPE_CP_RELATIVE_DIR = 9,
    PVEC_TYPE_CP_RELATIVE_RANDOM_DIR = 10,
    PVEC_TYPE_FLOAT_COMPONENTS = 11,
    PVEC_TYPE_FLOAT_INTERP_CLAMPED = 12,
    PVEC_TYPE_FLOAT_INTERP_OPEN = 13,
    PVEC_TYPE_FLOAT_INTERP_GRADIENT = 14,
    PVEC_TYPE_RANDOM_UNIFORM = 15,
    PVEC_TYPE_RANDOM_UNIFORM_OFFSET = 16,
    PVEC_TYPE_CP_DELTA = 17,
    PVEC_TYPE_CLOSEST_CAMERA_POSITION = 18,
    PVEC_TYPE_COUNT = 19,
};

#endif // _INCLUDE_PARTICLEVECTYPE_T_H
