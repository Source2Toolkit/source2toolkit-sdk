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

#ifndef _INCLUDE_CFISH_H
#define _INCLUDE_CFISH_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseAnimGraph.h"
#include "CBaseEntity.h"
#include "CountdownTimer.h"
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

class CFishPool;

class CFish : public CBaseAnimGraph
{
public:
    DECLARE_SCHEMA_CLASS(CFish);

    SCHEMA_FIELD(CHandle<CFishPool>, m_pool);
    SCHEMA_FIELD(uint32_t, m_id);
    SCHEMA_FIELD(float, m_x);
    SCHEMA_FIELD(float, m_y);
    SCHEMA_FIELD(float, m_z);
    SCHEMA_FIELD(float, m_angle);
    SCHEMA_FIELD(float, m_angleChange);
    SCHEMA_FIELD(Vector, m_forward);
    SCHEMA_FIELD(Vector, m_perp);
    SCHEMA_FIELD(Vector, m_poolOrigin);
    SCHEMA_FIELD(float, m_waterLevel);
    SCHEMA_FIELD(float, m_speed);
    SCHEMA_FIELD(float, m_desiredSpeed);
    SCHEMA_FIELD(float, m_calmSpeed);
    SCHEMA_FIELD(float, m_panicSpeed);
    SCHEMA_FIELD(float, m_avoidRange);
    SCHEMA_FIELD(CountdownTimer, m_turnTimer);
    SCHEMA_FIELD(bool, m_turnClockwise);
    SCHEMA_FIELD(CountdownTimer, m_goTimer);
    SCHEMA_FIELD(CountdownTimer, m_moveTimer);
    SCHEMA_FIELD(CountdownTimer, m_panicTimer);
    SCHEMA_FIELD(CountdownTimer, m_disperseTimer);
    SCHEMA_FIELD(CountdownTimer, m_proximityTimer);
    SCHEMA_FIELD(CUtlVector<CFish*>, m_visible);

public:
    static CFish* New(const char* className)
    {
        return CBaseEntity::New<CFish>(className);
    }

    static CFish* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CFish>(iIndex);
    }

    static CFish* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CFish> GetHandle()
    {
        return CBaseEntity::GetHandle<CFish>();
    }
};

#endif // _INCLUDE_CFISH_H
