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

#ifndef _INCLUDE_ESOURCE2PLAYSTATSFIELDTYPE_H
#define _INCLUDE_ESOURCE2PLAYSTATSFIELDTYPE_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class ESource2PlayStatsFieldType : uint32_t
{
    Source2PlayStats_Invalid = 0,
    Source2PlayStats_UInt64 = 1,
    Source2PlayStats_UInt32 = 2,
    Source2PlayStats_UInt16 = 3,
    Source2PlayStats_UInt8 = 4,
    Source2PlayStats_Int64 = 5,
    Source2PlayStats_Int32 = 6,
    Source2PlayStats_Int16 = 7,
    Source2PlayStats_Int8 = 8,
    Source2PlayStats_Float64 = 9,
    Source2PlayStats_Float32 = 10,
    Source2PlayStats_Bool = 11,
    Source2PlayStats_String = 12,
    Source2PlayStats_LowCardinalityString = 13,
    Source2PlayStats_UTCDateTime = 14,
    Source2PlayStats_SteamIDTrustBucket = 15,
    Source2PlayStats_SteamIDTrustBucketMin = 16,
    Source2PlayStats_SteamID = 17,
};

#endif // _INCLUDE_ESOURCE2PLAYSTATSFIELDTYPE_H
