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

#ifndef _INCLUDE_CTRIGGERFAN_H
#define _INCLUDE_CTRIGGERFAN_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CBaseTrigger.h"
#include "CountdownTimer.h"
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

class CInfoFan;

class CTriggerFan : public CBaseTrigger
{
public:
    DECLARE_SCHEMA_CLASS(CTriggerFan);

    SCHEMA_FIELD(Vector, m_vFanOriginOffset);
    SCHEMA_FIELD(Vector, m_vDirection);
    SCHEMA_FIELD(bool, m_bPushTowardsInfoTarget);
    SCHEMA_FIELD(bool, m_bPushAwayFromInfoTarget);
    SCHEMA_FIELD(Quaternion, m_qNoiseDelta);
    SCHEMA_FIELD(CHandle<CInfoFan>, m_hInfoFan);
    SCHEMA_FIELD(float, m_flForce);
    SCHEMA_FIELD(bool, m_bFalloff);
    SCHEMA_FIELD(CountdownTimer, m_RampTimer);
    SCHEMA_FIELD(Vector, m_vFanOriginWS);
    SCHEMA_FIELD(Vector, m_vFanOriginLS);
    SCHEMA_FIELD(Vector, m_vFanEndLS);
    SCHEMA_FIELD(Vector, m_vNoiseDirectionTarget);
    SCHEMA_FIELD(CUtlSymbolLarge, m_iszInfoFan);
    SCHEMA_FIELD(float, m_flRopeForceScale);
    SCHEMA_FIELD(float, m_flParticleForceScale);
    SCHEMA_FIELD(float, m_flPlayerForce);
    SCHEMA_FIELD(bool, m_bPlayerWindblock);
    SCHEMA_FIELD(float, m_flNPCForce);
    SCHEMA_FIELD(float, m_flRampTime);
    SCHEMA_FIELD(float, m_fNoiseDegrees);
    SCHEMA_FIELD(float, m_fNoiseSpeed);
    SCHEMA_FIELD(bool, m_bPushPlayer);
    SCHEMA_FIELD(bool, m_bRampDown);
    SCHEMA_FIELD(int32_t, m_nManagerFanIdx);

public:
    static CTriggerFan* New(const char* className)
    {
        return CBaseEntity::New<CTriggerFan>(className);
    }

    static CTriggerFan* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CTriggerFan>(iIndex);
    }

    static CTriggerFan* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CTriggerFan> GetHandle()
    {
        return CBaseEntity::GetHandle<CTriggerFan>();
    }
};

#endif // _INCLUDE_CTRIGGERFAN_H
