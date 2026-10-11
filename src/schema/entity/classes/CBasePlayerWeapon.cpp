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

#include "source2toolkit/schema/entity/classes/CBasePlayerWeapon.h"

#include <tier0_strtools.h>

CCSWeaponBaseVData* CBasePlayerWeapon::GetWeaponVData()
{
    return (CCSWeaponBaseVData*)GetVData();
}

const char* CBasePlayerWeapon::GetWeaponClassname()
{
    const char* pszClassname = GetClassname();
    if (V_StringHasPrefixCaseSensitive(pszClassname, "item_"))
        return pszClassname;

    switch (m_AttributeManager().m_Item().m_iItemDefinitionIndex())
    {
    case 23:
        return "weapon_mp5sd";
    case 41:
        return "weapon_knifegg";
    case 42:
        return "weapon_knife";
    case 59:
        return "weapon_knife_t";
    case 60:
        return "weapon_m4a1_silencer";
    case 61:
        return "weapon_usp_silencer";
    case 63:
        return "weapon_cz75a";
    case 64:
        return "weapon_revolver";
    default:
        return pszClassname;
    }
}
