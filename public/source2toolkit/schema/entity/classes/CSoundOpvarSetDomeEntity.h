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

#ifndef _INCLUDE_CSOUNDOPVARSETDOMEENTITY_H
#define _INCLUDE_CSOUNDOPVARSETDOMEENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CSoundOpvarSetPointEntity.h"
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

class CSoundOpvarSetDomeEntity : public CSoundOpvarSetPointEntity
{
public:
    DECLARE_SCHEMA_CLASS(CSoundOpvarSetDomeEntity);

    SCHEMA_FIELD(CUtlVector<Vector>, m_arDirections);
    SCHEMA_FIELD(CUtlVector<float>, m_arOpenness);
    SCHEMA_FIELD(CUtlVector<int32_t>, m_arNeighbors);
    SCHEMA_FIELD(int32_t, m_nCurrentIndex);
    SCHEMA_FIELD(CUtlVector<int32_t>, m_arClusterParent);
    SCHEMA_FIELD(CUtlVector<int32_t>, m_arClusterSize);
    SCHEMA_FIELD(CUtlVector<float>, m_arClusterWeight);
    SCHEMA_FIELD(CUtlVector<Vector>, m_arClusterDirSum);
    SCHEMA_FIELD(int32_t, m_nClusterIndex);
    SCHEMA_FIELD(float, m_flClusteredOpenness);
    SCHEMA_FIELD(Vector, m_vClusterDirection);
    SCHEMA_FIELD(Vector, m_vSmoothedOpenDir);
    SCHEMA_FIELD(int32_t, m_nDirWarmupThinksRemaining);
    SCHEMA_FIELD(Vector, m_vLastTraceOrigin);
    SCHEMA_FIELD(bool, m_bTraceOriginValid);
    SCHEMA_FIELD(int32_t, m_nCatchUpThinksRemaining);
    SCHEMA_FIELD(bool, m_bDiscontinuityPending);
    SCHEMA_FIELD(float, m_flSmoothedOpenness);
    SCHEMA_FIELD(float, m_flLastSmoothTime);
    SCHEMA_FIELD(float, m_flSize);
    SCHEMA_FIELD(int32_t, m_nTotalDirections);
    SCHEMA_FIELD(int32_t, m_nTracesPerFrame);
    SCHEMA_FIELD(bool, m_bDomeMode);
    SCHEMA_FIELD(bool, m_bMultiWall);
    SCHEMA_FIELD(float, m_flWallTransmission);
    SCHEMA_FIELD(int32_t, m_nClusterK);
    SCHEMA_FIELD(float, m_flClusterP);
    SCHEMA_FIELD(float, m_flClusterBlend);
    SCHEMA_FIELD(float, m_flOpennessExponent);
    SCHEMA_FIELD(float, m_flShoulderExponent);
    SCHEMA_FIELD(float, m_flSmoothHalfLife);

public:
    static CSoundOpvarSetDomeEntity* New(const char* className)
    {
        return CBaseEntity::New<CSoundOpvarSetDomeEntity>(className);
    }

    static CSoundOpvarSetDomeEntity* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CSoundOpvarSetDomeEntity>(iIndex);
    }

    static CSoundOpvarSetDomeEntity* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CSoundOpvarSetDomeEntity> GetHandle()
    {
        return CBaseEntity::GetHandle<CSoundOpvarSetDomeEntity>();
    }
};

#endif // _INCLUDE_CSOUNDOPVARSETDOMEENTITY_H
