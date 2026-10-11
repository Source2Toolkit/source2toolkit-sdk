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

#ifndef _INCLUDE_CITEMGENERIC_H
#define _INCLUDE_CITEMGENERIC_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CItem.h"
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

class CBaseFilter;
class CItemGenericTriggerHelper;
class InfoForResourceTypeIParticleSystemDefinition;

class CItemGeneric : public CItem
{
public:
    DECLARE_SCHEMA_CLASS(CItemGeneric);

    SCHEMA_FIELD(bool, m_bHasTriggerRadius);
    SCHEMA_FIELD(bool, m_bHasPickupRadius);
    SCHEMA_FIELD(float, m_flPickupRadiusSqr);
    SCHEMA_FIELD(float, m_flTriggerRadiusSqr);
    SCHEMA_FIELD(float, m_flLastPickupCheck);
    SCHEMA_FIELD(bool, m_bPlayerCounterListenerAdded);
    SCHEMA_FIELD(bool, m_bPlayerInTriggerRadius);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIParticleSystemDefinition>, m_hSpawnParticleEffect);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pAmbientSoundEffect);
    SCHEMA_FIELD(bool, m_bAutoStartAmbientSound);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pSpawnScriptFunction);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIParticleSystemDefinition>, m_hPickupParticleEffect);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pPickupSoundEffect);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pPickupScriptFunction);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIParticleSystemDefinition>, m_hTimeoutParticleEffect);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pTimeoutSoundEffect);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pTimeoutScriptFunction);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pPickupFilterName);
    SCHEMA_FIELD(CHandle<CBaseFilter>, m_hPickupFilter);
    SCHEMA_FIELD(CEntityIOOutput, m_OnPickup);
    SCHEMA_FIELD(CEntityIOOutput, m_OnTimeout);
    SCHEMA_FIELD(CEntityIOOutput, m_OnTriggerStartTouch);
    SCHEMA_FIELD(CEntityIOOutput, m_OnTriggerTouch);
    SCHEMA_FIELD(CEntityIOOutput, m_OnTriggerEndTouch);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pAllowPickupScriptFunction);
    SCHEMA_FIELD(float, m_flPickupRadius);
    SCHEMA_FIELD(float, m_flTriggerRadius);
    SCHEMA_FIELD(CUtlSymbolLarge, m_pTriggerSoundEffect);
    SCHEMA_FIELD(bool, m_bGlowWhenInTrigger);
    SCHEMA_FIELD(Color, m_glowColor);
    SCHEMA_FIELD(bool, m_bUseable);
    SCHEMA_FIELD(CHandle<CItemGenericTriggerHelper>, m_hTriggerHelper);

public:
    static CItemGeneric* New(const char* className)
    {
        return CBaseEntity::New<CItemGeneric>(className);
    }

    static CItemGeneric* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CItemGeneric>(iIndex);
    }

    static CItemGeneric* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CItemGeneric> GetHandle()
    {
        return CBaseEntity::GetHandle<CItemGeneric>();
    }
};

#endif // _INCLUDE_CITEMGENERIC_H
