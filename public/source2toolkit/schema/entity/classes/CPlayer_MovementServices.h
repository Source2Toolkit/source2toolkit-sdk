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

#ifndef _INCLUDE_CPLAYER_MOVEMENTSERVICES_H
#define _INCLUDE_CPLAYER_MOVEMENTSERVICES_H
#ifdef _WIN32
#pragma once
#endif

#include "CInButtonState.h"
#include "CPlayerPawnComponent.h"
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

class CPlayer_MovementServices : public CPlayerPawnComponent
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CPlayer_MovementServices);

    SCHEMA_FIELD(int32_t, m_nImpulse);
    SCHEMA_FIELD(CInButtonState, m_nButtons);
    SCHEMA_FIELD(uint64_t, m_nQueuedButtonDownMask);
    SCHEMA_FIELD(uint64_t, m_nQueuedButtonChangeMask);
    SCHEMA_FIELD(uint64_t, m_nButtonDoublePressed);
    SCHEMA_FIELD_POINTER(uint32_t, m_pButtonPressedCmdNumber);
    SCHEMA_FIELD(uint32_t, m_nLastCommandNumberProcessed);
    SCHEMA_FIELD(uint64_t, m_nToggleButtonDownMask);
    SCHEMA_FIELD(float, m_flCmdForwardMove);
    SCHEMA_FIELD(float, m_flCmdLeftMove);
    SCHEMA_FIELD(float, m_flCmdUpMove);
    SCHEMA_FIELD(float, m_flMaxspeed);
    SCHEMA_FIELD_POINTER(float, m_arrForceSubtickMoveWhen);
    SCHEMA_FIELD(float, m_flForwardMove);
    SCHEMA_FIELD(float, m_flLeftMove);
    SCHEMA_FIELD(float, m_flUpMove);
    SCHEMA_FIELD(Vector, m_vecLastMovementImpulses);
    SCHEMA_FIELD(QAngle, m_vecOldViewAngles);
};

#endif // _INCLUDE_CPLAYER_MOVEMENTSERVICES_H
