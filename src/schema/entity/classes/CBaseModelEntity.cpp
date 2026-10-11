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

#include "source2toolkit/schema/entity/classes/CBaseModelEntity.h"

#include "source2toolkit/IToolkitAddresses.h"
#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entity/classes/CBodyComponent.h"
#include "source2toolkit/schema/entity/classes/CSkeletonInstance.h"
TOOLKIT_GLOBALVARS();

CUtlSymbolLarge CBaseModelEntity::GetModelName()
{
    CBodyComponent* bodyComponent = m_CBodyComponent();
    if (bodyComponent && bodyComponent->m_pSceneNode() && bodyComponent->m_pSceneNode()->GetSkeletonInstance())
    {
        return bodyComponent->m_pSceneNode()->GetSkeletonInstance()->m_modelState().m_ModelName();
    }

    return CUtlSymbolLarge();
}

Vector CBaseModelEntity::GetEyePosition()
{
    Vector x = Vector(m_vecViewOffset().m_vecX, m_vecViewOffset().m_vecY, m_vecViewOffset().m_vecZ);
    Vector m_vecOrigin = GetAbsOrigin();
    return {
        m_vecOrigin.x + x.x,
        m_vecOrigin.y + x.y,
        m_vecOrigin.z + x.z
    };
}

void CBaseModelEntity::SetModel(const char* pszModel, HookChain eChain) {
    ResolveHookChain(ADDR_SET_MODEL(), eChain)(this, pszModel);
}

void CBaseModelEntity::SetBodyGroup(const char* pszName, int nValue)
{
    // The engine exposes no direct setter -- only an entity input taking
    // "name,value".
    char szValue[128];
    V_snprintf(szValue, sizeof(szValue), "%s,%d", pszName, nValue);

    AcceptInput("SetBodyGroup", this, this, szValue);
}
