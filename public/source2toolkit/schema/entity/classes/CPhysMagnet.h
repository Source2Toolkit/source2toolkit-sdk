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

#ifndef _INCLUDE_CPHYSMAGNET_H
#define _INCLUDE_CPHYSMAGNET_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseAnimGraph.h"
#include "CBaseEntity.h"
#include "magnetted_objects_t.h"
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

class CPhysMagnet : public CBaseAnimGraph
{
public:
    DECLARE_SCHEMA_CLASS(CPhysMagnet);

    SCHEMA_FIELD(CEntityIOOutput, m_OnMagnetAttach);
    SCHEMA_FIELD(CEntityIOOutput, m_OnMagnetDetach);
    SCHEMA_FIELD(float, m_massScale);
    SCHEMA_FIELD(float, m_forceLimit);
    SCHEMA_FIELD(float, m_torqueLimit);
    SCHEMA_FIELD(CUtlVector<magnetted_objects_t>, m_MagnettedEntities);
    SCHEMA_FIELD(bool, m_bActive);
    SCHEMA_FIELD(bool, m_bHasHitSomething);
    SCHEMA_FIELD(float, m_flTotalMass);
    SCHEMA_FIELD(float, m_flRadius);
    SCHEMA_FIELD(float, m_flNextSuckTime);
    SCHEMA_FIELD(int32_t, m_iMaxObjectsAttached);

public:
    static CPhysMagnet* New(const char* className)
    {
        return CBaseEntity::New<CPhysMagnet>(className);
    }

    static CPhysMagnet* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPhysMagnet>(iIndex);
    }

    static CPhysMagnet* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPhysMagnet> GetHandle()
    {
        return CBaseEntity::GetHandle<CPhysMagnet>();
    }
};

#endif // _INCLUDE_CPHYSMAGNET_H
