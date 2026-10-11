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

#ifndef _INCLUDE_CBASETRIGGER_H
#define _INCLUDE_CBASETRIGGER_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CBaseToggle.h"
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
class CBaseFilter;

class CBaseTrigger : public CBaseToggle
{
public:
    DECLARE_SCHEMA_CLASS(CBaseTrigger);

    SCHEMA_FIELD(CEntityIOOutput, m_OnStartTouch);
    SCHEMA_FIELD(CEntityIOOutput, m_OnStartTouchAll);
    SCHEMA_FIELD(CEntityIOOutput, m_OnEndTouch);
    SCHEMA_FIELD(CEntityIOOutput, m_OnEndTouchAll);
    SCHEMA_FIELD(CEntityIOOutput, m_OnTouching);
    SCHEMA_FIELD(CEntityIOOutput, m_OnTouchingEachEntity);
    SCHEMA_FIELD(CEntityIOOutput, m_OnNotTouching);
    SCHEMA_FIELD(CEntityIOOutput, m_OnTouchingChanged);
    SCHEMA_FIELD(CUtlVector<CHandle<CBaseEntity>>, m_hTouchingEntities);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iFilterName);
    SCHEMA_FIELD(CHandle<CBaseFilter>, m_hFilter);
    SCHEMA_FIELD(bool, m_bDisabled);
    SCHEMA_FIELD(bool, m_bUseAsyncQueries);

public:
    static CBaseTrigger* New(const char* className)
    {
        return CBaseEntity::New<CBaseTrigger>(className);
    }

    static CBaseTrigger* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBaseTrigger>(iIndex);
    }

    static CBaseTrigger* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBaseTrigger> GetHandle()
    {
        return CBaseEntity::GetHandle<CBaseTrigger>();
    }
};

#endif // _INCLUDE_CBASETRIGGER_H
