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

#ifndef _INCLUDE_CPHYSCONSTRAINT_H
#define _INCLUDE_CPHYSCONSTRAINT_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CLogicalEntity.h"
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
class IPhysicsJoint;

class CPhysConstraint : public CLogicalEntity
{
public:
    DECLARE_SCHEMA_CLASS(CPhysConstraint);

    SCHEMA_FIELD(IPhysicsJoint*, m_hJoint);
    SCHEMA_FIELD(CUtlSymbolLarge, m_nameAttach1);
    SCHEMA_FIELD(CUtlSymbolLarge, m_nameAttach2);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hAttach1);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hAttach2);
    SCHEMA_FIELD(CUtlSymbolLarge, m_nameAttachment1);
    SCHEMA_FIELD(CUtlSymbolLarge, m_nameAttachment2);
    SCHEMA_FIELD(CGameSoundEventName, m_breakSound);
    SCHEMA_FIELD(float, m_forceLimit);
    SCHEMA_FIELD(float, m_torqueLimit);
    SCHEMA_FIELD(float, m_minTeleportDistance);
    SCHEMA_FIELD(bool, m_bSnapObjectPositions);
    SCHEMA_FIELD(bool, m_bTreatEntity1AsInfiniteMass);
    SCHEMA_FIELD(CEntityIOOutput, m_OnBreak);

public:
    static CPhysConstraint* New(const char* className)
    {
        return CBaseEntity::New<CPhysConstraint>(className);
    }

    static CPhysConstraint* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPhysConstraint>(iIndex);
    }

    static CPhysConstraint* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPhysConstraint> GetHandle()
    {
        return CBaseEntity::GetHandle<CPhysConstraint>();
    }
};

#endif // _INCLUDE_CPHYSCONSTRAINT_H
