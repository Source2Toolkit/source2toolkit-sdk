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

#ifndef _INCLUDE_CPOINTCAMERA_H
#define _INCLUDE_CPOINTCAMERA_H
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

class CPointCamera : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CPointCamera);

    SCHEMA_FIELD(float, m_FOV);
    SCHEMA_FIELD(float, m_Resolution);
    SCHEMA_FIELD(bool, m_bFogEnable);
    SCHEMA_FIELD(Color, m_FogColor);
    SCHEMA_FIELD(float, m_flFogStart);
    SCHEMA_FIELD(float, m_flFogEnd);
    SCHEMA_FIELD(float, m_flFogMaxDensity);
    SCHEMA_FIELD(bool, m_bActive);
    SCHEMA_FIELD(bool, m_bUseScreenAspectRatio);
    SCHEMA_FIELD(float, m_flAspectRatio);
    SCHEMA_FIELD(bool, m_bNoSky);
    SCHEMA_FIELD(float, m_fBrightness);
    SCHEMA_FIELD(float, m_flZFar);
    SCHEMA_FIELD(float, m_flZNear);
    SCHEMA_FIELD(bool, m_bCanHLTVUse);
    SCHEMA_FIELD(bool, m_bAlignWithParent);
    SCHEMA_FIELD(bool, m_bDofEnabled);
    SCHEMA_FIELD(float, m_flDofNearBlurry);
    SCHEMA_FIELD(float, m_flDofNearCrisp);
    SCHEMA_FIELD(float, m_flDofFarCrisp);
    SCHEMA_FIELD(float, m_flDofFarBlurry);
    SCHEMA_FIELD(float, m_flDofTiltToGround);
    SCHEMA_FIELD(float, m_TargetFOV);
    SCHEMA_FIELD(float, m_DegreesPerSecond);
    SCHEMA_FIELD(bool, m_bIsOn);
    SCHEMA_FIELD(CPointCamera*, m_pNext);

public:
    static CPointCamera* New(const char* className)
    {
        return CBaseEntity::New<CPointCamera>(className);
    }

    static CPointCamera* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPointCamera>(iIndex);
    }

    static CPointCamera* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPointCamera> GetHandle()
    {
        return CBaseEntity::GetHandle<CPointCamera>();
    }
};

#endif // _INCLUDE_CPOINTCAMERA_H
