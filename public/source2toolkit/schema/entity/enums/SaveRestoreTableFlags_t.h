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

#ifndef _INCLUDE_SAVERESTORETABLEFLAGS_T_H
#define _INCLUDE_SAVERESTORETABLEFLAGS_T_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class SaveRestoreTableFlags_t : uint32_t
{
    FENTTABLE_NONE = 0,
    FENTTABLE_PLAYER = 2147483648,
    FENTTABLE_REMOVED = 1073741824,
    FENTTABLE_MOVEABLE = 536870912,
    FENTTABLE_GLOBAL = 268435456,
    FENTTABLE_PLAYERCHILD = 134217728,
    LEVELMASK_BIT_0 = 1,
    LEVELMASK_BIT_1 = 2,
    LEVELMASK_BIT_2 = 4,
    LEVELMASK_BIT_3 = 8,
    LEVELMASK_BIT_4 = 16,
    LEVELMASK_BIT_5 = 32,
    LEVELMASK_BIT_6 = 64,
    LEVELMASK_BIT_7 = 128,
    LEVELMASK_BIT_8 = 256,
    LEVELMASK_BIT_9 = 512,
    LEVELMASK_BIT_10 = 1024,
    LEVELMASK_BIT_11 = 2048,
    LEVELMASK_BIT_12 = 4096,
    LEVELMASK_BIT_13 = 8192,
    LEVELMASK_BIT_14 = 16384,
    LEVELMASK_BIT_15 = 32768,
};

#endif // _INCLUDE_SAVERESTORETABLEFLAGS_T_H
