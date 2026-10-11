/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
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
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
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
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

#ifndef _INCLUDE_HUDPANELDIALOGVARIABLESTRING_T_H
#define _INCLUDE_HUDPANELDIALOGVARIABLESTRING_T_H
#ifdef _WIN32
#pragma once
#endif

#include <platform.h>
#include <utlstring.h>

/**
 * @brief One dialog-variable string of a custom HUD layout state.
 *
 * Hand-written for the same reason as HUDPanelHasClass_t: it is a value type we
 * construct and hand to an engine-owned CUtlVector, so the layout has to be the
 * engine's, storage and vtable included.
 *
 * @note Panel and variable names are not stored here; the two indices point into
 *       CCSCustomHudLayout's m_vecPanelIds / m_vecDialogVariableNames tables.
 */
struct HUDPanelDialogVariableString_t
{
private:
    /// The engine's struct is polymorphic; the slot has to exist and hold the
    /// engine's table, which the constructor patches in.
    virtual void unk00() {}

public:
    /// <summary>Constructs new object with correct vtable pointer.</summary>
    HUDPanelDialogVariableString_t(uint16 nPanelIdIndex, uint16 nDialogVariableIndex, CUtlString sValue, bool bIsSet);

    /// Identity is the panel/variable pair -- the string is the value being set.
    bool operator==(const HUDPanelDialogVariableString_t& other) const
    {
        return m_nPanelIdIndex == other.m_nPanelIdIndex && m_nDialogVariableIndex == other.m_nDialogVariableIndex;
    }

    uint16 m_nPanelIdIndex;
    uint16 m_nDialogVariableIndex;
    CUtlString m_sValue;
    bool m_bIsSet;
};

#endif // _INCLUDE_HUDPANELDIALOGVARIABLESTRING_T_H
