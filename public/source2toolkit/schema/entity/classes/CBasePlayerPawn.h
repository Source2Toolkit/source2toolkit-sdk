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

#ifndef _INCLUDE_CBASEPLAYERPAWN_H
#define _INCLUDE_CBASEPLAYERPAWN_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseCombatCharacter.h"
#include "CBaseEntity.h"
#include "sky3dparams_t.h"
#include "sndopvarlatchdata_t.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"
#include "ViewAngleServerChange_t.h"

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

class CAI_Expresser;
class CBasePlayerController;
class CCSPlayerController;
class CPlayer_AutoaimServices;
class CPlayer_CameraServices;
class CPlayer_FlashlightServices;
class CPlayer_ItemServices;
class CPlayer_MovementServices;
class CPlayer_ObserverServices;
class CPlayer_UseServices;
class CPlayer_WaterServices;
class CPlayer_WeaponServices;

class CBasePlayerPawn : public CBaseCombatCharacter
{
public:
    DECLARE_SCHEMA_CLASS(CBasePlayerPawn);

    SCHEMA_FIELD(CPlayer_WeaponServices*, m_pWeaponServices);
    SCHEMA_FIELD(CPlayer_ItemServices*, m_pItemServices);
    SCHEMA_FIELD(CPlayer_AutoaimServices*, m_pAutoaimServices);
    SCHEMA_FIELD(CPlayer_ObserverServices*, m_pObserverServices);
    SCHEMA_FIELD(CPlayer_WaterServices*, m_pWaterServices);
    SCHEMA_FIELD(CPlayer_UseServices*, m_pUseServices);
    SCHEMA_FIELD(CPlayer_FlashlightServices*, m_pFlashlightServices);
    SCHEMA_FIELD(CPlayer_CameraServices*, m_pCameraServices);
    SCHEMA_FIELD(CPlayer_MovementServices*, m_pMovementServices);
    SCHEMA_FIELD(CUtlVector<ViewAngleServerChange_t>, m_ServerViewAngleChanges);
    SCHEMA_FIELD(QAngle, v_angle);
    SCHEMA_FIELD(QAngle, v_anglePrevious);
    SCHEMA_FIELD(uint32_t, m_iHideHUD);
    SCHEMA_FIELD(sky3dparams_t, m_skybox3d);
    SCHEMA_FIELD(float, m_fTimeLastHurt);
    SCHEMA_FIELD(float, m_flDeathTime);
    SCHEMA_FIELD(float, m_fNextSuicideTime);
    SCHEMA_FIELD(bool, m_fInitHUD);
    SCHEMA_FIELD(CAI_Expresser*, m_pExpresser);
    SCHEMA_FIELD(CHandle<CBasePlayerController>, m_hController);
    SCHEMA_FIELD(CHandle<CBasePlayerController>, m_hDefaultController);
    SCHEMA_FIELD(float, m_fHltvReplayDelay);
    SCHEMA_FIELD(float, m_fHltvReplayEnd);
    SCHEMA_FIELD(CEntityIndex, m_iHltvReplayEntity);
    SCHEMA_FIELD(CUtlVector<sndopvarlatchdata_t>, m_sndOpvarLatchData);

public:
    /// <summary>Force suicide.</summary>
    void CommitSuicide(bool bExplode, bool bForce, HookChain eChain = HookChain::Run);
    /// <summary>Snaps the pawn's view angles. No-op where the engine function could not be resolved.</summary>
    void SnapViewAngles(const QAngle& angEyeAngles, HookChain eChain = HookChain::Run);
    /// <summary>Get controller.</summary>
    CCSPlayerController* GetController();
    /// <summary>Get default controller.</summary>
    CCSPlayerController* GetDefaultController();

public:
    static CBasePlayerPawn* New(const char* className)
    {
        return CBaseEntity::New<CBasePlayerPawn>(className);
    }

    static CBasePlayerPawn* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBasePlayerPawn>(iIndex);
    }

    static CBasePlayerPawn* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBasePlayerPawn> GetHandle()
    {
        return CBaseEntity::GetHandle<CBasePlayerPawn>();
    }
};

#endif // _INCLUDE_CBASEPLAYERPAWN_H
