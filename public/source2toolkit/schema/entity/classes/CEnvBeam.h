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

#ifndef _INCLUDE_CENVBEAM_H
#define _INCLUDE_CENVBEAM_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/Touch_t.h"
#include "CBaseEntity.h"
#include "CBeam.h"
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

class CBaseEntity;
class InfoForResourceTypeIMaterial2;

class CEnvBeam : public CBeam
{
public:
    DECLARE_SCHEMA_CLASS(CEnvBeam);

    SCHEMA_FIELD(int32_t, m_active);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIMaterial2>, m_spriteTexture);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszStartEntity);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszEndEntity);
    SCHEMA_FIELD(float, m_life);
    SCHEMA_FIELD(float, m_boltWidth);
    SCHEMA_FIELD(float, m_noiseAmplitude);
    SCHEMA_FIELD(int32_t, m_speed);
    SCHEMA_FIELD(float, m_restrike);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSpriteName);
    SCHEMA_FIELD(int32_t, m_frameStart);
    SCHEMA_FIELD(Vector, m_vEndPointWorld);
    SCHEMA_FIELD(Vector, m_vEndPointRelative);
    SCHEMA_FIELD(float, m_radius);
    SCHEMA_FIELD(Touch_t, m_TouchType);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iFilterName);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hFilter);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszDecal);
    SCHEMA_FIELD(CEntityIOOutput, m_OnTouchedByEntity);

public:
    static CEnvBeam* New(const char* className)
    {
        return CBaseEntity::New<CEnvBeam>(className);
    }

    static CEnvBeam* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CEnvBeam>(iIndex);
    }

    static CEnvBeam* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CEnvBeam> GetHandle()
    {
        return CBaseEntity::GetHandle<CEnvBeam>();
    }
};

#endif // _INCLUDE_CENVBEAM_H
