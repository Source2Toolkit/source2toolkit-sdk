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

#ifndef _INCLUDE_CGENERICCONSTRAINT_H
#define _INCLUDE_CGENERICCONSTRAINT_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/JointMotion_t.h"
#include "CBaseEntity.h"
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

class CGenericConstraint : public CPhysConstraint
{
public:
    DECLARE_SCHEMA_CLASS(CGenericConstraint);

    SCHEMA_FIELD(bool, m_bPlaceAnchorsAtConstraintTransform);
    SCHEMA_FIELD(JointMotion_t, m_nLinearMotionX);
    SCHEMA_FIELD(JointMotion_t, m_nLinearMotionY);
    SCHEMA_FIELD(JointMotion_t, m_nLinearMotionZ);
    SCHEMA_FIELD(float, m_flLinearFrequencyX);
    SCHEMA_FIELD(float, m_flLinearFrequencyY);
    SCHEMA_FIELD(float, m_flLinearFrequencyZ);
    SCHEMA_FIELD(float, m_flLinearDampingRatioX);
    SCHEMA_FIELD(float, m_flLinearDampingRatioY);
    SCHEMA_FIELD(float, m_flLinearDampingRatioZ);
    SCHEMA_FIELD(float, m_flMaxLinearImpulseX);
    SCHEMA_FIELD(float, m_flMaxLinearImpulseY);
    SCHEMA_FIELD(float, m_flMaxLinearImpulseZ);
    SCHEMA_FIELD(float, m_flBreakAfterTimeX);
    SCHEMA_FIELD(float, m_flBreakAfterTimeY);
    SCHEMA_FIELD(float, m_flBreakAfterTimeZ);
    SCHEMA_FIELD(float, m_flBreakAfterTimeStartTimeX);
    SCHEMA_FIELD(float, m_flBreakAfterTimeStartTimeY);
    SCHEMA_FIELD(float, m_flBreakAfterTimeStartTimeZ);
    SCHEMA_FIELD(float, m_flBreakAfterTimeThresholdX);
    SCHEMA_FIELD(float, m_flBreakAfterTimeThresholdY);
    SCHEMA_FIELD(float, m_flBreakAfterTimeThresholdZ);
    SCHEMA_FIELD(float, m_flNotifyForceX);
    SCHEMA_FIELD(float, m_flNotifyForceY);
    SCHEMA_FIELD(float, m_flNotifyForceZ);
    SCHEMA_FIELD(float, m_flNotifyForceMinTimeX);
    SCHEMA_FIELD(float, m_flNotifyForceMinTimeY);
    SCHEMA_FIELD(float, m_flNotifyForceMinTimeZ);
    SCHEMA_FIELD(float, m_flNotifyForceLastTimeX);
    SCHEMA_FIELD(float, m_flNotifyForceLastTimeY);
    SCHEMA_FIELD(float, m_flNotifyForceLastTimeZ);
    SCHEMA_FIELD(bool, m_bAxisNotifiedX);
    SCHEMA_FIELD(bool, m_bAxisNotifiedY);
    SCHEMA_FIELD(bool, m_bAxisNotifiedZ);
    SCHEMA_FIELD(JointMotion_t, m_nAngularMotionX);
    SCHEMA_FIELD(JointMotion_t, m_nAngularMotionY);
    SCHEMA_FIELD(JointMotion_t, m_nAngularMotionZ);
    SCHEMA_FIELD(float, m_flAngularFrequencyX);
    SCHEMA_FIELD(float, m_flAngularFrequencyY);
    SCHEMA_FIELD(float, m_flAngularFrequencyZ);
    SCHEMA_FIELD(float, m_flAngularDampingRatioX);
    SCHEMA_FIELD(float, m_flAngularDampingRatioY);
    SCHEMA_FIELD(float, m_flAngularDampingRatioZ);
    SCHEMA_FIELD(float, m_flMaxAngularImpulseX);
    SCHEMA_FIELD(float, m_flMaxAngularImpulseY);
    SCHEMA_FIELD(float, m_flMaxAngularImpulseZ);
    SCHEMA_FIELD(CEntityIOOutput, m_NotifyForceReachedX);
    SCHEMA_FIELD(CEntityIOOutput, m_NotifyForceReachedY);
    SCHEMA_FIELD(CEntityIOOutput, m_NotifyForceReachedZ);

public:
    static CGenericConstraint* New(const char* className)
    {
        return CBaseEntity::New<CGenericConstraint>(className);
    }

    static CGenericConstraint* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CGenericConstraint>(iIndex);
    }

    static CGenericConstraint* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CGenericConstraint> GetHandle()
    {
        return CBaseEntity::GetHandle<CGenericConstraint>();
    }
};

#endif // _INCLUDE_CGENERICCONSTRAINT_H
