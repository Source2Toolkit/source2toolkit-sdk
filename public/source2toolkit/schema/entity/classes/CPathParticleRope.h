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

#ifndef _INCLUDE_CPATHPARTICLEROPE_H
#define _INCLUDE_CPATHPARTICLEROPE_H
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

class InfoForResourceTypeIParticleSystemDefinition;

class CPathParticleRope : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CPathParticleRope);

    SCHEMA_FIELD(bool, m_bStartActive);
    SCHEMA_FIELD(float, m_flMaxSimulationTime);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszEffectName);
    SCHEMA_FIELD(CUtlVector<CUtlSymbolLarge>, m_PathNodes_Name);
    SCHEMA_FIELD(float, m_flParticleSpacing);
    SCHEMA_FIELD(float, m_flSlack);
    SCHEMA_FIELD(float, m_flRadius);
    SCHEMA_FIELD(Color, m_ColorTint);
    SCHEMA_FIELD(int32_t, m_nEffectState);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIParticleSystemDefinition>, m_iEffectIndex);
    SCHEMA_FIELD(CUtlVector<Vector>, m_PathNodes_Position);
    SCHEMA_FIELD(CUtlVector<Vector>, m_PathNodes_TangentIn);
    SCHEMA_FIELD(CUtlVector<Vector>, m_PathNodes_TangentOut);
    SCHEMA_FIELD(CUtlVector<Vector>, m_PathNodes_Color);
    SCHEMA_FIELD(CUtlVector<bool>, m_PathNodes_PinEnabled);
    SCHEMA_FIELD(CUtlVector<float>, m_PathNodes_RadiusScale);

public:
    static CPathParticleRope* New(const char* className)
    {
        return CBaseEntity::New<CPathParticleRope>(className);
    }

    static CPathParticleRope* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPathParticleRope>(iIndex);
    }

    static CPathParticleRope* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPathParticleRope> GetHandle()
    {
        return CBaseEntity::GetHandle<CPathParticleRope>();
    }
};

#endif // _INCLUDE_CPATHPARTICLEROPE_H
