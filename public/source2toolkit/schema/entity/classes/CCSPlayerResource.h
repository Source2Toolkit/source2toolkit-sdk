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

#ifndef _INCLUDE_CCSPLAYERRESOURCE_H
#define _INCLUDE_CCSPLAYERRESOURCE_H
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

class CCSPlayerResource : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CCSPlayerResource);

    SCHEMA_FIELD_POINTER(bool, m_bHostageAlive);
    SCHEMA_FIELD_POINTER(bool, m_isHostageFollowingSomeone);
    SCHEMA_FIELD_POINTER(CEntityIndex, m_iHostageEntityIDs);
    SCHEMA_FIELD(Vector, m_bombsiteCenterA);
    SCHEMA_FIELD(Vector, m_bombsiteCenterB);
    SCHEMA_FIELD_POINTER(int32_t, m_hostageRescueX);
    SCHEMA_FIELD_POINTER(int32_t, m_hostageRescueY);
    SCHEMA_FIELD_POINTER(int32_t, m_hostageRescueZ);
    SCHEMA_FIELD(bool, m_bEndMatchNextMapAllVoted);
    SCHEMA_FIELD(bool, m_foundGoalPositions);

public:
    static CCSPlayerResource* New(const char* className)
    {
        return CBaseEntity::New<CCSPlayerResource>(className);
    }

    static CCSPlayerResource* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CCSPlayerResource>(iIndex);
    }

    static CCSPlayerResource* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CCSPlayerResource> GetHandle()
    {
        return CBaseEntity::GetHandle<CCSPlayerResource>();
    }
};

#endif // _INCLUDE_CCSPLAYERRESOURCE_H
