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

#ifndef _INCLUDE_ECOMMUNITYITEMCLASS_H
#define _INCLUDE_ECOMMUNITYITEMCLASS_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class ECommunityItemClass : uint32_t
{
    k_ECommunityItemClass_Invalid = 0,
    k_ECommunityItemClass_Badge = 1,
    k_ECommunityItemClass_GameCard = 2,
    k_ECommunityItemClass_ProfileBackground = 3,
    k_ECommunityItemClass_Emoticon = 4,
    k_ECommunityItemClass_BoosterPack = 5,
    k_ECommunityItemClass_Consumable = 6,
    k_ECommunityItemClass_GameGoo = 7,
    k_ECommunityItemClass_ProfileModifier = 8,
    k_ECommunityItemClass_Scene = 9,
    k_ECommunityItemClass_SalienItem = 10,
};

#endif // _INCLUDE_ECOMMUNITYITEMCLASS_H
