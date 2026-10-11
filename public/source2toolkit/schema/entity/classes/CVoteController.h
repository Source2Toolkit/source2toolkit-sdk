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

#ifndef _INCLUDE_CVOTECONTROLLER_H
#define _INCLUDE_CVOTECONTROLLER_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CountdownTimer.h"
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

class CBaseIssue;

class CVoteController : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CVoteController);

    SCHEMA_FIELD(int32_t, m_iActiveIssueIndex);
    SCHEMA_FIELD(int32_t, m_iOnlyTeamToVote);
    SCHEMA_FIELD_POINTER(int32_t, m_nVoteOptionCount);
    SCHEMA_FIELD(int32_t, m_nPotentialVotes);
    SCHEMA_FIELD(bool, m_bIsYesNoVote);
    SCHEMA_FIELD(CountdownTimer, m_acceptingVotesTimer);
    SCHEMA_FIELD(CountdownTimer, m_executeCommandTimer);
    SCHEMA_FIELD(CountdownTimer, m_resetVoteTimer);
    SCHEMA_FIELD_POINTER(int32_t, m_nVotesCast);
    SCHEMA_FIELD(int32_t, m_playerHoldingVote);
    SCHEMA_FIELD(int32_t, m_playerOverrideForVote);
    SCHEMA_FIELD(int32_t, m_nHighestCountIndex);
    SCHEMA_FIELD(CUtlVector<CBaseIssue*>, m_potentialIssues);
    SCHEMA_FIELD(CUtlVector<char*>, m_VoteOptions);

public:
    static CVoteController* New(const char* className)
    {
        return CBaseEntity::New<CVoteController>(className);
    }

    static CVoteController* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CVoteController>(iIndex);
    }

    static CVoteController* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CVoteController> GetHandle()
    {
        return CBaseEntity::GetHandle<CVoteController>();
    }
};

#endif // _INCLUDE_CVOTECONTROLLER_H
