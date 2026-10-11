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

#ifndef _INCLUDE_NAVATTRIBUTEDYNAMICTYPE_H
#define _INCLUDE_NAVATTRIBUTEDYNAMICTYPE_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class NavAttributeDynamicType : uint32_t
{
    NAV_AREA_NONE = 0,
    NAV_AREA_UNDER_WATER = 1,
    NAV_AREA_UNDER_WATER_DEEP = 2,
    NAV_AREA_EXTERNALLY_CREATED = 4,
    NAV_AREA_SHOULD_BE_DESTROYED = 8,
    NAV_AREA_CREATED_BY_OBSTACLE_MGR = 16,
    NAV_AREA_SPLIT_BY_OBSTACLE_MGR = 32,
    NAV_AREA_SPLIT_OBS_CONTAINED = 64,
    NAV_AREA_SPLIT_OBS_BASE_CONTAINED = 128,
    NAV_AREA_HAS_LADDERS = 256,
    NAV_AREA_NAV_LINK = 512,
    NAV_AREA_NAV_LINK_TERMINUS = 1024,
    NAV_AREA_CONNECTED_TO_NAV_LINK_OUT = 2048,
    NAV_AREA_CONNECTED_TO_NAV_LINK_IN = 4096,
    NAV_AREA_MOVABLE = 8192,
    NAV_AREA_DOCK = 16384,
    NAV_AREA_DOCKING_CANDIDATE = 32768,
    NAV_AREA_BOUNDARY = 65536,
    NAV_AREA_HAS_TACTICAL_SEARCH_ANNOTATIONS = 131072,
    NAV_AREA_DEFORMABLE = 262144,
    NAV_AREA_DEFORMABLE_DOCK = 524288,
    NAV_AREA_LINK_AUTO_ADJUST = 1048576,
};

#endif // _INCLUDE_NAVATTRIBUTEDYNAMICTYPE_H
