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

#ifndef _INCLUDE_CBASEBUTTON_H
#define _INCLUDE_CBASEBUTTON_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CBaseToggle.h"
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

class CBaseModelEntity;

class CBaseButton : public CBaseToggle
{
public:
    DECLARE_SCHEMA_CLASS(CBaseButton);

    SCHEMA_FIELD(QAngle, m_angMoveEntitySpace);
    SCHEMA_FIELD(bool, m_fStayPushed);
    SCHEMA_FIELD(bool, m_fRotating);
    SCHEMA_FIELD(locksound_t, m_ls);
    SCHEMA_FIELD(CGameSoundEventName, m_sUseSound);
    SCHEMA_FIELD(CGameSoundEventName, m_sLockedSound);
    SCHEMA_FIELD(CGameSoundEventName, m_sUnlockedSound);
    SCHEMA_FIELD(CUtlSymbolLarge, m_sOverrideAnticipationName);
    SCHEMA_FIELD(bool, m_bLocked);
    SCHEMA_FIELD(bool, m_bDisabled);
    SCHEMA_FIELD(float, m_flSpeed);
    SCHEMA_FIELD(float, m_flUseLockedTime);
    SCHEMA_FIELD(bool, m_bSolidBsp);
    SCHEMA_FIELD(CEntityIOOutput, m_OnDamaged);
    SCHEMA_FIELD(CEntityIOOutput, m_OnPressed);
    SCHEMA_FIELD(CEntityIOOutput, m_OnUseLocked);
    SCHEMA_FIELD(CEntityIOOutput, m_OnIn);
    SCHEMA_FIELD(CEntityIOOutput, m_OnOut);
    SCHEMA_FIELD(int32_t, m_nState);
    SCHEMA_FIELD(CEntityHandle, m_hConstraint);
    SCHEMA_FIELD(CEntityHandle, m_hConstraintParent);
    SCHEMA_FIELD(bool, m_bForceNpcExclude);
    SCHEMA_FIELD(CUtlSymbolLarge, m_sGlowEntity);
    SCHEMA_FIELD(CHandle<CBaseModelEntity>, m_glowEntity);
    SCHEMA_FIELD(bool, m_usable);
    SCHEMA_FIELD(CUtlSymbolLarge, m_szDisplayText);

public:
    static CBaseButton* New(const char* className)
    {
        return CBaseEntity::New<CBaseButton>(className);
    }

    static CBaseButton* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBaseButton>(iIndex);
    }

    static CBaseButton* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBaseButton> GetHandle()
    {
        return CBaseEntity::GetHandle<CBaseButton>();
    }
};

#endif // _INCLUDE_CBASEBUTTON_H
