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

#include "source2toolkit/schema/entity/classes/HUDPanelDialogVariableString_t.h"

#include "source2toolkit/IToolkitModule.h"

HUDPanelDialogVariableString_t::HUDPanelDialogVariableString_t(uint16 nPanelIdIndex, uint16 nDialogVariableIndex, CUtlString sValue, bool bIsSet)
{
    // We construct this ourselves, so slot 0 holds whatever the compiler put
    // there. Point it at the engine's table.
    static void* const pVTable = []() -> void*
    {
        IToolkitModule* libserver = IToolkitModule::New("server");
        if (!libserver)
            return nullptr;

        void* pTable = libserver->GetVirtualTableByName("HUDPanelDialogVariableString_t").GetPtr();
        delete libserver;
        return pTable;
    }();

    if (pVTable) *reinterpret_cast<void**>(this) = pVTable;

    m_nPanelIdIndex = nPanelIdIndex;
    m_nDialogVariableIndex = nDialogVariableIndex;
    m_sValue = sValue;
    m_bIsSet = bIsSet;
}
