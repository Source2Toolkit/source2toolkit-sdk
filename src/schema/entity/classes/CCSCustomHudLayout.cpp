/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
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
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
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
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

#include "source2toolkit/schema/entity/classes/CCSCustomHudLayout.h"

#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitCustomHud.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entity/classes/CCSPlayerController.h"

#include <entitykeyvalues.h>

#include <string>
TOOLKIT_GLOBALVARS();

namespace
{
    /// Panorama resolves layouts out of this directory; callers name the layout,
    /// not the path.
    constexpr const char* LAYOUT_PATH_PREFIX = "panorama/layout/custom_game/";
    constexpr const char* LAYOUT_PATH_SUFFIX = ".vxml_c";

    /// Index of a string in one of the layout's string tables, appending it if
    /// it is not there yet. The per-player states store these indices, not the
    /// strings themselves, so a new string has to reach the clients with them:
    /// without the table's own state change a client gets a state pointing past
    /// the end of its copy of the table.
    template <typename Field>
    int InternString(Field& field, const char* pszValue)
    {
        CUtlVector<CUtlString>& vecTable = field();
        int index = vecTable.Find(pszValue);

        if (index == -1)
        {
            index = vecTable.AddToTail(pszValue);
            field.NetworkStateChanged();
        }

        return index;
    }
}

CCSCustomHudLayout* CCSCustomHudLayout::Create(const char* pszLayout, const char* pszTargetName)
{
    if (!pszLayout || !*pszLayout)
        return nullptr;

    CCSCustomHudLayout* pLayout = CCSCustomHudLayout::New("custom_hud_layout");

    if (!pLayout)
        return nullptr;

    const std::string sLayoutPath = LAYOUT_PATH_PREFIX + std::string(pszLayout) + LAYOUT_PATH_SUFFIX;

    auto* pKeyValues = new CEntityKeyValues();
    pKeyValues->SetString("layout", sLayoutPath.c_str());

    if (pszTargetName && *pszTargetName)
        pKeyValues->SetString("targetname", pszTargetName);

    pLayout->DispatchSpawn(pKeyValues);
    return pLayout;
}

// A player's state is an element of m_vecPlayerLayoutStates, embedded without a NetworkVar of its own: the
// state change its fields raise finds no slot and goes nowhere, so the entity's field carries it.
void CCSCustomHudLayout::StateChanged(CCSPlayerController* pController)
{
    if (pController)
        m_vecPlayerLayoutStates.NetworkStateChanged();
    else
        m_globalLayoutState.NetworkStateChanged();
}

CCSCustomHudLayoutState& CCSCustomHudLayout::GetLayoutState(CCSPlayerController* pController)
{
    if (!pController)
        return m_globalLayoutState();

    // m_vecPlayerLayoutStates is a CUtlVectorEmbeddedNetworkVar, not the plain
    // CUtlVector its declaration suggests, so indexing it directly walks the
    // wrong memory. The schema system publishes an element accessor for exactly
    // this case; go through it.
    const SchemaCollectionManipulatorFn_t pfnManipulator = m_vecPlayerLayoutStates.GetManipulator();
    if (!pfnManipulator)
        return m_globalLayoutState();

    void* pElement = pfnManipulator(SCHEMA_COLLECTION_MANIPULATOR_ACTION_GET_ELEMENT,
                                    &m_vecPlayerLayoutStates(),
                                    pController->GetPlayerSlot().Get(), 0);
    if (!pElement)
        return m_globalLayoutState();

    return *static_cast<CCSCustomHudLayoutState*>(pElement);
}

void CCSCustomHudLayout::SetHasClass(const char* pszPanelId, const char* pszClassName, bool bHasClass, CCSPlayerController* pController)
{
    if (!pszPanelId || !pszClassName)
        return;

    const int panelIndex = InternString(m_vecPanelIds, pszPanelId);
    const int classIndex = InternString(m_vecClassNames, pszClassName);

    auto& layoutState = GetLayoutState(pController);

    HUDPanelHasClass_t hasClass(static_cast<uint16>(panelIndex), static_cast<uint16>(classIndex), bHasClass);

    const int hasClassIndex = layoutState.m_vecHasClasses().Find(hasClass);

    if (hasClassIndex == -1)
        layoutState.m_vecHasClasses().AddToTail(hasClass);
    else
        layoutState.m_vecHasClasses().Element(hasClassIndex).m_eClassStatus = hasClass.m_eClassStatus;

    layoutState.m_vecHasClasses.NetworkStateChanged();
    StateChanged(pController);
}

void CCSCustomHudLayout::SetDialogVariableString(const char* pszPanelId, const char* pszVariableName, const char* pszValue, CCSPlayerController* pController)
{
    if (!pszPanelId || !pszVariableName || !pszValue)
        return;

    const int panelIndex = InternString(m_vecPanelIds, pszPanelId);
    const int variableIndex = InternString(m_vecDialogVariableNames, pszVariableName);

    auto& layoutState = GetLayoutState(pController);

    HUDPanelDialogVariableString_t dialogVariable(static_cast<uint16>(panelIndex), static_cast<uint16>(variableIndex), pszValue, true);

    const int dialogVariableIndex = layoutState.m_vecDialogVariableStrings().Find(dialogVariable);

    if (dialogVariableIndex == -1)
        layoutState.m_vecDialogVariableStrings().AddToTail(dialogVariable);
    else
        layoutState.m_vecDialogVariableStrings().Element(dialogVariableIndex).m_sValue = pszValue;

    layoutState.m_vecDialogVariableStrings.NetworkStateChanged();
    StateChanged(pController);
}

void CCSCustomHudLayout::SetInputCaptureEnabled(bool bEnable, CCSPlayerController* pController)
{
    GetLayoutState(pController).m_bInputCaptureEnabled = bEnable;
    StateChanged(pController);
}

bool CCSCustomHudLayout::IsInputCaptureEnabled(CCSPlayerController* pController)
{
    return GetLayoutState(pController).m_bInputCaptureEnabled();
}

void CCSCustomHudLayout::HookClick(std::function<void(CCSPlayerController*, CCSCustomHudLayout*, const char*)> callback)
{
    g_pToolkitCustomHud->HookCustomHudClick(this, std::move(callback));
}

void CCSCustomHudLayout::UnhookClicks()
{
    g_pToolkitCustomHud->UnhookCustomHudClick(this);
}
