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

#ifndef _INCLUDE_CBASEPROPDOOR_H
#define _INCLUDE_CBASEPROPDOOR_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/DoorState_t.h"
#include "CBaseEntity.h"
#include "CDynamicProp.h"
#include "locksound_t.h"
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

class CBasePropDoor : public CDynamicProp
{
public:
    DECLARE_SCHEMA_CLASS(CBasePropDoor);

    SCHEMA_FIELD(float, m_flAutoReturnDelay);
    SCHEMA_FIELD(CUtlVector<CHandle<CBasePropDoor>>, m_hDoorList);
    SCHEMA_FIELD(int32_t, m_nHardwareType);
    SCHEMA_FIELD(bool, m_bNeedsHardware);
    SCHEMA_FIELD(DoorState_t, m_eDoorState);
    SCHEMA_FIELD(bool, m_bLocked);
    SCHEMA_FIELD(bool, m_bNoNPCs);
    SCHEMA_FIELD(Vector, m_closedPosition);
    SCHEMA_FIELD(QAngle, m_closedAngles);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hBlocker);
    SCHEMA_FIELD(bool, m_bFirstBlocked);
    SCHEMA_FIELD(locksound_t, m_ls);
    SCHEMA_FIELD(bool, m_bForceClosed);
    SCHEMA_FIELD(Vector, m_vecLatchWorldPosition);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hActivator);
    SCHEMA_FIELD(float, m_flSpeed);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundMoving);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundOpen);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundClose);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundLock);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundUnlock);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundLatch);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundPound);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundJiggle);
    SCHEMA_FIELD(CGameSoundEventName, m_SoundLockedAnim);
    SCHEMA_FIELD(int32_t, m_numCloseAttempts);
    SCHEMA_FIELD_POINTER(CUtlStringToken, m_nPhysicsMaterial);
    SCHEMA_FIELD(CUtlSymbolLarge, m_SlaveName);
    SCHEMA_FIELD(CHandle<CBasePropDoor>, m_hMaster);
    SCHEMA_FIELD(CEntityIOOutput, m_OnBlockedClosing);
    SCHEMA_FIELD(CEntityIOOutput, m_OnBlockedOpening);
    SCHEMA_FIELD(CEntityIOOutput, m_OnUnblockedClosing);
    SCHEMA_FIELD(CEntityIOOutput, m_OnUnblockedOpening);
    SCHEMA_FIELD(CEntityIOOutput, m_OnFullyClosed);
    SCHEMA_FIELD(CEntityIOOutput, m_OnFullyOpen);
    SCHEMA_FIELD(CEntityIOOutput, m_OnClose);
    SCHEMA_FIELD(CEntityIOOutput, m_OnOpen);
    SCHEMA_FIELD(CEntityIOOutput, m_OnLockedUse);
    SCHEMA_FIELD(CEntityIOOutput, m_OnAjarOpen);

public:
    static CBasePropDoor* New(const char* className)
    {
        return CBaseEntity::New<CBasePropDoor>(className);
    }

    static CBasePropDoor* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBasePropDoor>(iIndex);
    }

    static CBasePropDoor* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBasePropDoor> GetHandle()
    {
        return CBaseEntity::GetHandle<CBasePropDoor>();
    }
};

#endif // _INCLUDE_CBASEPROPDOOR_H
