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

#ifndef _INCLUDE_VMIXOFFSETTYPE_T_H
#define _INCLUDE_VMIXOFFSETTYPE_T_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class VMixOffsetType_t : uint32_t
{
    VO_CHAR = 0,
    VO_ARRAY = 1,
    VO_BOOL = 2,
    VO_FLOAT = 3,
    VO_UINT32 = 4,
    VO_INT32 = 5,
    VO_VECTOR = 6,
    VO_QUATERNION = 7,
    VO_CUBIC_SPLINE = 8,
    VO_VSND_INPUT = 9,
    VO_FLOAT_UTLVECTOR = 10,
    VO_SHAREDPTR_IR = 11,
    VO_TYPE_COUNT = 12,
};

#endif // _INCLUDE_VMIXOFFSETTYPE_T_H
