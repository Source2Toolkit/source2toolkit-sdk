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

#ifndef _INCLUDE_PERMMODELINFO_T__FLAGENUM_H
#define _INCLUDE_PERMMODELINFO_T__FLAGENUM_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class PermModelInfo_t__FlagEnum : uint32_t
{
    FLAG_TRANSLUCENT = 1,
    FLAG_TRANSLUCENT_TWO_PASS = 2,
    FLAG_MODEL_IS_RUNTIME_COMBINED = 4,
    FLAG_SOURCE1_IMPORT = 8,
    FLAG_MODEL_PART_CHILD = 16,
    FLAG_NAV_GEN_NONE = 32,
    FLAG_NAV_GEN_HULL = 64,
    FLAG_NO_FORCED_FADE = 2048,
    FLAG_HAS_SKINNED_MESHES = 1024,
    FLAG_DO_NOT_CAST_SHADOWS = 131072,
    FLAG_FORCE_PHONEME_CROSSFADE = 4096,
    FLAG_NO_ANIM_EVENTS = 1048576,
    FLAG_ANIMATION_DRIVEN_FLEXES = 2097152,
    FLAG_IMPLICIT_BIND_POSE_SEQUENCE = 4194304,
    FLAG_MODEL_DOC = 8388608,
};

#endif // _INCLUDE_PERMMODELINFO_T__FLAGENUM_H
