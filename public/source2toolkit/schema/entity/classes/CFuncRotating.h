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

#ifndef _INCLUDE_CFUNCROTATING_H
#define _INCLUDE_CFUNCROTATING_H
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

class CFuncRotating : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CFuncRotating);

    SCHEMA_FIELD(CEntityIOOutput, m_OnStopped);
    SCHEMA_FIELD(CEntityIOOutput, m_OnStarted);
    SCHEMA_FIELD(CEntityIOOutput, m_OnReachedStart);
    SCHEMA_FIELD(RotationVector, m_localRotationVector);
    SCHEMA_FIELD(float, m_flSpeed);
    SCHEMA_FIELD(float, m_flFanFriction);
    SCHEMA_FIELD(float, m_flAttenuation);
    SCHEMA_FIELD(float, m_flVolume);
    SCHEMA_FIELD(float, m_flTargetSpeed);
    SCHEMA_FIELD(float, m_flMaxSpeed);
    SCHEMA_FIELD(float, m_flBlockDamage);
    SCHEMA_FIELD(CGameSoundEventName, m_NoiseRunning);
    SCHEMA_FIELD(bool, m_bReversed);
    SCHEMA_FIELD(bool, m_bAccelDecel);
    SCHEMA_FIELD(QAngle, m_prevLocalAngles);
    SCHEMA_FIELD(QAngle, m_angStart);
    SCHEMA_FIELD(bool, m_bStopAtStartPos);
    SCHEMA_FIELD(Vector, m_vecClientOrigin);
    SCHEMA_FIELD(QAngle, m_vecClientAngles);

public:
    static CFuncRotating* New(const char* className)
    {
        return CBaseEntity::New<CFuncRotating>(className);
    }

    static CFuncRotating* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CFuncRotating>(iIndex);
    }

    static CFuncRotating* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CFuncRotating> GetHandle()
    {
        return CBaseEntity::GetHandle<CFuncRotating>();
    }
};

#endif // _INCLUDE_CFUNCROTATING_H
