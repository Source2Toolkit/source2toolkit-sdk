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

#ifndef _INCLUDE_CBREAKABLE_H
#define _INCLUDE_CBREAKABLE_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/EOverrideBlockLOS_t.h"
#include "../enums/Explosions.h"
#include "../enums/Materials.h"
#include "../enums/PerformanceMode_t.h"
#include "CBaseEntity.h"
#include "CBaseModelEntity.h"
#include "CPropDataComponent.h"
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
class CBasePlayerPawn;

class CBreakable : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CBreakable);

    SCHEMA_FIELD(CPropDataComponent, m_CPropDataComponent);
    SCHEMA_FIELD(Materials, m_Material);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hBreaker);
    SCHEMA_FIELD(Explosions, m_Explosion);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSpawnObject);
    SCHEMA_FIELD(float, m_flPressureDelay);
    SCHEMA_FIELD(int32_t, m_iMinHealthDmg);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszPropData);
    SCHEMA_FIELD(float, m_impactEnergyScale);
    SCHEMA_FIELD(EOverrideBlockLOS_t, m_nOverrideBlockLOS);
    SCHEMA_FIELD(CEntityIOOutput, m_OnStartDeath);
    SCHEMA_FIELD(CEntityIOOutput, m_OnBreak);
    SCHEMA_FIELD(PerformanceMode_t, m_PerformanceMode);
    SCHEMA_FIELD(CHandle<CBasePlayerPawn>, m_hPhysicsAttacker);
    SCHEMA_FIELD(float, m_flLastPhysicsInfluenceTime);

public:
    static CBreakable* New(const char* className)
    {
        return CBaseEntity::New<CBreakable>(className);
    }

    static CBreakable* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBreakable>(iIndex);
    }

    static CBreakable* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBreakable> GetHandle()
    {
        return CBaseEntity::GetHandle<CBreakable>();
    }
};

#endif // _INCLUDE_CBREAKABLE_H
