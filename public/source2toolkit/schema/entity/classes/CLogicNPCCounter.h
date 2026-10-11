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

#ifndef _INCLUDE_CLOGICNPCCOUNTER_H
#define _INCLUDE_CLOGICNPCCOUNTER_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
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

class CLogicNPCCounter : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CLogicNPCCounter);

    SCHEMA_FIELD(CEntityIOOutput, m_OnMinCountAll);
    SCHEMA_FIELD(CEntityIOOutput, m_OnMaxCountAll);
    SCHEMA_FIELD(CEntityIOOutput, m_OnMinCount_1);
    SCHEMA_FIELD(CEntityIOOutput, m_OnMaxCount_1);
    SCHEMA_FIELD(CEntityIOOutput, m_OnMinCount_2);
    SCHEMA_FIELD(CEntityIOOutput, m_OnMaxCount_2);
    SCHEMA_FIELD(CEntityIOOutput, m_OnMinCount_3);
    SCHEMA_FIELD(CEntityIOOutput, m_OnMaxCount_3);
    SCHEMA_FIELD(CEntityHandle, m_hSource);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSourceEntityName);
    SCHEMA_FIELD(float, m_flDistanceMax);
    SCHEMA_FIELD(bool, m_bDisabled);
    SCHEMA_FIELD(int32_t, m_nMinCountAll);
    SCHEMA_FIELD(int32_t, m_nMaxCountAll);
    SCHEMA_FIELD(int32_t, m_nMinFactorAll);
    SCHEMA_FIELD(int32_t, m_nMaxFactorAll);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszNPCClassname_1);
    SCHEMA_FIELD(int32_t, m_nNPCState_1);
    SCHEMA_FIELD(bool, m_bInvertState_1);
    SCHEMA_FIELD(int32_t, m_nMinCount_1);
    SCHEMA_FIELD(int32_t, m_nMaxCount_1);
    SCHEMA_FIELD(int32_t, m_nMinFactor_1);
    SCHEMA_FIELD(int32_t, m_nMaxFactor_1);
    SCHEMA_FIELD(float, m_flDefaultDist_1);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszNPCClassname_2);
    SCHEMA_FIELD(int32_t, m_nNPCState_2);
    SCHEMA_FIELD(bool, m_bInvertState_2);
    SCHEMA_FIELD(int32_t, m_nMinCount_2);
    SCHEMA_FIELD(int32_t, m_nMaxCount_2);
    SCHEMA_FIELD(int32_t, m_nMinFactor_2);
    SCHEMA_FIELD(int32_t, m_nMaxFactor_2);
    SCHEMA_FIELD(float, m_flDefaultDist_2);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszNPCClassname_3);
    SCHEMA_FIELD(int32_t, m_nNPCState_3);
    SCHEMA_FIELD(bool, m_bInvertState_3);
    SCHEMA_FIELD(int32_t, m_nMinCount_3);
    SCHEMA_FIELD(int32_t, m_nMaxCount_3);
    SCHEMA_FIELD(int32_t, m_nMinFactor_3);
    SCHEMA_FIELD(int32_t, m_nMaxFactor_3);
    SCHEMA_FIELD(float, m_flDefaultDist_3);

public:
    static CLogicNPCCounter* New(const char* className)
    {
        return CBaseEntity::New<CLogicNPCCounter>(className);
    }

    static CLogicNPCCounter* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CLogicNPCCounter>(iIndex);
    }

    static CLogicNPCCounter* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CLogicNPCCounter> GetHandle()
    {
        return CBaseEntity::GetHandle<CLogicNPCCounter>();
    }
};

#endif // _INCLUDE_CLOGICNPCCOUNTER_H
