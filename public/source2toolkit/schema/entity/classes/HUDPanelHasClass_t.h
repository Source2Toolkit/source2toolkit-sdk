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

#ifndef _INCLUDE_HUDPANELHASCLASS_T_H
#define _INCLUDE_HUDPANELHASCLASS_T_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/EHudPanelClassStatus_t.h"

#include <platform.h>

/**
 * @brief One "panel has CSS class" entry of a custom HUD layout state.
 *
 * Hand-written, not schema-generated: these are value types that we construct
 * ourselves and hand to an engine-owned CUtlVector, so the C++ layout has to be
 * the engine's real one. A SCHEMA_FIELD wrapper carries no storage -- the class
 * would be 3 bytes wide against the engine's 8, and both the stack instance and
 * every AddToTail()/Element() stride would be wrong.
 *
 * @note Panel and class names are not stored here; the two indices point into
 *       CCSCustomHudLayout's m_vecPanelIds / m_vecClassNames string tables.
 */
struct HUDPanelHasClass_t
{
    HUDPanelHasClass_t(uint16 nPanelIdIndex, uint16 nClassNameIndex, bool bHasClass) :
        m_nPanelIdIndex(nPanelIdIndex), m_nClassNameIndex(nClassNameIndex),
        m_eClassStatus(static_cast<EHudPanelClassStatus_t>(bHasClass))
    {
    }

    /// Identity is the panel/class pair -- the status is the value being set.
    bool operator==(const HUDPanelHasClass_t& other) const
    {
        return m_nPanelIdIndex == other.m_nPanelIdIndex && m_nClassNameIndex == other.m_nClassNameIndex;
    }

    uint16 m_nPanelIdIndex;
    uint16 m_nClassNameIndex;
    EHudPanelClassStatus_t m_eClassStatus;
};

#endif // _INCLUDE_HUDPANELHASCLASS_T_H
