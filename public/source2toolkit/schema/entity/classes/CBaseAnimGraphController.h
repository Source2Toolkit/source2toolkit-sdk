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

#ifndef _INCLUDE_CBASEANIMGRAPHCONTROLLER_H
#define _INCLUDE_CBASEANIMGRAPHCONTROLLER_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/AnimationAlgorithm_t.h"
#include "../enums/AnimLoopMode_t.h"
#include "../enums/SequenceFinishNotifyState_t.h"
#include "AnimGraph2SerializedPoseRecipeSlot_t.h"
#include "CAnimGraph2InstancePtr.h"
#include "CExternalAnimGraphList.h"
#include "CSkeletonAnimationController.h"
#include "ExternalAnimGraphHandle_t.h"
#include "ResourceId_t.h"
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

class CBaseAnimGraph;
class InfoForResourceTypeCNmGraphDefinition;

class CBaseAnimGraphController : public CSkeletonAnimationController
{
public:
    DECLARE_SCHEMA_CLASS_INLINE(CBaseAnimGraphController);

    SCHEMA_FIELD(AnimationAlgorithm_t, m_nAnimationAlgorithm);
    SCHEMA_FIELD(ExternalAnimGraphHandle_t, m_nNextExternalGraphHandle);
    SCHEMA_FIELD(CUtlVector<CGlobalSymbol>, m_vecSecondarySkeletonSlotIDs);
    SCHEMA_FIELD(CUtlVector<CHandle<CBaseAnimGraph>>, m_vecSecondarySkeletons);
    SCHEMA_FIELD(int32_t, m_nSecondarySkeletonMasterCount);
    SCHEMA_FIELD(float, m_flSoundSyncTime);
    SCHEMA_FIELD(uint32_t, m_nActiveIKChainMask);
    SCHEMA_FIELD(int32_t, m_hSequence);
    SCHEMA_FIELD(float, m_flSeqStartTime);
    SCHEMA_FIELD(float, m_flSeqFixedCycle);
    SCHEMA_FIELD(AnimLoopMode_t, m_nAnimLoopMode);
    SCHEMA_FIELD(float, m_flPlaybackRate);
    SCHEMA_FIELD(SequenceFinishNotifyState_t, m_nNotifyState);
    SCHEMA_FIELD(bool, m_bNetworkedAnimationInputsChanged);
    SCHEMA_FIELD(bool, m_bNetworkedSequenceChanged);
    SCHEMA_FIELD(bool, m_bLastUpdateSkipped);
    SCHEMA_FIELD(bool, m_bSequenceFinished);
    SCHEMA_FIELD(int32_t, m_nPrevAnimUpdateTick);
    SCHEMA_FIELD(CStrongHandle<InfoForResourceTypeCNmGraphDefinition>, m_hGraphDefinitionAG2);
    SCHEMA_FIELD(CUtlVector<AnimGraph2SerializedPoseRecipeSlot_t>, m_SerializePoseRecipeAG2Slots);
    SCHEMA_FIELD(CUtlVector<uint8_t>, m_SerializePoseRecipeAG2Dynamic);
    SCHEMA_FIELD(uint32_t, m_nSerializePoseRecipeAG2ActiveSlot);
    SCHEMA_FIELD(int32_t, m_nSerializePoseRecipeVersionAG2);
    SCHEMA_FIELD(int32_t, m_nServerGraphInstanceIteration);
    SCHEMA_FIELD(int32_t, m_nServerSerializationContextIteration);
    SCHEMA_FIELD(ResourceId_t, m_primaryGraphId);
    SCHEMA_FIELD(CUtlVector<ResourceId_t>, m_vecExternalGraphIds);
    SCHEMA_FIELD(CUtlVector<ResourceId_t>, m_vecExternalClipIds);
    SCHEMA_FIELD(CGlobalSymbol, m_sAnimGraph2Identifier);
    SCHEMA_FIELD(CAnimGraph2InstancePtr, m_pGraphInstanceAG2);
    SCHEMA_FIELD(CExternalAnimGraphList, m_vecExternalGraphs);
};

#endif // _INCLUDE_CBASEANIMGRAPHCONTROLLER_H
