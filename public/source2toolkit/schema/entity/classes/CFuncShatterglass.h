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

#ifndef _INCLUDE_CFUNCSHATTERGLASS_H
#define _INCLUDE_CFUNCSHATTERGLASS_H
#ifdef _WIN32
#pragma once
#endif

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

class InfoForResourceTypeIMaterial2;

class CFuncShatterglass : public CBaseModelEntity
{
public:
    DECLARE_SCHEMA_CLASS(CFuncShatterglass);

    SCHEMA_FIELD(matrix3x4_t, m_matPanelTransform);
    SCHEMA_FIELD(matrix3x4_t, m_matPanelTransformWsTemp);
    SCHEMA_FIELD(CUtlVector<uint32_t>, m_vecShatterGlassShards);
    SCHEMA_FIELD(Vector2D, m_PanelSize);
    SCHEMA_FIELD(float, m_flLastShatterSoundEmitTime);
    SCHEMA_FIELD(float, m_flLastCleanupTime);
    SCHEMA_FIELD(float, m_flInitAtTime);
    SCHEMA_FIELD(float, m_flGlassThickness);
    SCHEMA_FIELD(float, m_flSpawnInvulnerability);
    SCHEMA_FIELD(bool, m_bBreakSilent);
    SCHEMA_FIELD(bool, m_bBreakShardless);
    SCHEMA_FIELD(bool, m_bBroken);
    SCHEMA_FIELD(bool, m_bGlassNavIgnore);
    SCHEMA_FIELD(bool, m_bGlassInFrame);
    SCHEMA_FIELD(bool, m_bStartBroken);
    SCHEMA_FIELD(uint8_t, m_iInitialDamageType);
    SCHEMA_FIELD(CUtlSymbolLarge, m_szDamagePositioningEntityName01);
    SCHEMA_FIELD(CUtlSymbolLarge, m_szDamagePositioningEntityName02);
    SCHEMA_FIELD(CUtlSymbolLarge, m_szDamagePositioningEntityName03);
    SCHEMA_FIELD(CUtlSymbolLarge, m_szDamagePositioningEntityName04);
    SCHEMA_FIELD(CUtlVector<Vector>, m_vInitialDamagePositions);
    SCHEMA_FIELD(CUtlVector<Vector>, m_vExtraDamagePositions);
    SCHEMA_FIELD(CUtlVector<Vector4D>, m_vInitialPanelVertices);
    SCHEMA_FIELD(CEntityIOOutput, m_OnBroken);
    SCHEMA_FIELD(uint8_t, m_iSurfaceType);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeIMaterial2>, m_hMaterialDamageBase);

public:
    static CFuncShatterglass* New(const char* className)
    {
        return CBaseEntity::New<CFuncShatterglass>(className);
    }

    static CFuncShatterglass* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CFuncShatterglass>(iIndex);
    }

    static CFuncShatterglass* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CFuncShatterglass> GetHandle()
    {
        return CBaseEntity::GetHandle<CFuncShatterglass>();
    }
};

#endif // _INCLUDE_CFUNCSHATTERGLASS_H
