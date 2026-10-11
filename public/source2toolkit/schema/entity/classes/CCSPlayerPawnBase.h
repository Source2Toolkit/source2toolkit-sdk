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

#ifndef _INCLUDE_CCSPLAYERPAWNBASE_H
#define _INCLUDE_CCSPLAYERPAWNBASE_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/CSPlayerState.h"
#include "CBaseEntity.h"
#include "CBasePlayerPawn.h"
#include "CTouchExpansionComponent.h"
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

class CCSPlayerController;
class CCSPlayer_PingServices;

class CCSPlayerPawnBase : public CBasePlayerPawn
{
public:
    DECLARE_SCHEMA_CLASS(CCSPlayerPawnBase);

    SCHEMA_FIELD(CTouchExpansionComponent, m_CTouchExpansionComponent);
    SCHEMA_FIELD(CCSPlayer_PingServices*, m_pPingServices);
    SCHEMA_FIELD(float, m_blindUntilTime);
    SCHEMA_FIELD(float, m_blindStartTime);
    SCHEMA_FIELD(CSPlayerState, m_iPlayerState);
    SCHEMA_FIELD(bool, m_bRespawning);
    SCHEMA_FIELD(bool, m_bHasMovedSinceSpawn);
    SCHEMA_FIELD(int32_t, m_iNumSpawns);
    SCHEMA_FIELD(float, m_flIdleTimeSinceLastAction);
    SCHEMA_FIELD(float, m_fNextRadarUpdateTime);
    SCHEMA_FIELD(float, m_flFlashDuration);
    SCHEMA_FIELD(float, m_flFlashMaxAlpha);
    SCHEMA_FIELD(float, m_flProgressBarStartTime);
    SCHEMA_FIELD(int32_t, m_iProgressBarDuration);
    SCHEMA_FIELD(CHandle<CCSPlayerController>, m_hOriginalController);

public:
    /// <summary>Get original controller.</summary>
    CCSPlayerController* GetOriginalController();
    /// <summary>Whether the pawn may move (false while frozen, defusing, ...).</summary>
    bool CanMove();

public:
    static CCSPlayerPawnBase* New(const char* className)
    {
        return CBaseEntity::New<CCSPlayerPawnBase>(className);
    }

    static CCSPlayerPawnBase* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CCSPlayerPawnBase>(iIndex);
    }

    static CCSPlayerPawnBase* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CCSPlayerPawnBase> GetHandle()
    {
        return CBaseEntity::GetHandle<CCSPlayerPawnBase>();
    }
};

#endif // _INCLUDE_CCSPLAYERPAWNBASE_H
