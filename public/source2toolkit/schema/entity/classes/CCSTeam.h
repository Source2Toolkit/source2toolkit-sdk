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

#ifndef _INCLUDE_CCSTEAM_H
#define _INCLUDE_CCSTEAM_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CTeam.h"
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

class CCSTeam : public CTeam
{
public:
    DECLARE_SCHEMA_CLASS(CCSTeam);

    SCHEMA_FIELD(int32_t, m_nLastRecievedShorthandedRoundBonus);
    SCHEMA_FIELD(int32_t, m_nShorthandedRoundBonusStartRound);
    SCHEMA_FIELD(bool, m_bSurrendered);
    SCHEMA_FIELD_POINTER(char, m_szTeamMatchStat);
    SCHEMA_FIELD(int32_t, m_numMapVictories);
    SCHEMA_FIELD(int32_t, m_scoreFirstHalf);
    SCHEMA_FIELD(int32_t, m_scoreSecondHalf);
    SCHEMA_FIELD(int32_t, m_scoreOvertime);
    SCHEMA_FIELD_POINTER(char, m_szClanTeamname);
    SCHEMA_FIELD(uint32_t, m_iClanID);
    SCHEMA_FIELD_POINTER(char, m_szTeamFlagImage);
    SCHEMA_FIELD_POINTER(char, m_szTeamLogoImage);
    SCHEMA_FIELD(float, m_flNextResourceTime);
    SCHEMA_FIELD(int32_t, m_iLastUpdateSentAt);

public:
    static CCSTeam* New(const char* className)
    {
        return CBaseEntity::New<CCSTeam>(className);
    }

    static CCSTeam* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CCSTeam>(iIndex);
    }

    static CCSTeam* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CCSTeam> GetHandle()
    {
        return CBaseEntity::GetHandle<CCSTeam>();
    }
};

#endif // _INCLUDE_CCSTEAM_H
