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

#ifndef _INCLUDE_CSOUNDOPVARSETPATHCORNERENTITY_H
#define _INCLUDE_CSOUNDOPVARSETPATHCORNERENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CSoundOpvarSetPointEntity.h"
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

class CSoundOpvarSetPathCornerEntity : public CSoundOpvarSetPointEntity
{
public:
    DECLARE_SCHEMA_CLASS(CSoundOpvarSetPathCornerEntity);

    SCHEMA_FIELD(bool, m_bUseParentedPath);
    SCHEMA_FIELD(float, m_flDistMinSqr);
    SCHEMA_FIELD(float, m_flDistMaxSqr);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszPathCornerEntityName);

public:
    static CSoundOpvarSetPathCornerEntity* New(const char* className)
    {
        return CBaseEntity::New<CSoundOpvarSetPathCornerEntity>(className);
    }

    static CSoundOpvarSetPathCornerEntity* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CSoundOpvarSetPathCornerEntity>(iIndex);
    }

    static CSoundOpvarSetPathCornerEntity* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CSoundOpvarSetPathCornerEntity> GetHandle()
    {
        return CBaseEntity::GetHandle<CSoundOpvarSetPathCornerEntity>();
    }
};

#endif // _INCLUDE_CSOUNDOPVARSETPATHCORNERENTITY_H
