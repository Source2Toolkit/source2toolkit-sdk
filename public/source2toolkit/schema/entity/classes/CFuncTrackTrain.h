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

#ifndef _INCLUDE_CFUNCTRACKTRAIN_H
#define _INCLUDE_CFUNCTRACKTRAIN_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/TrainOrientationType_t.h"
#include "../enums/TrainVelocityType_t.h"
#include "CBaseEntity.h"
#include "CBaseModelEntity.h"
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

class CPathTrack;

class CFuncTrackTrain : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CFuncTrackTrain);

    SCHEMA_FIELD(CHandle<CPathTrack>, m_ppath);
    SCHEMA_FIELD(float, m_length);
    SCHEMA_FIELD(Vector, m_vPosPrev);
    SCHEMA_FIELD(QAngle, m_angPrev);
    SCHEMA_FIELD(float, m_flSpeed);
    SCHEMA_FIELD(Vector, m_controlMins);
    SCHEMA_FIELD(Vector, m_controlMaxs);
    SCHEMA_FIELD(Vector, m_lastBlockPos);
    SCHEMA_FIELD(int32_t, m_lastBlockTick);
    SCHEMA_FIELD(float, m_flVolume);
    SCHEMA_FIELD(float, m_flBank);
    SCHEMA_FIELD(float, m_oldSpeed);
    SCHEMA_FIELD(float, m_flBlockDamage);
    SCHEMA_FIELD(float, m_height);
    SCHEMA_FIELD(float, m_maxSpeed);
    SCHEMA_FIELD(float, m_dir);
    SCHEMA_FIELD(CGameSoundEventName, m_iszSoundMove);
    SCHEMA_FIELD(CGameSoundEventName, m_iszSoundMovePing);
    SCHEMA_FIELD(CGameSoundEventName, m_iszSoundStart);
    SCHEMA_FIELD(CGameSoundEventName, m_iszSoundStop);
    SCHEMA_FIELD(CGameSoundEventName, m_strPathTarget);
    SCHEMA_FIELD(float, m_flMoveSoundMinDuration);
    SCHEMA_FIELD(float, m_flMoveSoundMaxDuration);
    SCHEMA_FIELD(float, m_flNextMoveSoundTime);
    SCHEMA_FIELD(float, m_flMoveSoundMinPitch);
    SCHEMA_FIELD(float, m_flMoveSoundMaxPitch);
    SCHEMA_FIELD(TrainOrientationType_t, m_eOrientationType);
    SCHEMA_FIELD(TrainVelocityType_t, m_eVelocityType);
    SCHEMA_FIELD(CEntityIOOutput, m_OnStart);
    SCHEMA_FIELD(CEntityIOOutput, m_OnNext);
    SCHEMA_FIELD(CEntityIOOutput, m_OnArrivedAtDestinationNode);
    SCHEMA_FIELD(bool, m_bManualSpeedChanges);
    SCHEMA_FIELD(float, m_flDesiredSpeed);
    SCHEMA_FIELD(float, m_flSpeedChangeTime);
    SCHEMA_FIELD(float, m_flAccelSpeed);
    SCHEMA_FIELD(float, m_flDecelSpeed);
    SCHEMA_FIELD(bool, m_bAccelToSpeed);
    SCHEMA_FIELD(float, m_flNextMPSoundTime);

public:
    static CFuncTrackTrain* New(const char* className)
    {
        return CBaseEntity::New<CFuncTrackTrain>(className);
    }

    static CFuncTrackTrain* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CFuncTrackTrain>(iIndex);
    }

    static CFuncTrackTrain* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CFuncTrackTrain> GetHandle()
    {
        return CBaseEntity::GetHandle<CFuncTrackTrain>();
    }
};

#endif // _INCLUDE_CFUNCTRACKTRAIN_H
