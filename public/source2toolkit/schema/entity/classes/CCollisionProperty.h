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

#ifndef _INCLUDE_CCOLLISIONPROPERTY_H
#define _INCLUDE_CCOLLISIONPROPERTY_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/SurroundingBoundsType_t.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"
#include "VPhysicsCollisionAttribute_t.h"

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

class CCollisionProperty
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CCollisionProperty);

    SCHEMA_FIELD(VPhysicsCollisionAttribute_t, m_collisionAttribute);
    SCHEMA_FIELD(Vector, m_vecMins);
    SCHEMA_FIELD(Vector, m_vecMaxs);
    SCHEMA_FIELD(uint8_t, m_usSolidFlags);
    SCHEMA_FIELD(SolidType_t, m_nSolidType);
    SCHEMA_FIELD(uint8_t, m_triggerBloat);
    SCHEMA_FIELD(SurroundingBoundsType_t, m_nSurroundType);
    SCHEMA_FIELD(uint8_t, m_CollisionGroup);
    SCHEMA_FIELD(uint8_t, m_nEnablePhysics);
    SCHEMA_FIELD(float, m_flBoundingRadius);
    SCHEMA_FIELD(Vector, m_vecSpecifiedSurroundingMins);
    SCHEMA_FIELD(Vector, m_vecSpecifiedSurroundingMaxs);
    SCHEMA_FIELD(Vector, m_vecSurroundingMaxs);
    SCHEMA_FIELD(Vector, m_vecSurroundingMins);
    SCHEMA_FIELD(Vector, m_vCapsuleCenter1);
    SCHEMA_FIELD(Vector, m_vCapsuleCenter2);
    SCHEMA_FIELD(float, m_flCapsuleRadius);
};

#endif // _INCLUDE_CCOLLISIONPROPERTY_H
