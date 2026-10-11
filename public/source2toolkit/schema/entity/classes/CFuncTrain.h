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

#ifndef _INCLUDE_CFUNCTRAIN_H
#define _INCLUDE_CFUNCTRAIN_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CBasePlatTrain.h"
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

class CFuncTrain : public CBasePlatTrain
{
public:
    DECLARE_SCHEMA_CLASS(CFuncTrain);

    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hCurrentTarget);
    SCHEMA_FIELD(bool, m_activated);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hEnemy);
    SCHEMA_FIELD(float, m_flBlockDamage);
    SCHEMA_FIELD(float, m_flNextBlockTime);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszLastTarget);
    SCHEMA_FIELD(float, m_flSpeed);

public:
    static CFuncTrain* New(const char* className)
    {
        return CBaseEntity::New<CFuncTrain>(className);
    }

    static CFuncTrain* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CFuncTrain>(iIndex);
    }

    static CFuncTrain* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CFuncTrain> GetHandle()
    {
        return CBaseEntity::GetHandle<CFuncTrain>();
    }
};

#endif // _INCLUDE_CFUNCTRAIN_H
