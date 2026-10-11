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

#ifndef _INCLUDE_CENVCOMBINEDLIGHTPROBEVOLUME_H
#define _INCLUDE_CENVCOMBINEDLIGHTPROBEVOLUME_H
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

class CEnvCombinedLightProbeVolume : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CEnvCombinedLightProbeVolume);

    SCHEMA_FIELD(Color, m_Entity_Color);
    SCHEMA_FIELD(float, m_Entity_flBrightness);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hCubemapTexture);
    SCHEMA_FIELD(bool, m_Entity_bCustomCubemapTexture);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hLightProbeTexture_AmbientCube);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hLightProbeTexture_SDF);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hLightProbeTexture_SH2_DC);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hLightProbeTexture_SH2_L1);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hLightProbeDirectLightIndicesTexture);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hLightProbeDirectLightScalarsTexture);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_Entity_hLightProbeDirectLightShadowsTexture);
    SCHEMA_FIELD(Vector, m_Entity_vBoxMins);
    SCHEMA_FIELD(Vector, m_Entity_vBoxMaxs);
    SCHEMA_FIELD(bool, m_Entity_bMoveable);
    SCHEMA_FIELD(int32_t, m_Entity_nHandshake);
    SCHEMA_FIELD(int32_t, m_Entity_nEnvCubeMapArrayIndex);
    SCHEMA_FIELD(int32_t, m_Entity_nPriority);
    SCHEMA_FIELD(bool, m_Entity_bStartDisabled);
    SCHEMA_FIELD(float, m_Entity_flEdgeFadeDist);
    SCHEMA_FIELD(Vector, m_Entity_vEdgeFadeDists);
    SCHEMA_FIELD(int32_t, m_Entity_nLightProbeSizeX);
    SCHEMA_FIELD(int32_t, m_Entity_nLightProbeSizeY);
    SCHEMA_FIELD(int32_t, m_Entity_nLightProbeSizeZ);
    SCHEMA_FIELD(int32_t, m_Entity_nLightProbeAtlasX);
    SCHEMA_FIELD(int32_t, m_Entity_nLightProbeAtlasY);
    SCHEMA_FIELD(int32_t, m_Entity_nLightProbeAtlasZ);
    SCHEMA_FIELD(bool, m_Entity_bEnabled);

public:
    static CEnvCombinedLightProbeVolume* New(const char* className)
    {
        return CBaseEntity::New<CEnvCombinedLightProbeVolume>(className);
    }

    static CEnvCombinedLightProbeVolume* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CEnvCombinedLightProbeVolume>(iIndex);
    }

    static CEnvCombinedLightProbeVolume* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CEnvCombinedLightProbeVolume> GetHandle()
    {
        return CBaseEntity::GetHandle<CEnvCombinedLightProbeVolume>();
    }
};

#endif // _INCLUDE_CENVCOMBINEDLIGHTPROBEVOLUME_H
