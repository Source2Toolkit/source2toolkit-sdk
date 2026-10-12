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

#ifndef _INCLUDE_CLIGHTCOMPONENT_H
#define _INCLUDE_CLIGHTCOMPONENT_H
#ifdef _WIN32
#pragma once
#endif

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

class CLightComponent : public CEntityComponent
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CLightComponent);

    SCHEMA_FIELD(CNetworkVarChainer, __m_pChainEntity);
    SCHEMA_FIELD(Color, m_Color);
    SCHEMA_FIELD(Color, m_SecondaryColor);
    SCHEMA_FIELD(float, m_flBrightness);
    SCHEMA_FIELD(float, m_flBrightnessScale);
    SCHEMA_FIELD(float, m_flBrightnessMult);
    SCHEMA_FIELD(float, m_flRange);
    SCHEMA_FIELD(float, m_flFalloff);
    SCHEMA_FIELD(float, m_flAttenuation0);
    SCHEMA_FIELD(float, m_flAttenuation1);
    SCHEMA_FIELD(float, m_flAttenuation2);
    SCHEMA_FIELD(float, m_flTheta);
    SCHEMA_FIELD(float, m_flPhi);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_hLightCookie);
    SCHEMA_FIELD(int32_t, m_nCascades);
    SCHEMA_FIELD(int32_t, m_nCastShadows);
    SCHEMA_FIELD(int32_t, m_nShadowWidth);
    SCHEMA_FIELD(int32_t, m_nShadowHeight);
    SCHEMA_FIELD(bool, m_bRenderDiffuse);
    SCHEMA_FIELD(int32_t, m_nRenderSpecular);
    SCHEMA_FIELD(bool, m_bRenderTransmissive);
    SCHEMA_FIELD(float, m_flOrthoLightWidth);
    SCHEMA_FIELD(float, m_flOrthoLightHeight);
    SCHEMA_FIELD(int32_t, m_nStyle);
    SCHEMA_FIELD(CUtlString, m_Pattern);
    SCHEMA_FIELD(int32_t, m_nCascadeRenderStaticObjects);
    SCHEMA_FIELD(float, m_flShadowCascadeCrossFade);
    SCHEMA_FIELD(float, m_flShadowCascadeDistanceFade);
    SCHEMA_FIELD(float, m_flShadowCascadeDistance0);
    SCHEMA_FIELD(float, m_flShadowCascadeDistance1);
    SCHEMA_FIELD(float, m_flShadowCascadeDistance2);
    SCHEMA_FIELD(float, m_flShadowCascadeDistance3);
    SCHEMA_FIELD(int32_t, m_nShadowCascadeResolution0);
    SCHEMA_FIELD(int32_t, m_nShadowCascadeResolution1);
    SCHEMA_FIELD(int32_t, m_nShadowCascadeResolution2);
    SCHEMA_FIELD(int32_t, m_nShadowCascadeResolution3);
    SCHEMA_FIELD(bool, m_bUsesBakedShadowing);
    SCHEMA_FIELD(int32_t, m_nShadowPriority);
    SCHEMA_FIELD(int32_t, m_nBakedShadowIndex);
    SCHEMA_FIELD(int32_t, m_nLightPathUniqueId);
    SCHEMA_FIELD(int32_t, m_nLightMapUniqueId);
    SCHEMA_FIELD(bool, m_bRenderToCubemaps);
    SCHEMA_FIELD(bool, m_bAllowSSTGeneration);
    SCHEMA_FIELD(int32_t, m_nDirectLight);
    SCHEMA_FIELD(int32_t, m_nBounceLight);
    SCHEMA_FIELD(float, m_flBounceScale);
    SCHEMA_FIELD(float, m_flFadeMinDist);
    SCHEMA_FIELD(float, m_flFadeMaxDist);
    SCHEMA_FIELD(float, m_flShadowFadeMinDist);
    SCHEMA_FIELD(float, m_flShadowFadeMaxDist);
    SCHEMA_FIELD(bool, m_bEnabled);
    SCHEMA_FIELD(bool, m_bFlicker);
    SCHEMA_FIELD(bool, m_bPrecomputedFieldsValid);
    SCHEMA_FIELD(Vector, m_vPrecomputedBoundsMins);
    SCHEMA_FIELD(Vector, m_vPrecomputedBoundsMaxs);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBOrigin);
    SCHEMA_FIELD(QAngle, m_vPrecomputedOBBAngles);
    SCHEMA_FIELD(Vector, m_vPrecomputedOBBExtent);
    SCHEMA_FIELD(float, m_flPrecomputedMaxRange);
    SCHEMA_FIELD(int32_t, m_nFogLightingMode);
    SCHEMA_FIELD(float, m_flFogContributionStength);
    SCHEMA_FIELD(float, m_flNearClipPlane);
    SCHEMA_FIELD(Color, m_SkyColor);
    SCHEMA_FIELD(float, m_flSkyIntensity);
    SCHEMA_FIELD(Color, m_SkyAmbientBounce);
    SCHEMA_FIELD(bool, m_bUseSecondaryColor);
    SCHEMA_FIELD(bool, m_bMixedShadows);
    SCHEMA_FIELD(float, m_flLightStyleStartTime);
    SCHEMA_FIELD(float, m_flCapsuleLength);
    SCHEMA_FIELD(float, m_flMinRoughness);
    SCHEMA_FIELD(bool, m_bPvsModifyEntity);
};

#endif // _INCLUDE_CLIGHTCOMPONENT_H
