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

#ifndef _INCLUDE_CBEAM_H
#define _INCLUDE_CBEAM_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/BeamType_t.h"
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

class CBaseEntity;
class InfoForResourceTypeIMaterial2;

class CBeam : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CBeam);

    SCHEMA_FIELD(float, m_flFrameRate);
    SCHEMA_FIELD(float, m_flHDRColorScale);
    SCHEMA_FIELD(float, m_flFireTime);
    SCHEMA_FIELD(float, m_flDamage);
    SCHEMA_FIELD(uint8_t, m_nNumBeamEnts);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIMaterial2>, m_hBaseMaterial);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIMaterial2>, m_nHaloIndex);
    SCHEMA_FIELD(BeamType_t, m_nBeamType);
    SCHEMA_FIELD(uint32_t, m_nBeamFlags);
    SCHEMA_FIELD_POINTER(CHandle<CBaseEntity>, m_hAttachEntity);
    SCHEMA_FIELD(float, m_fWidth);
    SCHEMA_FIELD(float, m_fEndWidth);
    SCHEMA_FIELD(float, m_fFadeLength);
    SCHEMA_FIELD(float, m_fHaloScale);
    SCHEMA_FIELD(float, m_fAmplitude);
    SCHEMA_FIELD(float, m_fStartFrame);
    SCHEMA_FIELD(float, m_fSpeed);
    SCHEMA_FIELD(float, m_flFrame);
    SCHEMA_FIELD(bool, m_bTurnedOff);
    SCHEMA_FIELD(Vector, m_vecEndPos);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hEndEntity);
    SCHEMA_FIELD(int32_t, m_nDissolveType);

public:
    static CBeam* New(const char* className)
    {
        return CBaseEntity::New<CBeam>(className);
    }

    static CBeam* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBeam>(iIndex);
    }

    static CBeam* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBeam> GetHandle()
    {
        return CBaseEntity::GetHandle<CBeam>();
    }
};

#endif // _INCLUDE_CBEAM_H
