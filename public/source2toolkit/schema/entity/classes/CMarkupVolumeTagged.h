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

#ifndef _INCLUDE_CMARKUPVOLUMETAGGED_H
#define _INCLUDE_CMARKUPVOLUMETAGGED_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CMarkupVolume.h"
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

class CMarkupVolumeTagged : public CMarkupVolume
{
public:
    DECLARE_SCHEMA_CLASS(CMarkupVolumeTagged);

    SCHEMA_FIELD(CUtlVector<CGlobalSymbol>, m_GroupNames);
    SCHEMA_FIELD(CUtlVector<CGlobalSymbol>, m_Tags);
    SCHEMA_FIELD(bool, m_bIsGroup);
    SCHEMA_FIELD(bool, m_bGroupByPrefab);
    SCHEMA_FIELD(bool, m_bGroupByVolume);
    SCHEMA_FIELD(bool, m_bGroupOtherGroups);
    SCHEMA_FIELD(bool, m_bIsInGroup);

public:
    static CMarkupVolumeTagged* New(const char* className)
    {
        return CBaseEntity::New<CMarkupVolumeTagged>(className);
    }

    static CMarkupVolumeTagged* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CMarkupVolumeTagged>(iIndex);
    }

    static CMarkupVolumeTagged* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CMarkupVolumeTagged> GetHandle()
    {
        return CBaseEntity::GetHandle<CMarkupVolumeTagged>();
    }
};

#endif // _INCLUDE_CMARKUPVOLUMETAGGED_H
