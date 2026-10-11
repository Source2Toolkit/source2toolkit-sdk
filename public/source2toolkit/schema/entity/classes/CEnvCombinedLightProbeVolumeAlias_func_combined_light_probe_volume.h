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

#ifndef _INCLUDE_CENVCOMBINEDLIGHTPROBEVOLUMEALIAS_FUNC_COMBINED_LIGHT_PROBE_VOLUME_H
#define _INCLUDE_CENVCOMBINEDLIGHTPROBEVOLUMEALIAS_FUNC_COMBINED_LIGHT_PROBE_VOLUME_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CEnvCombinedLightProbeVolume.h"
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

class CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume : public CEnvCombinedLightProbeVolume
{
public:
    DECLARE_SCHEMA_CLASS(CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume);


public:
    static CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume* New(const char* className)
    {
        return CBaseEntity::New<CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume>(className);
    }

    static CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume>(iIndex);
    }

    static CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume> GetHandle()
    {
        return CBaseEntity::GetHandle<CEnvCombinedLightProbeVolumeAlias_func_combined_light_probe_volume>();
    }
};

#endif // _INCLUDE_CENVCOMBINEDLIGHTPROBEVOLUMEALIAS_FUNC_COMBINED_LIGHT_PROBE_VOLUME_H
