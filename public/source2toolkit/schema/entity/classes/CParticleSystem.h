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

#ifndef _INCLUDE_CPARTICLESYSTEM_H
#define _INCLUDE_CPARTICLESYSTEM_H
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

class CBaseEntity;
class InfoForResourceTypeIParticleSystemDefinition;

class CParticleSystem : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CParticleSystem);

    SCHEMA_FIELD_POINTER(char, m_szSnapshotFileName);
    SCHEMA_FIELD(bool, m_bActive);
    SCHEMA_FIELD(bool, m_bFrozen);
    SCHEMA_FIELD(float, m_flFreezeTransitionDuration);
    SCHEMA_FIELD(int32_t, m_nStopType);
    SCHEMA_FIELD(bool, m_bAnimateDuringGameplayPause);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIParticleSystemDefinition>, m_iEffectIndex);
    SCHEMA_FIELD(float, m_flStartTime);
    SCHEMA_FIELD(float, m_flPreSimTime);
    SCHEMA_FIELD_POINTER(Vector, m_vServerControlPoints);
    SCHEMA_FIELD_POINTER(uint8_t, m_iServerControlPointAssignments);
    SCHEMA_FIELD_POINTER(CHandle<CBaseEntity>, m_hControlPointEnts);
    SCHEMA_FIELD(bool, m_bDataStringLocalized);
    SCHEMA_FIELD(CUtlString, m_strDataString);
    SCHEMA_FIELD(bool, m_bNoSave);
    SCHEMA_FIELD(bool, m_bNoFreeze);
    SCHEMA_FIELD(bool, m_bNoRamp);
    SCHEMA_FIELD(bool, m_bStartActive);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszEffectName);
    SCHEMA_FIELD_POINTER(CUtlSymbolLarge, m_iszControlPointNames);
    SCHEMA_FIELD(int32_t, m_nDataCP);
    SCHEMA_FIELD(Vector, m_vecDataCPValue);
    SCHEMA_FIELD(int32_t, m_nTintCP);
    SCHEMA_FIELD(Color, m_clrTint);

public:
    static CParticleSystem* New(const char* className)
    {
        return CBaseEntity::New<CParticleSystem>(className);
    }

    static CParticleSystem* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CParticleSystem>(iIndex);
    }

    static CParticleSystem* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CParticleSystem> GetHandle()
    {
        return CBaseEntity::GetHandle<CParticleSystem>();
    }
};

#endif // _INCLUDE_CPARTICLESYSTEM_H
