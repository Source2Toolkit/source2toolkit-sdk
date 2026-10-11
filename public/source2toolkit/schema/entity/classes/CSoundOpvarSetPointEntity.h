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

#ifndef _INCLUDE_CSOUNDOPVARSETPOINTENTITY_H
#define _INCLUDE_CSOUNDOPVARSETPOINTENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CSoundOpvarSetPointBase.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"

#include <ehandle.h>
#include <entityhandle.h>
#include <igameevents.h>
#include <utlbinaryblock.h>
#include <utlstring.h>
#include <utlstringtoken.h>
#include <utlsymbol.h>
#include <utlsymbollarge.h>
#include <vector.h>

#include <cstdint>

class CSoundOpvarSetPointEntity : public CSoundOpvarSetPointBase
{
public:
    DECLARE_SCHEMA_CLASS(CSoundOpvarSetPointEntity);

    SCHEMA_FIELD(CEntityIOOutput, m_OnEnter);
    SCHEMA_FIELD(CEntityIOOutput, m_OnExit);
    SCHEMA_FIELD(bool, m_bAutoDisable);
    SCHEMA_FIELD(float, m_flDistanceMin);
    SCHEMA_FIELD(float, m_flDistanceMax);
    SCHEMA_FIELD(float, m_flDistanceMapMin);
    SCHEMA_FIELD(float, m_flDistanceMapMax);
    SCHEMA_FIELD(float, m_flOcclusionRadius);
    SCHEMA_FIELD(float, m_flOcclusionMin);
    SCHEMA_FIELD(float, m_flOcclusionMax);
    SCHEMA_FIELD(float, m_flValSetOnDisable);
    SCHEMA_FIELD(bool, m_bSetValueOnDisable);
    SCHEMA_FIELD(bool, m_bReloading);
    SCHEMA_FIELD(int32_t, m_nSimulationMode);
    SCHEMA_FIELD(int32_t, m_nVisibilitySamples);
    SCHEMA_FIELD(Vector, m_vDynamicProxyPoint);
    SCHEMA_FIELD(float, m_flDynamicMaximumOcclusion);
    SCHEMA_FIELD(CEntityHandle, m_hDynamicEntity);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszDynamicEntityName);
    SCHEMA_FIELD(float, m_flPathingDistanceNormFactor);
    SCHEMA_FIELD(Vector, m_vPathingSourcePos);
    SCHEMA_FIELD(Vector, m_vPathingListenerPos);
    SCHEMA_FIELD(Vector, m_vPathingDirection);
    SCHEMA_FIELD(int32_t, m_nPathingSourceIndex);

public:
    static CSoundOpvarSetPointEntity* New(const char* className)
    {
        return CBaseEntity::New<CSoundOpvarSetPointEntity>(className);
    }

    static CSoundOpvarSetPointEntity* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CSoundOpvarSetPointEntity>(iIndex);
    }

    static CSoundOpvarSetPointEntity* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CSoundOpvarSetPointEntity> GetHandle()
    {
        return CBaseEntity::GetHandle<CSoundOpvarSetPointEntity>();
    }
};

#endif // _INCLUDE_CSOUNDOPVARSETPOINTENTITY_H
