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

#ifndef _INCLUDE_CFUNCROTATOR_H
#define _INCLUDE_CFUNCROTATOR_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/CFuncRotator__Rotate_t.h"
#include "../enums/CFuncRotator__RotationAxis_t.h"
#include "CBaseEntity.h"
#include "CBaseModelEntity.h"
#include "FuncRotatorRotationSummary_t.h"
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

class CFuncRotator : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CFuncRotator);

    SCHEMA_FIELD(CFuncRotator__Rotate_t, m_eRotateType);
    SCHEMA_FIELD(bool, m_bIsRotating);
    SCHEMA_FIELD(SolidType_t, m_eSolidType);
    SCHEMA_FIELD(float, m_flSpeed);
    SCHEMA_FIELD(float, m_flRotationDistanceDegrees);
    SCHEMA_FIELD(float, m_flTimeToCompleteRotation);
    SCHEMA_FIELD(CHandle<CBaseEntity>, m_hRotatorTarget);
    SCHEMA_FIELD(CUtlSymbolLarge, m_strRotatorTarget);
    SCHEMA_FIELD(CEntityIOOutput, m_OnRotationStarted);
    SCHEMA_FIELD(CEntityIOOutput, m_OnRotationCompleted);
    SCHEMA_FIELD(CEntityIOOutput, m_OnOscillate);
    SCHEMA_FIELD(CEntityIOOutput, m_OnOscillateStartArrive);
    SCHEMA_FIELD(CEntityIOOutput, m_OnOscillateStartDepart);
    SCHEMA_FIELD(CEntityIOOutput, m_OnOscillateEndArrive);
    SCHEMA_FIELD(CEntityIOOutput, m_OnOscillateEndDepart);
    SCHEMA_FIELD(int32_t, m_nTickRotateRan);
    SCHEMA_FIELD(bool, m_bStartedRotating);
    SCHEMA_FIELD(FuncRotatorRotationSummary_t, m_rotationSummary);
    SCHEMA_FIELD(float, m_flTimeToReachMaxSpeed);
    SCHEMA_FIELD(float, m_flTimeToReachZeroSpeed);
    SCHEMA_FIELD(float, m_flTimeRotationStart);
    SCHEMA_FIELD(float, m_flTimeRotationStop);
    SCHEMA_FIELD(float, m_flStartSpeed);
    SCHEMA_FIELD(Quaternion, m_qSpawnOrientation);
    SCHEMA_FIELD(bool, m_bReturningToInitialRotation);
    SCHEMA_FIELD(float, m_flMinYawRotation);
    SCHEMA_FIELD(float, m_flMaxYawRotation);
    SCHEMA_FIELD(bool, m_bOscillationFromStart);
    SCHEMA_FIELD(CGameSoundEventName, m_iszStartSound);
    SCHEMA_FIELD(CGameSoundEventName, m_iszLoopSound);
    SCHEMA_FIELD(CGameSoundEventName, m_iszStopSound);
    SCHEMA_FIELD(float, m_flTargetAngle);
    SCHEMA_FIELD(float, m_flCurrentAngle);
    SCHEMA_FIELD(CFuncRotator__RotationAxis_t, m_eRotationAxis);
    SCHEMA_FIELD(float, m_flSpeedDriftFromOverRotate);
    SCHEMA_FIELD(bool, m_bQueueStop);

public:
    static CFuncRotator* New(const char* className)
    {
        return CBaseEntity::New<CFuncRotator>(className);
    }

    static CFuncRotator* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CFuncRotator>(iIndex);
    }

    static CFuncRotator* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CFuncRotator> GetHandle()
    {
        return CBaseEntity::GetHandle<CFuncRotator>();
    }
};

#endif // _INCLUDE_CFUNCROTATOR_H
