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

#ifndef _INCLUDE_CBASEGRENADE_H
#define _INCLUDE_CBASEGRENADE_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseAnimGraph.h"
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

class CCSPlayerPawn;

class CBaseGrenade : public CBaseAnimGraph
{
public:
    DECLARE_SCHEMA_CLASS(CBaseGrenade);

    SCHEMA_FIELD(bool, m_bDamageDetonating);
    SCHEMA_FIELD(CEntityIOOutput, m_OnPlayerPickup);
    SCHEMA_FIELD(CEntityIOOutput, m_OnExplode);
    SCHEMA_FIELD(bool, m_bHasWarnedAI);
    SCHEMA_FIELD(bool, m_bIsSmokeGrenade);
    SCHEMA_FIELD(bool, m_bIsLive);
    SCHEMA_FIELD(float, m_DmgRadius);
    SCHEMA_FIELD(float, m_flDetonateTime);
    SCHEMA_FIELD(float, m_flWarnAITime);
    SCHEMA_FIELD(float, m_flDamage);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszBounceSound);
    SCHEMA_FIELD(CUtlString, m_ExplosionSound);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_hThrower);
    SCHEMA_FIELD(float, m_flNextAttack);
    SCHEMA_FIELD(CHandle<CCSPlayerPawn>, m_hOriginalThrower);

public:
    static CBaseGrenade* New(const char* className)
    {
        return CBaseEntity::New<CBaseGrenade>(className);
    }

    static CBaseGrenade* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBaseGrenade>(iIndex);
    }

    static CBaseGrenade* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBaseGrenade> GetHandle()
    {
        return CBaseEntity::GetHandle<CBaseGrenade>();
    }
};

#endif // _INCLUDE_CBASEGRENADE_H
