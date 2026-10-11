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

#ifndef _INCLUDE_CPROPDOORROTATING_H
#define _INCLUDE_CPROPDOORROTATING_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/doorCheck_e.h"
#include "../enums/PropDoorRotatingOpenDirection_e.h"
#include "../enums/PropDoorRotatingSpawnPos_t.h"
#include "CBaseEntity.h"
#include "CBasePropDoor.h"
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

class CEntityBlocker;

class CPropDoorRotating : public CBasePropDoor
{
public:
    DECLARE_SCHEMA_CLASS(CPropDoorRotating);

    SCHEMA_FIELD(Vector, m_vecAxis);
    SCHEMA_FIELD(float, m_flDistance);
    SCHEMA_FIELD(PropDoorRotatingSpawnPos_t, m_eSpawnPosition);
    SCHEMA_FIELD(PropDoorRotatingOpenDirection_e, m_eOpenDirection);
    SCHEMA_FIELD(PropDoorRotatingOpenDirection_e, m_eCurrentOpenDirection);
    SCHEMA_FIELD(doorCheck_e, m_eDefaultCheckDirection);
    SCHEMA_FIELD(float, m_flAjarAngle);
    SCHEMA_FIELD(QAngle, m_angRotationAjarDeprecated);
    SCHEMA_FIELD(QAngle, m_angRotationClosed);
    SCHEMA_FIELD(QAngle, m_angRotationOpenForward);
    SCHEMA_FIELD(QAngle, m_angRotationOpenBack);
    SCHEMA_FIELD(QAngle, m_angGoal);
    SCHEMA_FIELD(Vector, m_vecForwardBoundsMin);
    SCHEMA_FIELD(Vector, m_vecForwardBoundsMax);
    SCHEMA_FIELD(Vector, m_vecBackBoundsMin);
    SCHEMA_FIELD(Vector, m_vecBackBoundsMax);
    SCHEMA_FIELD(bool, m_bAjarDoorShouldntAlwaysOpen);
    SCHEMA_FIELD(CHandle<CEntityBlocker>, m_hEntityBlocker);

public:
    static CPropDoorRotating* New(const char* className)
    {
        return CBaseEntity::New<CPropDoorRotating>(className);
    }

    static CPropDoorRotating* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPropDoorRotating>(iIndex);
    }

    static CPropDoorRotating* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPropDoorRotating> GetHandle()
    {
        return CBaseEntity::GetHandle<CPropDoorRotating>();
    }
};

#endif // _INCLUDE_CPROPDOORROTATING_H
