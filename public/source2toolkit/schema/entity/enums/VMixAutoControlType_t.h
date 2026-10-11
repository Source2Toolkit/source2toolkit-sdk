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

#ifndef _INCLUDE_VMIXAUTOCONTROLTYPE_T_H
#define _INCLUDE_VMIXAUTOCONTROLTYPE_T_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class VMixAutoControlType_t : uint8_t
{
    VMIX_AUTO_SEND_LEVEL = 0,
    VMIX_AUTO_STACK_VAR = 1,
    VMIX_AUTO_PLAYTIME = 2,
    VMIX_AUTO_DISTANCE = 3,
    VMIX_AUTO_POSITION_X = 4,
    VMIX_AUTO_POSITION_Y = 5,
    VMIX_AUTO_POSITION_Z = 6,
    VMIX_AUTO_POSITION_VECTOR = 7,
    VMIX_AUTO_LISTENER_YAW_SIN = 8,
    VMIX_AUTO_LISTENER_YAW_COS = 9,
    VMIX_AUTO_LISTENER_PITCH_SIN = 10,
    VMIX_AUTO_LISTENER_PITCH_COS = 11,
    VMIX_AUTO_LISTENER_ROLL_SIN = 12,
    VMIX_AUTO_LISTENER_ROLL_COS = 13,
};

#endif // _INCLUDE_VMIXAUTOCONTROLTYPE_T_H
