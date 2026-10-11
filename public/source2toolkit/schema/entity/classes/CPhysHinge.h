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

#ifndef _INCLUDE_CPHYSHINGE_H
#define _INCLUDE_CPHYSHINGE_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "constraint_hingeparams_t.h"
#include "ConstraintSoundInfo.h"
#include "CPhysConstraint.h"
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

class CPhysHinge : public CPhysConstraint
{
public:
    DECLARE_SCHEMA_CLASS(CPhysHinge);

    SCHEMA_FIELD(ConstraintSoundInfo, m_soundInfo);
    SCHEMA_FIELD(CEntityIOOutput, m_NotifyMinLimitReached);
    SCHEMA_FIELD(CEntityIOOutput, m_NotifyMaxLimitReached);
    SCHEMA_FIELD(bool, m_bAtMinLimit);
    SCHEMA_FIELD(bool, m_bAtMaxLimit);
    SCHEMA_FIELD(constraint_hingeparams_t, m_hinge);
    SCHEMA_FIELD(float, m_hingeFriction);
    SCHEMA_FIELD(float, m_systemLoadScale);
    SCHEMA_FIELD(bool, m_bIsAxisLocal);
    SCHEMA_FIELD(float, m_flMinRotation);
    SCHEMA_FIELD(float, m_flMaxRotation);
    SCHEMA_FIELD(float, m_flInitialRotation);
    SCHEMA_FIELD(float, m_flMotorFrequency);
    SCHEMA_FIELD(float, m_flMotorDampingRatio);
    SCHEMA_FIELD(float, m_flAngleSpeed);
    SCHEMA_FIELD(float, m_flAngleSpeedThreshold);
    SCHEMA_FIELD(float, m_flLimitsDebugVisRotation);
    SCHEMA_FIELD(CEntityIOOutput, m_OnStartMoving);
    SCHEMA_FIELD(CEntityIOOutput, m_OnStopMoving);

public:
    static CPhysHinge* New(const char* className)
    {
        return CBaseEntity::New<CPhysHinge>(className);
    }

    static CPhysHinge* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPhysHinge>(iIndex);
    }

    static CPhysHinge* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPhysHinge> GetHandle()
    {
        return CBaseEntity::GetHandle<CPhysHinge>();
    }
};

#endif // _INCLUDE_CPHYSHINGE_H
