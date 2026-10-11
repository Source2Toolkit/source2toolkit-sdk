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

#ifndef _INCLUDE_EGCITEMCUSTOMIZATIONNOTIFICATION_H
#define _INCLUDE_EGCITEMCUSTOMIZATIONNOTIFICATION_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>

enum class EGCItemCustomizationNotification : uint32_t
{
    k_EGCItemCustomizationNotification_NameItem = 1006,
    k_EGCItemCustomizationNotification_UnlockCrate = 1007,
    k_EGCItemCustomizationNotification_XRayItemReveal = 1008,
    k_EGCItemCustomizationNotification_XRayItemClaim = 1009,
    k_EGCItemCustomizationNotification_CasketTooFull = 1011,
    k_EGCItemCustomizationNotification_CasketContents = 1012,
    k_EGCItemCustomizationNotification_CasketAdded = 1013,
    k_EGCItemCustomizationNotification_CasketRemoved = 1014,
    k_EGCItemCustomizationNotification_CasketInvFull = 1015,
    k_EGCItemCustomizationNotification_NameBaseItem = 1019,
    k_EGCItemCustomizationNotification_RemoveItemName = 1030,
    k_EGCItemCustomizationNotification_RemoveSticker = 1053,
    k_EGCItemCustomizationNotification_ExtractSticker = 1054,
    k_EGCItemCustomizationNotification_EncapsulateSticker = 1055,
    k_EGCItemCustomizationNotification_ApplySticker = 1086,
    k_EGCItemCustomizationNotification_StatTrakSwap = 1088,
    k_EGCItemCustomizationNotification_RemovePatch = 1089,
    k_EGCItemCustomizationNotification_ApplyPatch = 1090,
    k_EGCItemCustomizationNotification_ApplyKeychain = 1091,
    k_EGCItemCustomizationNotification_RemoveKeychain = 1092,
    k_EGCItemCustomizationNotification_ActivateFanToken = 9178,
    k_EGCItemCustomizationNotification_ActivateOperationCoin = 9179,
    k_EGCItemCustomizationNotification_GraffitiUnseal = 9185,
    k_EGCItemCustomizationNotification_GenerateSouvenir = 9204,
    k_EGCItemCustomizationNotification_ClientRedeemMissionReward = 9209,
    k_EGCItemCustomizationNotification_ClientRedeemFreeReward = 9219,
    k_EGCItemCustomizationNotification_XpShopUseTicket = 9221,
    k_EGCItemCustomizationNotification_XpShopAckTracks = 9222,
};

#endif // _INCLUDE_EGCITEMCUSTOMIZATIONNOTIFICATION_H
