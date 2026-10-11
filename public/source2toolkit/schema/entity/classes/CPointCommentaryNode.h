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

#ifndef _INCLUDE_CPOINTCOMMENTARYNODE_H
#define _INCLUDE_CPOINTCOMMENTARYNODE_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseAnimGraph.h"
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

class CBaseEntity;

class CPointCommentaryNode : public CBaseAnimGraph
{
public:
    DECLARE_SCHEMA_CLASS(CPointCommentaryNode);

    SCHEMA_FIELD(CUtlSymbolLarge, m_iszPreCommands);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszPostCommands);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszCommentaryFile);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszViewTarget);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hViewTarget);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hViewTargetAngles);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszViewPosition);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hViewPosition);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hViewPositionMover);
    SCHEMA_FIELD(bool, m_bPreventMovement);
    SCHEMA_FIELD(bool, m_bUnderCrosshair);
    SCHEMA_FIELD(bool, m_bUnstoppable);
    SCHEMA_FIELD(float, m_flFinishedTime);
    SCHEMA_FIELD(Vector, m_vecFinishOrigin);
    SCHEMA_FIELD(QAngle, m_vecOriginalAngles);
    SCHEMA_FIELD(QAngle, m_vecFinishAngles);
    SCHEMA_FIELD(bool, m_bPreventChangesWhileMoving);
    SCHEMA_FIELD(bool, m_bDisabled);
    SCHEMA_FIELD(Vector, m_vecTeleportOrigin);
    SCHEMA_FIELD(float, m_flAbortedPlaybackAt);
    SCHEMA_FIELD(CEntityIOOutput, m_pOnCommentaryStarted);
    SCHEMA_FIELD(CEntityIOOutput, m_pOnCommentaryStopped);
    SCHEMA_FIELD(bool, m_bActive);
    SCHEMA_FIELD(float, m_flStartTime);
    SCHEMA_FIELD(float, m_flStartTimeInCommentary);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszTitle);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszSpeakers);
    SCHEMA_FIELD(int32_t, m_iNodeNumber);
    SCHEMA_FIELD(int32_t, m_iNodeNumberMax);
    SCHEMA_FIELD(bool, m_bListenedTo);

public:
    static CPointCommentaryNode* New(const char* className)
    {
        return CBaseEntity::New<CPointCommentaryNode>(className);
    }

    static CPointCommentaryNode* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPointCommentaryNode>(iIndex);
    }

    static CPointCommentaryNode* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPointCommentaryNode> GetHandle()
    {
        return CBaseEntity::GetHandle<CPointCommentaryNode>();
    }
};

#endif // _INCLUDE_CPOINTCOMMENTARYNODE_H
