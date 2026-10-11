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

#ifndef _INCLUDE_PULSEVALUETYPE_T_H
#define _INCLUDE_PULSEVALUETYPE_T_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class PulseValueType_t : int32_t
{
    PVAL_VOID = -1,
    PVAL_BOOL = 0,
    PVAL_INT = 1,
    PVAL_FLOAT = 2,
    PVAL_STRING = 3,
    PVAL_VEC2 = 4,
    PVAL_VEC3 = 5,
    PVAL_QANGLE = 6,
    PVAL_VEC3_WORLDSPACE = 7,
    PVAL_VEC4 = 8,
    PVAL_TRANSFORM = 9,
    PVAL_TRANSFORM_WORLDSPACE = 10,
    PVAL_COLOR_RGB = 11,
    PVAL_GAMETIME = 12,
    PVAL_EHANDLE = 13,
    PVAL_RESOURCE = 14,
    PVAL_RESOURCE_NAME = 15,
    PVAL_SNDEVT_GUID = 16,
    PVAL_SNDEVT_NAME = 17,
    PVAL_ENTITY_NAME = 18,
    PVAL_OPAQUE_HANDLE = 19,
    PVAL_TYPESAFE_INT = 20,
    PVAL_MODEL_MATERIAL_GROUP = 21,
    PVAL_CURSOR_FLOW = 22,
    PVAL_VARIANT = 23,
    PVAL_UNKNOWN = 24,
    PVAL_SCHEMA_ENUM = 25,
    PVAL_PANORAMA_PANEL_HANDLE = 26,
    PVAL_TEST_HANDLE = 27,
    PVAL_ARRAY = 28,
    PVAL_TYPESAFE_INT64 = 29,
    PVAL_PARTICLE_EHANDLE = 30,
    PVAL_ANIM_SEQUENCE = 31,
    PVAL_VDATA_CHOICE = 32,
    PVAL_COUNT = 33,
};

#endif // _INCLUDE_PULSEVALUETYPE_T_H
