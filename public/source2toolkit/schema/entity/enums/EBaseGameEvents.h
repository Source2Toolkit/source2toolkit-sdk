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

#ifndef _INCLUDE_EBASEGAMEEVENTS_H
#define _INCLUDE_EBASEGAMEEVENTS_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class EBaseGameEvents : uint32_t
{
    GE_VDebugGameSessionIDEvent = 200,
    GE_PlaceDecalEvent = 201,
    GE_ClearWorldDecalsEvent = 202,
    GE_ClearEntityDecalsEvent = 203,
    GE_ClearDecalsForEntityEvent = 204,
    GE_Source1LegacyGameEventList = 205,
    GE_Source1LegacyListenEvents = 206,
    GE_Source1LegacyGameEvent = 207,
    GE_SosStartSoundEvent = 208,
    GE_SosStopSoundEvent = 209,
    GE_SosSetSoundEventParams = 210,
    GE_SosSetLibraryStackFields = 211,
    GE_SosStopSoundEventHash = 212,
    GE_ClothStiffenAnimEvent = 213,
    GE_ClothEffectAnimEvent = 214,
};

#endif // _INCLUDE_EBASEGAMEEVENTS_H
