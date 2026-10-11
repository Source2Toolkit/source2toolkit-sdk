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

#ifndef _INCLUDE_CINFERNO_H
#define _INCLUDE_CINFERNO_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CBaseModelEntity.h"
#include "CountdownTimer.h"
#include "Extent.h"
#include "IntervalTimer.h"
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

class CInferno : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CInferno);

    SCHEMA_FIELD_POINTER(Vector, m_firePositions);
    SCHEMA_FIELD_POINTER(Vector, m_fireParentPositions);
    SCHEMA_FIELD_POINTER(bool, m_bFireIsBurning);
    SCHEMA_FIELD_POINTER(Vector, m_BurnNormal);
    SCHEMA_FIELD(int32_t, m_fireCount);
    SCHEMA_FIELD(int32_t, m_nInfernoType);
    SCHEMA_FIELD(int32_t, m_nFireEffectTickBegin);
    SCHEMA_FIELD(float, m_nFireLifetime);
    SCHEMA_FIELD(bool, m_bInPostEffectTime);
    SCHEMA_FIELD(bool, m_bWasCreatedInSmoke);
    SCHEMA_FIELD(Extent, m_extent);
    SCHEMA_FIELD(CountdownTimer, m_damageTimer);
    SCHEMA_FIELD(CountdownTimer, m_damageRampTimer);
    SCHEMA_FIELD(Vector, m_splashVelocity);
    SCHEMA_FIELD(Vector, m_InitialSplashVelocity);
    SCHEMA_FIELD(Vector, m_startPos);
    SCHEMA_FIELD(Vector, m_vecOriginalSpawnLocation);
    SCHEMA_FIELD(IntervalTimer, m_activeTimer);
    SCHEMA_FIELD(int32_t, m_fireSpawnOffset);
    SCHEMA_FIELD(int32_t, m_nMaxFlames);
    SCHEMA_FIELD(int32_t, m_nSpreadCount);
    SCHEMA_FIELD(CountdownTimer, m_BookkeepingTimer);
    SCHEMA_FIELD(CountdownTimer, m_NextSpreadTimer);
    SCHEMA_FIELD(uint16_t, m_nSourceItemDefIndex);

public:
    static CInferno* New(const char* className)
    {
        return CBaseEntity::New<CInferno>(className);
    }

    static CInferno* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CInferno>(iIndex);
    }

    static CInferno* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CInferno> GetHandle()
    {
        return CBaseEntity::GetHandle<CInferno>();
    }
};

#endif // _INCLUDE_CINFERNO_H
