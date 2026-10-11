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

#ifndef _INCLUDE_CBASEMODELENTITY_H
#define _INCLUDE_CBASEMODELENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseEntity.h"
#include "CBaseModelEntity__BodyGroupRequest_t.h"
#include "CCollisionProperty.h"
#include "CGlowProperty.h"
#include "CHitboxComponent.h"
#include "CNetworkViewOffsetVector.h"
#include "EntityRenderAttribute_t.h"
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

class CChoreoComponent;
class CDestructiblePartsComponent;
class CRenderComponent;

class CBaseModelEntity : public CBaseEntity
{
public:
    DECLARE_SCHEMA_CLASS(CBaseModelEntity);

    SCHEMA_FIELD(CRenderComponent*, m_CRenderComponent);
    SCHEMA_FIELD(CHitboxComponent, m_CHitboxComponent);
    SCHEMA_FIELD(CChoreoComponent*, m_pChoreoComponent);
    SCHEMA_FIELD(HitGroup_t, m_nDestructiblePartInitialStateDestructed0);
    SCHEMA_FIELD(HitGroup_t, m_nDestructiblePartInitialStateDestructed1);
    SCHEMA_FIELD(HitGroup_t, m_nDestructiblePartInitialStateDestructed2);
    SCHEMA_FIELD(HitGroup_t, m_nDestructiblePartInitialStateDestructed3);
    SCHEMA_FIELD(HitGroup_t, m_nDestructiblePartInitialStateDestructed4);
    SCHEMA_FIELD(int32_t, m_nDestructiblePartInitialStateDestructed0_PartIndex);
    SCHEMA_FIELD(int32_t, m_nDestructiblePartInitialStateDestructed1_PartIndex);
    SCHEMA_FIELD(int32_t, m_nDestructiblePartInitialStateDestructed2_PartIndex);
    SCHEMA_FIELD(int32_t, m_nDestructiblePartInitialStateDestructed3_PartIndex);
    SCHEMA_FIELD(int32_t, m_nDestructiblePartInitialStateDestructed4_PartIndex);
    SCHEMA_FIELD(bool, m_bDestructiblePartInitialStateDestructed0_GenerateBreakpieces);
    SCHEMA_FIELD(bool, m_bDestructiblePartInitialStateDestructed1_GenerateBreakpieces);
    SCHEMA_FIELD(bool, m_bDestructiblePartInitialStateDestructed2_GenerateBreakpieces);
    SCHEMA_FIELD(bool, m_bDestructiblePartInitialStateDestructed3_GenerateBreakpieces);
    SCHEMA_FIELD(bool, m_bDestructiblePartInitialStateDestructed4_GenerateBreakpieces);
    SCHEMA_FIELD(CDestructiblePartsComponent*, m_pDestructiblePartsSystemComponent);
    SCHEMA_FIELD(float, m_flDissolveStartTime);
    SCHEMA_FIELD(CEntityIOOutput, m_OnIgnite);
    SCHEMA_FIELD(RenderMode_t, m_nRenderMode);
    SCHEMA_FIELD(RenderFx_t, m_nRenderFX);
    SCHEMA_FIELD(bool, m_bAllowFadeInView);
    SCHEMA_FIELD(Color, m_clrRender);
    SCHEMA_FIELD(CUtlVector<EntityRenderAttribute_t>, m_vecRenderAttributes);
    SCHEMA_FIELD(bool, m_bRenderToCubemaps);
    SCHEMA_FIELD(bool, m_bExpandRenderBoundsToIncludeCloth);
    SCHEMA_FIELD(bool, m_bNoInterpolate);
    SCHEMA_FIELD(CCollisionProperty, m_Collision);
    SCHEMA_FIELD(CGlowProperty, m_Glow);
    SCHEMA_FIELD(float, m_flGlowBackfaceMult);
    SCHEMA_FIELD(float, m_fadeMinDist);
    SCHEMA_FIELD(float, m_fadeMaxDist);
    SCHEMA_FIELD(float, m_flFadeScale);
    SCHEMA_FIELD(float, m_flShadowStrength);
    SCHEMA_FIELD(uint8_t, m_nObjectCulling);
    SCHEMA_FIELD(uint32_t, m_bodyGroupTotalRequestCount);
    SCHEMA_FIELD(CUtlVector<CBaseModelEntity__BodyGroupRequest_t>, m_bodyGroupRequests);
    SCHEMA_FIELD(CNetworkViewOffsetVector, m_vecViewOffset);
    SCHEMA_FIELD_POINTER(uint32_t, m_bvDisabledHitGroups);

public:
    /// <summary>Get model name.</summary>
    CUtlSymbolLarge GetModelName();
    /// <summary>Get eye position.</summary>
    Vector GetEyePosition();
    /// <summary>Set entity model.</summary>
    void SetModel(const char* pszModel, HookChain eChain = HookChain::Run);
    /// <summary>Switch a bodygroup on the model.</summary>
    void SetBodyGroup(const char* pszName, int nValue);

public:
    static CBaseModelEntity* New(const char* className)
    {
        return CBaseEntity::New<CBaseModelEntity>(className);
    }

    static CBaseModelEntity* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CBaseModelEntity>(iIndex);
    }

    static CBaseModelEntity* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CBaseModelEntity> GetHandle()
    {
        return CBaseEntity::GetHandle<CBaseModelEntity>();
    }
};

#endif // _INCLUDE_CBASEMODELENTITY_H
