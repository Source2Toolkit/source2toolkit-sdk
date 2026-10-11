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

#ifndef _INCLUDE_CPOINTORIENT_H
#define _INCLUDE_CPOINTORIENT_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/PointOrientConstraint_t.h"
#include "../enums/PointOrientGoalDirectionType_t.h"
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

class CPointOrient : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CPointOrient);

    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSpawnTargetName);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hTarget);
    SCHEMA_FIELD(bool, m_bActive);
    SCHEMA_FIELD(PointOrientGoalDirectionType_t, m_nGoalDirection);
    SCHEMA_FIELD(PointOrientConstraint_t, m_nConstraint);
    SCHEMA_FIELD(float, m_flMaxTurnRate);
    SCHEMA_FIELD(float, m_flLastGameTime);

public:
    static CPointOrient* New(const char* className)
    {
        return CBaseEntity::New<CPointOrient>(className);
    }

    static CPointOrient* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPointOrient>(iIndex);
    }

    static CPointOrient* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPointOrient> GetHandle()
    {
        return CBaseEntity::GetHandle<CPointOrient>();
    }
};

#endif // _INCLUDE_CPOINTORIENT_H
