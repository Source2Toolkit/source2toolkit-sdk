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

#ifndef _INCLUDE_CENVVOLUMETRICFOGCONTROLLER_H
#define _INCLUDE_CENVVOLUMETRICFOGCONTROLLER_H
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

class CEnvVolumetricFogController : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CEnvVolumetricFogController);

    SCHEMA_FIELD(float, m_flScattering);
    SCHEMA_FIELD(Color, m_TintColor);
    SCHEMA_FIELD(float, m_flAnisotropy);
    SCHEMA_FIELD(float, m_flFadeSpeed);
    SCHEMA_FIELD(float, m_flDrawDistance);
    SCHEMA_FIELD(float, m_flFadeInStart);
    SCHEMA_FIELD(float, m_flFadeInEnd);
    SCHEMA_FIELD(float, m_flIndirectStrength);
    SCHEMA_FIELD(int32_t, m_nVolumeDepth);
    SCHEMA_FIELD(float, m_fFirstVolumeSliceThickness);
    SCHEMA_FIELD(int32_t, m_nIndirectTextureDimX);
    SCHEMA_FIELD(int32_t, m_nIndirectTextureDimY);
    SCHEMA_FIELD(int32_t, m_nIndirectTextureDimZ);
    SCHEMA_FIELD(Vector, m_vBoxMins);
    SCHEMA_FIELD(Vector, m_vBoxMaxs);
    SCHEMA_FIELD(bool, m_bActive);
    SCHEMA_FIELD(float, m_flStartAnisoTime);
    SCHEMA_FIELD(float, m_flStartScatterTime);
    SCHEMA_FIELD(float, m_flStartDrawDistanceTime);
    SCHEMA_FIELD(float, m_flStartAnisotropy);
    SCHEMA_FIELD(float, m_flStartScattering);
    SCHEMA_FIELD(float, m_flStartDrawDistance);
    SCHEMA_FIELD(float, m_flDefaultAnisotropy);
    SCHEMA_FIELD(float, m_flDefaultScattering);
    SCHEMA_FIELD(float, m_flDefaultDrawDistance);
    SCHEMA_FIELD(bool, m_bStartDisabled);
    SCHEMA_FIELD(bool, m_bEnableIndirect);
    SCHEMA_FIELD(bool, m_bIsMaster);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCTextureBase>, m_hFogIndirectTexture);
    SCHEMA_FIELD(int32_t, m_nForceRefreshCount);
    SCHEMA_FIELD(float, m_fNoiseSpeed);
    SCHEMA_FIELD(float, m_fNoiseStrength);
    SCHEMA_FIELD(Vector, m_vNoiseScale);
    SCHEMA_FIELD(float, m_fWindSpeed);
    SCHEMA_FIELD(Vector, m_vWindDirection);
    SCHEMA_FIELD(bool, m_bFirstTime);

public:
    static CEnvVolumetricFogController* New(const char* className)
    {
        return CBaseEntity::New<CEnvVolumetricFogController>(className);
    }

    static CEnvVolumetricFogController* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CEnvVolumetricFogController>(iIndex);
    }

    static CEnvVolumetricFogController* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CEnvVolumetricFogController> GetHandle()
    {
        return CBaseEntity::GetHandle<CEnvVolumetricFogController>();
    }
};

#endif // _INCLUDE_CENVVOLUMETRICFOGCONTROLLER_H
