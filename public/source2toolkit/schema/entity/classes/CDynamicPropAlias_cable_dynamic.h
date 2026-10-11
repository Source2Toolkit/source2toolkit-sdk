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

#ifndef _INCLUDE_CDYNAMICPROPALIAS_CABLE_DYNAMIC_H
#define _INCLUDE_CDYNAMICPROPALIAS_CABLE_DYNAMIC_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CDynamicProp.h"
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

class CDynamicPropAlias_cable_dynamic : public CDynamicProp
{
public:
    DECLARE_SCHEMA_CLASS(CDynamicPropAlias_cable_dynamic);


public:
    static CDynamicPropAlias_cable_dynamic* New(const char* className)
    {
        return CBaseEntity::New<CDynamicPropAlias_cable_dynamic>(className);
    }

    static CDynamicPropAlias_cable_dynamic* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CDynamicPropAlias_cable_dynamic>(iIndex);
    }

    static CDynamicPropAlias_cable_dynamic* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CDynamicPropAlias_cable_dynamic> GetHandle()
    {
        return CBaseEntity::GetHandle<CDynamicPropAlias_cable_dynamic>();
    }
};

#endif // _INCLUDE_CDYNAMICPROPALIAS_CABLE_DYNAMIC_H
