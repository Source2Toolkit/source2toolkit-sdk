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

#ifndef _INCLUDE_CENVCUBEMAP_H
#define _INCLUDE_CENVCUBEMAP_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
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

class InfoForResourceTypeCTextureBase;

class CEnvCubemap : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CEnvCubemap);

    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hCubemapTexture);
    SCHEMA_FIELD(bool, m_Entity_bCustomCubemapTexture);
    SCHEMA_FIELD(float, m_Entity_flInfluenceRadius);
    SCHEMA_FIELD(Vector, m_Entity_vBoxProjectMins);
    SCHEMA_FIELD(Vector, m_Entity_vBoxProjectMaxs);
    SCHEMA_FIELD(bool, m_Entity_bMoveable);
    SCHEMA_FIELD(int32_t, m_Entity_nHandshake);
    SCHEMA_FIELD(int32_t, m_Entity_nEnvCubeMapArrayIndex);
    SCHEMA_FIELD(int32_t, m_Entity_nPriority);
    SCHEMA_FIELD(float, m_Entity_flEdgeFadeDist);
    SCHEMA_FIELD(Vector, m_Entity_vEdgeFadeDists);
    SCHEMA_FIELD(float, m_Entity_flDiffuseScale);
    SCHEMA_FIELD(bool, m_Entity_bStartDisabled);
    SCHEMA_FIELD(bool, m_Entity_bDefaultEnvMap);
    SCHEMA_FIELD(bool, m_Entity_bDefaultSpecEnvMap);
    SCHEMA_FIELD(bool, m_Entity_bIndoorCubeMap);
    SCHEMA_FIELD(bool, m_Entity_bCopyDiffuseFromDefaultCubemap);
    SCHEMA_FIELD(bool, m_Entity_bEnabled);

public:
    static CEnvCubemap* New(const char* className)
    {
        return CBaseEntity::New<CEnvCubemap>(className);
    }

    static CEnvCubemap* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CEnvCubemap>(iIndex);
    }

    static CEnvCubemap* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CEnvCubemap> GetHandle()
    {
        return CBaseEntity::GetHandle<CEnvCubemap>();
    }
};

#endif // _INCLUDE_CENVCUBEMAP_H
