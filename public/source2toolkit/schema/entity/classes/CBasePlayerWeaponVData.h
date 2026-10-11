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

#ifndef _INCLUDE_CBASEPLAYERWEAPONVDATA_H
#define _INCLUDE_CBASEPLAYERWEAPONVDATA_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/RumbleEffect_t.h"
#include "CEntitySubclassVDataBase.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"

#include <ehandle.h>
#include <entityhandle.h>
#include <igameevents.h>
#include <shareddefs.h>
#include <utlbinaryblock.h>
#include <utlstring.h>
#include <utlstringtoken.h>
#include <utlsymbol.h>
#include <utlsymbollarge.h>
#include <vector.h>

#include <cstdint>

class CBasePlayerWeaponVData : public CEntitySubclassVDataBase
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CBasePlayerWeaponVData);

    SCHEMA_FIELD(bool, m_bBuiltRightHanded);
    SCHEMA_FIELD(bool, m_bAllowFlipping);
    SCHEMA_FIELD(CUtlString, m_szMuzzleFlashParticleConfig);
    SCHEMA_FIELD(uint8_t, m_nMuzzleSmokeShotThreshold);
    SCHEMA_FIELD(float, m_flMuzzleSmokeTimeout);
    SCHEMA_FIELD(float, m_flMuzzleSmokeDecrementRate);
    SCHEMA_FIELD(bool, m_bGenerateMuzzleLight);
    SCHEMA_FIELD(bool, m_bShouldAnimateInWorld);
    SCHEMA_FIELD(bool, m_bLinkedCooldowns);
    SCHEMA_FIELD(ItemFlagTypes_t, m_iFlags);
    SCHEMA_FIELD(int32_t, m_iWeight);
    SCHEMA_FIELD(bool, m_bAutoSwitchTo);
    SCHEMA_FIELD(bool, m_bAutoSwitchFrom);
    SCHEMA_FIELD(uint8_t, m_nPrimaryAmmoType);
    SCHEMA_FIELD(uint8_t, m_nSecondaryAmmoType);
    SCHEMA_FIELD(int32_t, m_iMaxClip1);
    SCHEMA_FIELD(int32_t, m_iMaxClip2);
    SCHEMA_FIELD(int32_t, m_iDefaultClip1);
    SCHEMA_FIELD(int32_t, m_iDefaultClip2);
    SCHEMA_FIELD(bool, m_bReserveAmmoAsClips);
    SCHEMA_FIELD(bool, m_bTreatAsSingleClip);
    SCHEMA_FIELD(bool, m_bKeepLoadedAmmo);
    SCHEMA_FIELD(RumbleEffect_t, m_iRumbleEffect);
    SCHEMA_FIELD(float, m_flDropSpeed);
    SCHEMA_FIELD(int32_t, m_iSlot);
    SCHEMA_FIELD(int32_t, m_iPosition);
};

#endif // _INCLUDE_CBASEPLAYERWEAPONVDATA_H
