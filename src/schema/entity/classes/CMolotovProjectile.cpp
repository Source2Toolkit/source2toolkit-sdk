/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
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
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
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
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

#include "source2toolkit/schema/entity/classes/CMolotovProjectile.h"

#include "source2toolkit/IToolkitAddresses.h"
#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitGameConfig.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entity/classes/CBaseEntity.h"
TOOLKIT_GLOBALVARS();

CMolotovProjectile* CMolotovProjectile::EmitGrenade(const Vector& vecPosition, const QAngle& angAngle, const Vector& vecVelocity,
                              CBaseEntity* pOwner, uint32_t nItemDefIndex, HookChain eChain)
{
    // The game takes non-const pointers and reads the velocity twice, once as
    // the linear one and once as the angular one; SwiftlyS2 passes the same
    // vector for both and so do we.
    Vector vecPos = vecPosition;
    QAngle angAng = angAngle;
    Vector vecVel = vecVelocity;

    return ResolveHookChain(ADDR_EMIT_MOLOTOV(), eChain)(&vecPos, &angAng, &vecVel, &vecVel, pOwner, nItemDefIndex);
}
