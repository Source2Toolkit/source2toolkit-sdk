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

#ifndef _INCLUDE_RSCOMPARISON_T_H
#define _INCLUDE_RSCOMPARISON_T_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class RsComparison_t : uint8_t
{
    RS_CMP_NEVER = 0,
    RS_CMP_LESS = 1,
    RS_CMP_EQUAL = 2,
    RS_CMP_LESS_EQUAL = 3,
    RS_CMP_GREATER = 4,
    RS_CMP_NOT_EQUAL = 5,
    RS_CMP_GREATER_EQUAL = 6,
    RS_CMP_ALWAYS = 7,
    RS_CMP_FUNC_MASK = 7,
    RS_CMP_CLOSER_FARTHER_FLAG = 8,
    RS_CMP_CLOSER = 9,
    RS_CMP_CLOSER_EQUAL = 11,
    RS_CMP_FARTHER = 12,
    RS_CMP_FARTHER_EQUAL = 14,
};

#endif // _INCLUDE_RSCOMPARISON_T_H
