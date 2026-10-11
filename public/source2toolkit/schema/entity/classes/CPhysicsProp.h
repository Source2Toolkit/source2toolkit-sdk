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

#ifndef _INCLUDE_CPHYSICSPROP_H
#define _INCLUDE_CPHYSICSPROP_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/CPhysicsProp__CrateType_t.h"
#include "../enums/DynamicContinuousContactBehavior_t.h"
#include "../enums/INavObstacle__NavObstacleType_t.h"
#include "CBaseEntity.h"
#include "CBreakableProp.h"
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

class CPhysicsProp : public CBreakableProp
{
public:
    DECLARE_SCHEMA_CLASS(CPhysicsProp);

    SCHEMA_FIELD(CEntityIOOutput, m_MotionEnabled);
    SCHEMA_FIELD(CEntityIOOutput, m_OnAwakened);
    SCHEMA_FIELD(CEntityIOOutput, m_OnAwake);
    SCHEMA_FIELD(CEntityIOOutput, m_OnAsleep);
    SCHEMA_FIELD(CEntityIOOutput, m_OnPlayerUse);
    SCHEMA_FIELD(CEntityIOOutput, m_OnOutOfWorld);
    SCHEMA_FIELD(CEntityIOOutput, m_OnPlayerPickup);
    SCHEMA_FIELD(bool, m_bForceNavIgnore);
    SCHEMA_FIELD(bool, m_bNoNavmeshBlocker);
    SCHEMA_FIELD(bool, m_bForceNpcExclude);
    SCHEMA_FIELD(float, m_massScale);
    SCHEMA_FIELD(float, m_buoyancyScale);
    SCHEMA_FIELD(int32_t, m_damageType);
    SCHEMA_FIELD(int32_t, m_damageToEnableMotion);
    SCHEMA_FIELD(float, m_flForceToEnableMotion);
    SCHEMA_FIELD(bool, m_bDroppedByPlayer);
    SCHEMA_FIELD(bool, m_bTouchedByPlayer);
    SCHEMA_FIELD(bool, m_bFirstCollisionAfterLaunch);
    SCHEMA_FIELD(bool, m_bHasBeenAwakened);
    SCHEMA_FIELD(bool, m_bIsOverrideProp);
    SCHEMA_FIELD(float, m_flLastBurn);
    SCHEMA_FIELD(DynamicContinuousContactBehavior_t, m_nDynamicContinuousContactBehavior);
    SCHEMA_FIELD(float, m_fNextCheckDisableMotionContactsTime);
    SCHEMA_FIELD(int32_t, m_iInitialGlowState);
    SCHEMA_FIELD(int32_t, m_nGlowRange);
    SCHEMA_FIELD(int32_t, m_nGlowRangeMin);
    SCHEMA_FIELD(Color, m_glowColor);
    SCHEMA_FIELD(bool, m_bShouldAutoConvertBackFromDebris);
    SCHEMA_FIELD(bool, m_bMuteImpactEffects);
    SCHEMA_FIELD(INavObstacle__NavObstacleType_t, m_nNavObstacleType);
    SCHEMA_FIELD(bool, m_bUpdateNavWhenMoving);
    SCHEMA_FIELD(bool, m_bForceNavObstacleCut);
    SCHEMA_FIELD(bool, m_bAcceptDamageFromHeldObjects);
    SCHEMA_FIELD(bool, m_bEnableUseOutput);
    SCHEMA_FIELD(CPhysicsProp__CrateType_t, m_CrateType);
    SCHEMA_FIELD_POINTER(CUtlSymbolLarge, m_strItemClass);
    SCHEMA_FIELD_POINTER(int32_t, m_nItemCount);
    SCHEMA_FIELD(bool, m_bRemovableForAmmoBalancing);
    SCHEMA_FIELD(bool, m_bAwake);
    SCHEMA_FIELD(bool, m_bAttachedToReferenceFrame);

public:
    static CPhysicsProp* New(const char* className)
    {
        return CBaseEntity::New<CPhysicsProp>(className);
    }

    static CPhysicsProp* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPhysicsProp>(iIndex);
    }

    static CPhysicsProp* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPhysicsProp> GetHandle()
    {
        return CBaseEntity::GetHandle<CPhysicsProp>();
    }
};

#endif // _INCLUDE_CPHYSICSPROP_H
