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

#ifndef _INCLUDE_CPOINTVALUEREMAPPER_H
#define _INCLUDE_CPOINTVALUEREMAPPER_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/ValueRemapperHapticsType_t.h"
#include "../enums/ValueRemapperInputType_t.h"
#include "../enums/ValueRemapperMomentumType_t.h"
#include "../enums/ValueRemapperOutputType_t.h"
#include "../enums/ValueRemapperRatchetType_t.h"
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

class CBasePlayerPawn;

class CPointValueRemapper : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CPointValueRemapper);

    SCHEMA_FIELD(bool, m_bDisabled);
    SCHEMA_FIELD(bool, m_bUpdateOnClient);
    SCHEMA_FIELD(ValueRemapperInputType_t, m_nInputType);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszRemapLineStartName);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszRemapLineEndName);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hRemapLineStart);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hRemapLineEnd);
    SCHEMA_FIELD(float, m_flMaximumChangePerSecond);
    SCHEMA_FIELD(float, m_flDisengageDistance);
    SCHEMA_FIELD(float, m_flEngageDistance);
    SCHEMA_FIELD(bool, m_bRequiresUseKey);
    SCHEMA_FIELD(ValueRemapperOutputType_t, m_nOutputType);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszOutputEntityName);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszOutputEntity2Name);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszOutputEntity3Name);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszOutputEntity4Name);
    SCHEMA_FIELD(CUtlVector<CHandle<CBaseEntity>>, m_hOutputEntities);
    SCHEMA_FIELD(ValueRemapperHapticsType_t, m_nHapticsType);
    SCHEMA_FIELD(ValueRemapperMomentumType_t, m_nMomentumType);
    SCHEMA_FIELD(float, m_flMomentumModifier);
    SCHEMA_FIELD(float, m_flSnapValue);
    SCHEMA_FIELD(float, m_flCurrentMomentum);
    SCHEMA_FIELD(ValueRemapperRatchetType_t, m_nRatchetType);
    SCHEMA_FIELD(float, m_flRatchetOffset);
    SCHEMA_FIELD(float, m_flInputOffset);
    SCHEMA_FIELD(bool, m_bEngaged);
    SCHEMA_FIELD(bool, m_bFirstUpdate);
    SCHEMA_FIELD(float, m_flPreviousValue);
    SCHEMA_FIELD(float, m_flPreviousUpdateTickTime);
    SCHEMA_FIELD(Vector, m_vecPreviousTestPoint);
    SCHEMA_FIELD(CHandle<CBasePlayerPawn>, m_hUsingPlayer);
    SCHEMA_FIELD(float, m_flCustomOutputValue);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSoundEngage);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSoundDisengage);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSoundReachedValueZero);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSoundReachedValueOne);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSoundMovingLoop);
    SCHEMA_FIELD(CEntityIOOutput, m_OnReachedValueZero);
    SCHEMA_FIELD(CEntityIOOutput, m_OnReachedValueOne);
    SCHEMA_FIELD(CEntityIOOutput, m_OnReachedValueCustom);
    SCHEMA_FIELD(CEntityIOOutput, m_OnEngage);
    SCHEMA_FIELD(CEntityIOOutput, m_OnDisengage);

public:
    static CPointValueRemapper* New(const char* className)
    {
        return CBaseEntity::New<CPointValueRemapper>(className);
    }

    static CPointValueRemapper* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPointValueRemapper>(iIndex);
    }

    static CPointValueRemapper* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPointValueRemapper> GetHandle()
    {
        return CBaseEntity::GetHandle<CPointValueRemapper>();
    }
};

#endif // _INCLUDE_CPOINTVALUEREMAPPER_H
