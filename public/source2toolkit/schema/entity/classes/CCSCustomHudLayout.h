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

#ifndef _INCLUDE_CCSCUSTOMHUDLAYOUT_H
#define _INCLUDE_CCSCUSTOMHUDLAYOUT_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CCSCustomHudLayoutState.h"
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

class CCSCustomHudLayout : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CCSCustomHudLayout);

    SCHEMA_FIELD(CUtlSymbolLarge, m_strLayout);
    SCHEMA_FIELD(bool, m_bObservable);
    SCHEMA_FIELD(CUtlVector<CCSCustomHudLayoutState>, m_vecPlayerLayoutStates);
    SCHEMA_FIELD(CCSCustomHudLayoutState, m_globalLayoutState);
    SCHEMA_FIELD(CUtlVector<CUtlString>, m_vecPanelIds);
    SCHEMA_FIELD(CUtlVector<CUtlString>, m_vecClassNames);
    SCHEMA_FIELD(CUtlVector<CUtlString>, m_vecDialogVariableNames);

public:
    /// <summary>Creates and spawns a custom_hud_layout for a panorama layout, e.g. "my_panel" for panorama/layout/custom_game/my_panel.vxml_c.</summary>
    static CCSCustomHudLayout* Create(const char* pszLayout, const char* pszTargetName = nullptr);
    /// <summary>Gets the layout state of one player, or the global state when no controller is given.</summary>
    CCSCustomHudLayoutState& GetLayoutState(CCSPlayerController* pController = nullptr);
    /// <summary>Adds or removes a CSS class on a panel of the layout.</summary>
    void SetHasClass(const char* pszPanelId, const char* pszClassName, bool bHasClass, CCSPlayerController* pController = nullptr);
    /// <summary>Sets a dialog variable string on a panel of the layout.</summary>
    void SetDialogVariableString(const char* pszPanelId, const char* pszVariableName, const char* pszValue, CCSPlayerController* pController = nullptr);
    /// <summary>Enables or disables mouse input capture, without which the player cannot click the layout.</summary>
    void SetInputCaptureEnabled(bool bEnable, CCSPlayerController* pController = nullptr);
    /// <summary>Whether mouse input capture is enabled.</summary>
    bool IsInputCaptureEnabled(CCSPlayerController* pController = nullptr);
    /// <summary>Registers a callback fired when a player clicks a panel of this layout, keyed by the panel's id attribute.</summary>
    void HookClick(std::function<void(CCSPlayerController*, CCSCustomHudLayout*, const char*)> callback);
    /// <summary>Drops every click callback registered for this layout.</summary>
    void UnhookClicks();

public:
    static CCSCustomHudLayout* New(const char* className)
    {
        return CBaseEntity::New<CCSCustomHudLayout>(className);
    }

    static CCSCustomHudLayout* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CCSCustomHudLayout>(iIndex);
    }

    static CCSCustomHudLayout* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CCSCustomHudLayout> GetHandle()
    {
        return CBaseEntity::GetHandle<CCSCustomHudLayout>();
    }
};

#endif // _INCLUDE_CCSCUSTOMHUDLAYOUT_H
