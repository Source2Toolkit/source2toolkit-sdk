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

#ifndef _INCLUDE_QUESTPROGRESS__REASON_H
#define _INCLUDE_QUESTPROGRESS__REASON_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class QuestProgress__Reason : uint32_t
{
    QUEST_NONINITIALIZED = 0,
    QUEST_OK = 1,
    QUEST_NOT_ENOUGH_PLAYERS = 2,
    QUEST_WARMUP = 3,
    QUEST_NOT_CONNECTED_TO_STEAM = 4,
    QUEST_NONOFFICIAL_SERVER = 5,
    QUEST_NO_ENTITLEMENT = 6,
    QUEST_NO_QUEST = 7,
    QUEST_PLAYER_IS_BOT = 8,
    QUEST_WRONG_MAP = 9,
    QUEST_WRONG_MODE = 10,
    QUEST_NOT_SYNCED_WITH_SERVER = 11,
    QUEST_REASON_MAX = 12,
};

#endif // _INCLUDE_QUESTPROGRESS__REASON_H
