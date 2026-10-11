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

#ifndef _INCLUDE_EBASEUSERMESSAGES_H
#define _INCLUDE_EBASEUSERMESSAGES_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class EBaseUserMessages : uint32_t
{
    UM_AchievementEvent = 101,
    UM_CurrentTimescale = 104,
    UM_DesiredTimescale = 105,
    UM_Fade = 106,
    UM_GameTitle = 107,
    UM_HudMsg = 110,
    UM_HudText = 111,
    UM_ColoredText = 113,
    UM_RequestState = 114,
    UM_ResetHUD = 115,
    UM_Rumble = 116,
    UM_SayText = 117,
    UM_SayText2 = 118,
    UM_SayTextChannel = 119,
    UM_Shake = 120,
    UM_ShakeDir = 121,
    UM_WaterShake = 122,
    UM_TextMsg = 124,
    UM_ScreenTilt = 125,
    UM_VoiceMask = 128,
    UM_SendAudio = 130,
    UM_ItemPickup = 131,
    UM_AmmoDenied = 132,
    UM_ShowMenu = 134,
    UM_CreditsMsg = 135,
    UM_CloseCaptionPlaceholder = 142,
    UM_CameraTransition = 143,
    UM_AudioParameter = 144,
    UM_ParticleManager = 145,
    UM_HudError = 146,
    UM_CustomGameEvent = 148,
    UM_AnimGraphUpdate = 149,
    UM_HapticsManagerPulse = 150,
    UM_HapticsManagerEffect = 151,
    UM_UpdateCssClasses = 153,
    UM_ServerFrameTime = 154,
    UM_LagCompensationError = 155,
    UM_RequestDllStatus = 156,
    UM_RequestUtilAction = 157,
    UM_UtilActionResponse = 158,
    UM_DllStatusResponse = 159,
    UM_RequestInventory = 160,
    UM_InventoryResponse = 161,
    UM_RequestDiagnostic = 162,
    UM_DiagnosticResponse = 163,
    UM_ExtraUserData = 164,
    UM_NotifyResponseFound = 165,
    UM_PlayResponseConditional = 166,
    UM_UserSentBugBug = 167,
    UM_UsageReport = 168,
    UM_RemoteServerCommand = 169,
    UM_RemoteServerResponse = 170,
    UM_MAX_BASE = 200,
};

#endif // _INCLUDE_EBASEUSERMESSAGES_H
