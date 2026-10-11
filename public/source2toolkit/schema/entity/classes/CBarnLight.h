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

#ifndef _INCLUDE_CBARNLIGHT_H
#define _INCLUDE_CBARNLIGHT_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CBaseModelEntity.h"
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

class CBarnLight : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CBarnLight);

    SCHEMA_FIELD(bool, m_bEnabled);
    SCHEMA_FIELD(int32_t, m_nColorMode);
    SCHEMA_FIELD(Color, m_Color);
    SCHEMA_FIELD(float, m_flColorTemperature);
    SCHEMA_FIELD(float, m_flBrightness);
    SCHEMA_FIELD(float, m_flBrightnessScale);
    SCHEMA_FIELD(int32_t, m_nDirectLight);
    SCHEMA_FIELD(int32_t, m_nBakedShadowIndex);
    SCHEMA_FIELD(int32_t, m_nLightPathUniqueId);
    SCHEMA_FIELD(int32_t, m_nLightMapUniqueId);
    SCHEMA_FIELD(int32_t, m_nLuminaireShape);
    SCHEMA_FIELD(float, m_flLuminaireSize);
    SCHEMA_FIELD(float, m_flLuminaireAnisotropy);
    SCHEMA_FIELD(CUtlString, m_LightStyleString);
    SCHEMA_FIELD(float, m_flLightStyleStartTime);
    SCHEMA_FIELD(CUtlVector<CUtlString>, m_QueuedLightStyleStrings);
    SCHEMA_FIELD(CUtlVector<CUtlString>, m_LightStyleEvents);
    SCHEMA_FIELD(CUtlVector<CHandle<CBaseModelEntity>>, m_LightStyleTargets);
    SCHEMA_FIELD_POINTER(CEntityIOOutput, m_StyleEvent);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_hLightCookie);
    SCHEMA_FIELD(float, m_flShape);
    SCHEMA_FIELD(float, m_flSoftX);
    SCHEMA_FIELD(float, m_flSoftY);
    SCHEMA_FIELD(float, m_flSkirt);
    SCHEMA_FIELD(float, m_flSkirtNear);
    SCHEMA_FIELD(Vector, m_vSizeParams);
    SCHEMA_FIELD(float, m_flRange);
    SCHEMA_FIELD(Vector, m_vShear);
    SCHEMA_FIELD(int32_t, m_nBakeSpecularToCubemaps);
    SCHEMA_FIELD(Vector, m_vBakeSpecularToCubemapsSize);
    SCHEMA_FIELD(float, m_flBakeSpecularToCubemapsScale);
    SCHEMA_FIELD(int32_t, m_nCastShadows);
    SCHEMA_FIELD(int32_t, m_nShadowMapSize);
    SCHEMA_FIELD(int32_t, m_nShadowPriority);
    SCHEMA_FIELD(bool, m_bContactShadow);
    SCHEMA_FIELD(bool, m_bForceShadowsEnabled);
    SCHEMA_FIELD(int32_t, m_nBounceLight);
    SCHEMA_FIELD(float, m_flBounceScale);
    SCHEMA_FIELD(float, m_flMinRoughness);
    SCHEMA_FIELD(Vector, m_vAlternateColor);
    SCHEMA_FIELD(float, m_fAlternateColorBrightness);
    SCHEMA_FIELD(int32_t, m_nFog);
    SCHEMA_FIELD(float, m_flFogStrength);
    SCHEMA_FIELD(int32_t, m_nFogShadows);
    SCHEMA_FIELD(float, m_flFogScale);
    SCHEMA_FIELD(float, m_flFadeSizeStart);
    SCHEMA_FIELD(float, m_flFadeSizeEnd);
    SCHEMA_FIELD(float, m_flShadowFadeSizeStart);
    SCHEMA_FIELD(float, m_flShadowFadeSizeEnd);
    SCHEMA_FIELD(bool, m_bPrecomputedFieldsValid);
    SCHEMA_FIELD(Vector, m_vPrecomputedBoundsMins);
    SCHEMA_FIELD(Vector, m_vPrecomputedBoundsMaxs);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBOrigin);
    SCHEMA_FIELD(QAngle, m_vPrecomputedOBBAngles);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBExtent);
    SCHEMA_FIELD(int32_t, m_nPrecomputedSubFrusta);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBOrigin0);
    SCHEMA_FIELD(QAngle, m_vPrecomputedOBBAngles0);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBExtent0);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBOrigin1);
    SCHEMA_FIELD(QAngle, m_vPrecomputedOBBAngles1);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBExtent1);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBOrigin2);
    SCHEMA_FIELD(QAngle, m_vPrecomputedOBBAngles2);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBExtent2);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBOrigin3);
    SCHEMA_FIELD(QAngle, m_vPrecomputedOBBAngles3);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBExtent3);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBOrigin4);
    SCHEMA_FIELD(QAngle, m_vPrecomputedOBBAngles4);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBExtent4);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBOrigin5);
    SCHEMA_FIELD(QAngle, m_vPrecomputedOBBAngles5);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBExtent5);
    SCHEMA_FIELD(bool, m_bPvsModifyEntity);
    SCHEMA_FIELD(bool, m_bTransmitAlways);
    SCHEMA_FIELD(CUtlVector<uint16_t>, m_VisClusters);

public:
    static CBarnLight* New(const char* className)
    {
        return CBaseEntity::New<CBarnLight>(className);
    }

    static CBarnLight* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBarnLight>(iIndex);
    }

    static CBarnLight* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBarnLight> GetHandle()
    {
        return CBaseEntity::GetHandle<CBarnLight>();
    }
};

#endif // _INCLUDE_CBARNLIGHT_H
