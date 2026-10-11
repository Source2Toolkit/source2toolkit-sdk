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

#ifndef _INCLUDE_PREVIEWCHARACTERBANNERANIMATION_H
#define _INCLUDE_PREVIEWCHARACTERBANNERANIMATION_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class PreviewCharacterBannerAnimation : int32_t
{
    INVALID = -1,
    IDLE_OFFSCREEN = 0,
    BANNER_AWP_ACE_GUN = 1,
    BANNER_AWP_ACE_A = 2,
    BANNER_AWP_ACE_B = 3,
    BANNER_AWP_ACE_C = 4,
    BANNER_AWP_ACE_D = 5,
    BANNER_AWP_ACE_E = 6,
    BANNER_PISTOL3SHOT = 7,
    BANNER_3SHOT_A = 8,
    BANNER_3SHOT_B = 9,
    BANNER_3SHOT_C = 10,
    BANNER_PISTOL4SHOT = 11,
    BANNER_4SHOT_A = 12,
    BANNER_4SHOT_B = 13,
    BANNER_4SHOT_C = 14,
    BANNER_4SHOT_D = 15,
    CELEBRATE_STRETCH_NOWEAP_IDLE0 = 16,
    BANNER_BOMB_PLANT = 17,
    BANNER_BOMB_DEFUSAL_VER1 = 18,
    BANNER_BOMB_DEFUSAL_VER2 = 19,
    BANNER_FIRE = 20,
    BANNER_BOMB_BLAST_TOSS = 21,
    BANNER_BOMB_BLAST01 = 22,
    BANNER_BOMB_BLAST02 = 23,
    BANNER_BOMB_BLAST03 = 24,
    BANNER_CELEBRATE_01 = 25,
    BANNER_CELEBRATE_02 = 26,
    BANNER_CELEBRATE_03 = 27,
    BANNER_CELEBRATE_04 = 28,
};

#endif // _INCLUDE_PREVIEWCHARACTERBANNERANIMATION_H
