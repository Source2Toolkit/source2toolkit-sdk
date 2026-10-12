"""
Source2Toolkit
Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
All rights reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 3.0, as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.

As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
gives you permission to link the code of this program
(as well as its derivative works) to "Counter-Strike 2," "Source 2,"
"Steam," and any Game MODs or server software running on software by
Valve Corporation. You must obey the GNU General Public License in all
respects for all other code used.

Additionally, this exception applies to all derivative works unless
otherwise stated in LICENSE.txt.

Authors:
    - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl

Project: Source2Toolkit

Schema generator – Python port of the C# schemagen tool.
Original: tools/schemagen/Program.cs (and related files)

Usage:
    python generator.py [output_path]

Default output_path: ../../public/source2toolkit/schema/entity
Reads server.json from the same directory as this script.
"""

from __future__ import annotations

import json
import os
import re
import sys
from collections import deque
from dataclasses import dataclass, field
from enum import Enum, auto
from typing import Optional

LICENSE_HEADER = """\
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

"""

# ---------------------------------------------------------------------------
# Enums
# ---------------------------------------------------------------------------

class SchemaTypeCategory(Enum):
    Builtin = 0
    Ptr = 1
    Bitfield = 2
    FixedArray = 3
    Atomic = 4
    DeclaredClass = 5
    DeclaredEnum = 6
    NoneType = 7

class SchemaAtomicCategory(Enum):
    Basic = 0
    T = 1
    Collection = 2
    TT = 3
    I = 4
    Unknown = 5
    NoneAtomic = 6

# ---------------------------------------------------------------------------
# JSON input model
# ---------------------------------------------------------------------------

@dataclass
class SchemaMetaTag:
    name: str
    value: Optional[str]

@dataclass
class SchemaBaseClass:
    offset: int
    ref_idx: int

@dataclass
class SchemaSubtype:
    type: str
    name: Optional[str]
    size: Optional[int]
    alignment: Optional[int]
    template: Optional[list[SchemaSubtype]]
    ref_idx: Optional[int]
    element_size: Optional[int]
    count: Optional[int]
    subtype: Optional[SchemaSubtype]

    @staticmethod
    def from_dict(d: dict) -> SchemaSubtype:
        template = [SchemaSubtype.from_dict(t) for t in d["template"]] if d.get("template") else None
        subtype = SchemaSubtype.from_dict(d["subtype"]) if d.get("subtype") else None
        return SchemaSubtype(
            type=d.get("type", ""),
            name=d.get("name"),
            size=d.get("size"),
            alignment=d.get("alignment"),
            template=template,
            ref_idx=d.get("ref_idx"),
            element_size=d.get("element_size"),
            count=d.get("count"),
            subtype=subtype,
        )

@dataclass
class MemberTraits:
    metatags: Optional[list[SchemaMetaTag]]
    subtype: Optional[SchemaSubtype]

@dataclass
class SchemaMember:
    name: str
    offset: int
    traits: Optional[MemberTraits]

@dataclass
class SchemaEnumField:
    name: str
    value: int

@dataclass
class SchemaTraits:
    baseclasses: Optional[list[SchemaBaseClass]]
    members: Optional[list[SchemaMember]]
    fields: Optional[list[SchemaEnumField]]

@dataclass
class SchemaDef:
    type: str
    name: str
    size: Optional[int]
    alignment: Optional[int]
    traits: Optional[SchemaTraits]

def _parse_schema_defs(raw_defs: list[dict]) -> list[SchemaDef]:
    result = []
    for d in raw_defs:
        traits = None
        if raw_traits := d.get("traits"):
            baseclasses = None
            if raw_bc := raw_traits.get("baseclasses"):
                baseclasses = [SchemaBaseClass(b["offset"], b["ref_idx"]) for b in raw_bc]
            members = None
            if raw_members := raw_traits.get("members"):
                members = []
                for m in raw_members:
                    mt = None
                    if raw_mt := m.get("traits"):
                        metatags = None
                        if raw_tags := raw_mt.get("metatags"):
                            metatags = [SchemaMetaTag(t["name"], t.get("value")) for t in raw_tags]
                        sub = SchemaSubtype.from_dict(raw_mt["subtype"]) if raw_mt.get("subtype") else None
                        mt = MemberTraits(metatags, sub)
                    members.append(SchemaMember(m["name"], m["offset"], mt))
            enum_fields = None
            if raw_fields := raw_traits.get("fields"):
                enum_fields = [SchemaEnumField(f["name"], f["value"]) for f in raw_fields]
            traits = SchemaTraits(baseclasses, members, enum_fields)
        result.append(SchemaDef(d["type"], d["name"], d.get("size"), d.get("alignment"), traits))
    return result

# ---------------------------------------------------------------------------
# Intermediate model
# ---------------------------------------------------------------------------

@dataclass
class SchemaEnumItem:
    name: str
    value: int

@dataclass
class SchemaEnum:
    align: int
    items: list[SchemaEnumItem]

@dataclass
class SchemaFieldType:
    name: str
    category: SchemaTypeCategory
    atomic: Optional[SchemaAtomicCategory]
    inner: Optional[SchemaFieldType]

    def __post_init__(self):
        # Type normalizations
        if self.name == "VectorWS":
            self.name = "Vector"

        if self.name == "CTransformWS":
            self.name = "CTransform"
        elif self.name == "CNetworkVarChainer":
            self.name = "CNetworkVarChainer"

        if self.name in ("GameTime_t", "CNetworkedQuantizedFloat"):
            self.category = SchemaTypeCategory.Builtin
            self.name = "float32"
        elif self.name in ("CPlayerSlot", "HSequence", "CSplitScreenSlot", "GameTick_t"):
            self.category = SchemaTypeCategory.Builtin
            self.name = "int32"
        elif self.name == "AmmoIndex_t":
            self.category = SchemaTypeCategory.Builtin
            self.name = "uint8"
        elif self.name == "CBitVec":
            self.name = "CBitVec<64>"
        elif self.name == "CTypedBitVec":
            self.name = "CTypedBitVec<64>"

    @property
    def is_string(self) -> bool:
        return (
                self.category in (SchemaTypeCategory.FixedArray, SchemaTypeCategory.Ptr)
                and self.inner is not None
                and self.inner.category == SchemaTypeCategory.Builtin
                and self.inner.name == "char"
        )

    @property
    def is_declared(self) -> bool:
        return self.category in (SchemaTypeCategory.DeclaredClass, SchemaTypeCategory.DeclaredEnum)

    @property
    def array_size(self) -> Optional[int]:
        if self.category == SchemaTypeCategory.FixedArray:
            m = re.search(r"\[(\d+)\]$", self.name)
            return int(m.group(1)) if m else None
        return None

    @staticmethod
    def _builtin_to_cpp(name: str) -> str:
        return {
            "float32": "float",
            "float64": "double",
            "int8": "int8_t",
            "int16": "int16_t",
            "int32": "int32_t",
            "int64": "int64_t",
            "uint8": "uint8_t",
            "uint16": "uint16_t",
            "uint32": "uint32_t",
            "uint64": "uint64_t",
            "bool": "bool",
            "char": "char",
        }.get(name, name)

    @staticmethod
    def _atomic_to_cpp(name: str, atomic: SchemaAtomicCategory, inner: Optional[SchemaFieldType]) -> str:
        if atomic == SchemaAtomicCategory.Basic:
            return name
        if atomic == SchemaAtomicCategory.T:
            base = name.split("<")[0]
            return f"{base}<{inner.cpp_type_name}>"
        if atomic == SchemaAtomicCategory.Collection:
            return f"CUtlVector<{inner.cpp_type_name}>"
        return name

    @property
    def cpp_type_name(self) -> str:
        cat = self.category
        if cat == SchemaTypeCategory.Builtin:
            return self._builtin_to_cpp(self.name)
        if cat in (SchemaTypeCategory.DeclaredClass, SchemaTypeCategory.DeclaredEnum):
            return self.name
        if cat == SchemaTypeCategory.Atomic:
            return self._atomic_to_cpp(self.name, self.atomic, self.inner)
        if cat == SchemaTypeCategory.FixedArray:
            return self.inner.cpp_type_name
        if cat == SchemaTypeCategory.Ptr:
            return self.inner.cpp_type_name + "*"
        return "void"

@dataclass
class SchemaField:
    name: str
    type: SchemaFieldType
    metadata: dict[str, str]

@dataclass
class SchemaClass:
    index: int
    name: str
    parent: Optional[str]
    fields: list[SchemaField]

# ---------------------------------------------------------------------------
# Configuration – lists copied verbatim from C# source
# ---------------------------------------------------------------------------

IGNORE_CLASSES: frozenset[str] = frozenset({
    "GameTime_t",
    "GameTick_t",
    "AttachmentHandle_t",
    "CGameSceneNodeHandle",
    "HSequence",
    "CAttributeManager::cached_attribute_float_t",
    "QuestProgress::Reason",
    "IChoreoServices::ScriptState_t",
    "IChoreoServices::ChoreoState_t",
    "SpawnPointCoopEnemy::BotDefaultBehavior_t",
    "CLogicBranchList::LogicBranchListenerLastState_t",
    "SimpleConstraintSoundProfile::SimpleConstraintsSoundProfileKeypoints_t",
    "MoodAnimationLayer_t",
    "SoundeventPathCornerPairNetworked_t",
    "AISound_t",
    "CAttachmentNameSymbolWithStorage",
    "CAnimGraph2ParamAutoResetOptionalRef",
})

IGNORE_CLASS_WILDCARDS: tuple[str, ...] = (
    "CResourceNameTyped",
    "CEntityOutputTemplate",
    "CVariantBase",
    "HSCRIPT",
    "KeyValues3",
    "Unknown",
    "CUtlOrderedMap",
    "CAnimGraph2ParamOptionalRef",
    "CUtlHashtable",
    "CSmartPtr",
    "CUtlLeanVector"
)

HARD_SKIP_CLASSES: frozenset[str] = frozenset({
    "CEntityComponent",
    "CEntityIdentity",
    "CEntityInstance",
    "CEntityIOOutput",
    "ChangeAccessorFieldPathIndex_t",
    "CScriptComponent",
    "CVariantDefaultAllocator",
})

HARD_SKIP_ENUMS: frozenset[str] = frozenset({
    # s2sdk declares these unscoped (shareddefs.h / const.h). Emitting scoped
    # versions here makes any translation unit that reaches both fail to compile,
    # so the game's own definitions are used instead -- see SKIPPED_ENUM_HEADERS.
    "Class_T",
    "DamageTypes_t",
    "TakeDamageFlags_t",
    "EntityDissolveType_t",
    "EntityEffects_t",
    "ItemFlagTypes_t",
    "ObserverMode_t",
    "ENetworkDisconnectionReason",
    "EntityDormancyType_t",
    "fieldtype_t",
    "Flags_t",
    "HitGroup_t",
    "LifeState_t",
    "MoveCollide_t",
    "MoveType_t",
    "RenderFx_t",
    "RenderMode_t",
    "RenderMultisampleType_t",
    "SolidType_t",
    "soundlevel_t",
    "ThreeState_t"
})

# A hard-skipped enum still has to be declared for the fields that use it.
# Most arrive through headers the generated files already include; these do not,
# so the header that declares them is emitted alongside.
SKIPPED_ENUM_HEADERS: dict[str, str] = {
    "Class_T": "shareddefs.h",
    "DamageTypes_t": "shareddefs.h",
    "EntityDissolveType_t": "shareddefs.h",
    "EntityEffects_t": "const.h",
    "ItemFlagTypes_t": "shareddefs.h",
    "ObserverMode_t": "shareddefs.h",
    "TakeDamageFlags_t": "shareddefs.h",
}

NETWORK_CLASSES: list[str] = [
    "CAK47", "CBarnLight", "CBaseAnimGraph", "CBaseButton", "CBaseClientUIEntity",
    "CBaseCombatCharacter", "CBaseCSGrenade", "CBaseCSGrenadeProjectile", "CBaseDoor",
    "CBaseEntity", "CBaseFlex", "CBaseGrenade", "CBaseModelEntity", "CBasePlayerController",
    "CBasePlayerPawn", "CBasePlayerWeapon", "CBasePropDoor", "CBaseToggle", "CBaseTrigger",
    "CBeam", "CBombTarget", "CBreakable", "CBreakableProp", "CC4", "CChicken",
    "CColorCorrection", "CColorCorrectionVolume", "CCSGameRulesProxy",
    "CCSGO_TeamIntroCharacterPosition", "CCSGO_TeamIntroCounterTerroristPosition",
    "CCSGO_TeamIntroTerroristPosition", "CCSGO_TeamPreviewCharacterPosition",
    "CCSGO_TeamSelectCharacterPosition", "CCSGO_TeamSelectCounterTerroristPosition",
    "CCSGO_TeamSelectTerroristPosition", "CCSGO_WingmanIntroCharacterPosition",
    "CCSGO_WingmanIntroCounterTerroristPosition", "CCSGO_WingmanIntroTerroristPosition",
    "CCSMinimapBoundary", "CCSObserverPawn", "CCSPlayerController", "CCSPlayerPawn",
    "CCSPlayerPawnBase", "CCSPlayerResource", "CCSTeam", "CCSWeaponBase", "CCSWeaponBaseGun",
    "CDEagle", "CDecoyGrenade", "CDecoyProjectile", "CDynamicLight", "CDynamicProp",
    "CEconEntity", "CEconWearable", "CEntityDissolve", "CEntityFlame",
    "CEnvCombinedLightProbeVolume", "CEnvCubemap", "CEnvCubemapBox", "CEnvCubemapFog",
    "CEnvDecal", "CEnvDetailController", "CEnvLightProbeVolume", "CEnvParticleGlow",
    "CEnvSky", "CEnvVolumetricFogController", "CEnvVolumetricFogVolume", "CEnvWind",
    "CFireCrackerBlast", "CFish", "CFlashbang", "CFogController", "CFootstepControl",
    "CFuncBrush", "CFuncConveyor", "CFuncElectrifiedVolume", "CFuncLadder", "CFuncMonitor",
    "CFuncMoveLinear", "CFuncRotating", "CFuncTrackTrain", "CFuncWater", "CGameRulesProxy",
    "CGradientFog", "CHandleTest", "CHEGrenade", "CHostage", "CHostageCarriableProp",
    "CHostageRescueZone", "CIncendiaryGrenade", "CInferno",
    "CInfoInstructorHintHostageRescueZone", "CInfoLadderDismount",
    "CInfoOffscreenPanoramaTexture", "CInfoVisibilityBox", "CInfoWorldLayer",
    "CItem_Healthshot", "CItemDogtags", "CKnife", "CLightDirectionalEntity", "CLightEntity",
    "CLightEnvironmentEntity", "CLightOrthoEntity", "CLightSpotEntity",
    "CMapVetoPickController", "CModelPointEntity", "CMolotovGrenade", "CMolotovProjectile",
    "COmniLight", "CParticleSystem", "CPathParticleRope", "CPhysBox", "CPhysicsProp",
    "CPhysicsPropMultiplayer", "CPhysMagnet", "CPlantedC4", "CPlayerPing",
    "CPlayerSprayDecal", "CPlayerVisibility", "CPointCamera", "CPointClientUIDialog",
    "CPointClientUIWorldPanel", "CPointClientUIWorldTextPanel", "CPointCommentaryNode",
    "CPointEntity", "CPointValueRemapper", "CPointWorldText", "CPostProcessingVolume",
    "CPrecipitation", "CPrecipitationBlocker", "CRagdollManager", "CRagdollProp",
    "CRagdollPropAttached", "CRectLight", "CRopeKeyframe", "CSceneEntity",
    "CShatterGlassShardPhysics", "CSkyCamera", "CSmokeGrenade", "CSmokeGrenadeProjectile",
    "CSoundAreaEntityBase", "CSoundAreaEntityOrientedBox", "CSoundAreaEntitySphere",
    "CSoundOpvarSetAABBEntity", "CSoundOpvarSetOBBEntity", "CSoundOpvarSetOBBWindEntity",
    "CSoundOpvarSetPathCornerEntity", "CSoundOpvarSetPointBase", "CSoundOpvarSetPointEntity",
    "CSpotlightEnd", "CSprite", "CSpriteOriented", "CTeam", "CTextureBasedAnimatable",
    "CTonemapController2", "CTriggerBuoyancy", "CTriggerVolume", "CVoteController",
    "CWaterBullet", "CWeaponAug", "CWeaponAWP", "CWeaponBaseItem", "CWeaponBizon",
    "CWeaponElite", "CWeaponFamas", "CWeaponFiveSeven", "CWeaponG3SG1", "CWeaponGalilAR",
    "CWeaponGlock", "CWeaponHKP2000", "CWeaponM249", "CWeaponM4A1", "CWeaponMAC10",
    "CWeaponMag7", "CWeaponMP7", "CWeaponMP9", "CWeaponNegev", "CWeaponNOVA",
    "CWeaponP250", "CWeaponP90", "CWeaponSawedoff", "CWeaponSCAR20", "CWeaponSG556",
    "CWeaponSSG08", "CWeaponTaser", "CWeaponTec9", "CWeaponUMP45", "CWeaponXM1014",
    "CWorld",
]

# Types a class only mentions through its MANUAL_METHODS. The include collector
# walks schema types only, so a hand-added declaration is silently dropped on
# the next regeneration -- these survive it.
#
# Forward declarations, not includes: the headers only ever use these behind a
# pointer. The .cpp that defines the method includes the real header.
MANUAL_FORWARDS: dict[str, list[str]] = {
    "CBaseEntity": ["IRecipientFilter"],
    "CBasePlayerController": ["CCSPlayerPawn", "CServerSideClient"],
    "CBasePlayerPawn": ["CCSPlayerController"],
    "CCSCustomHudLayout": ["CCSPlayerController"],
    "CGameSceneNode": ["CSkeletonInstance"],
    "CDecoyProjectile": ["CBaseEntity"],
    "CFlashbangProjectile": ["CBaseEntity"],
    "CHEGrenadeProjectile": ["CBaseEntity"],
    "CMolotovProjectile": ["CBaseEntity"],
    "CSmokeGrenadeProjectile": ["CBaseEntity"],
}

MANUAL_METHODS: dict[str, list[str]] = {
    "CAttributeList": [
        "/// <summary>Sets an attribute by name, adding it to the list when it is not there yet.</summary>",
        "void SetOrAddAttribute(const char* pszAttributeName, float flValue, HookChain eChain = HookChain::Run);",
        "/// <summary>CAttributeList::SetOrAddAttributeValueByName, the engine's name for SetOrAddAttribute.</summary>",
        "void SetOrAddAttributeValueByName(const char* pszAttributeName, float flValue, HookChain eChain = HookChain::Run);",
    ],
    "CCSWeaponBaseVData": [
        "/// <summary>Looks weapon VData up by its key, e.g. \"weapon_ak47\". A weapon type other than -1 also requires the data to be of that type. Null when not found.</summary>",
        "static CCSWeaponBaseVData* FromKey(const char* pszKey, int nWeaponType = -1);",
    ],
    "CGameSceneNode": [
        "/// <summary>Gets the skeleton instance of this scene node, or null when it has none.</summary>",
        "CSkeletonInstance* GetSkeletonInstance();",
    ],
    "CDecoyProjectile": [
        "/// <summary>Spawns a live grenade of this type. Ported from SwiftlyS2.</summary>",
        "static CDecoyProjectile* EmitGrenade(const Vector& vecPosition, const QAngle& angAngle, const Vector& vecVelocity, CBaseEntity* pOwner, uint32_t nItemDefIndex, HookChain eChain = HookChain::Run);",
    ],
    "CFlashbangProjectile": [
        "/// <summary>Spawns a live grenade of this type. Ported from SwiftlyS2.</summary>",
        "static CFlashbangProjectile* EmitGrenade(const Vector& vecPosition, const QAngle& angAngle, const Vector& vecVelocity, CBaseEntity* pOwner, uint32_t nItemDefIndex, HookChain eChain = HookChain::Run);",
    ],
    "CHEGrenadeProjectile": [
        "/// <summary>Spawns a live grenade of this type. Ported from SwiftlyS2.</summary>",
        "static CHEGrenadeProjectile* EmitGrenade(const Vector& vecPosition, const QAngle& angAngle, const Vector& vecVelocity, CBaseEntity* pOwner, uint32_t nItemDefIndex, HookChain eChain = HookChain::Run);",
    ],
    "CMolotovProjectile": [
        "/// <summary>Spawns a live grenade of this type. Ported from SwiftlyS2.</summary>",
        "static CMolotovProjectile* EmitGrenade(const Vector& vecPosition, const QAngle& angAngle, const Vector& vecVelocity, CBaseEntity* pOwner, uint32_t nItemDefIndex, HookChain eChain = HookChain::Run);",
    ],
    "CSmokeGrenadeProjectile": [
        "/// <summary>Spawns a live grenade of this type. Ported from SwiftlyS2.</summary>",
        "static CSmokeGrenadeProjectile* EmitGrenade(const Vector& vecPosition, const QAngle& angAngle, const Vector& vecVelocity, int nTeam, CBaseEntity* pOwner, uint32_t nItemDefIndex, HookChain eChain = HookChain::Run);",
    ],
    "CCSCustomHudLayout": [
        "/// <summary>Creates and spawns a custom_hud_layout for a panorama layout, e.g. \"my_panel\" for panorama/layout/custom_game/my_panel.vxml_c.</summary>",
        "static CCSCustomHudLayout* Create(const char* pszLayout, const char* pszTargetName = nullptr);",
        "/// <summary>Gets the layout state of one player, or the global state when no controller is given.</summary>",
        "CCSCustomHudLayoutState& GetLayoutState(CCSPlayerController* pController = nullptr);",
        "/// <summary>Adds or removes a CSS class on a panel of the layout.</summary>",
        "void SetHasClass(const char* pszPanelId, const char* pszClassName, bool bHasClass, CCSPlayerController* pController = nullptr);",
        "/// <summary>Sets a dialog variable string on a panel of the layout.</summary>",
        "void SetDialogVariableString(const char* pszPanelId, const char* pszVariableName, const char* pszValue, CCSPlayerController* pController = nullptr);",
        "/// <summary>Enables or disables mouse input capture, without which the player cannot click the layout.</summary>",
        "void SetInputCaptureEnabled(bool bEnable, CCSPlayerController* pController = nullptr);",
        "/// <summary>Whether mouse input capture is enabled.</summary>",
        "bool IsInputCaptureEnabled(CCSPlayerController* pController = nullptr);",
        "/// <summary>Registers a callback fired when a player clicks a panel of this layout, keyed by the panel\'s id attribute.</summary>",
        "void HookClick(std::function<void(CCSPlayerController*, CCSCustomHudLayout*, const char*)> callback);",
        "/// <summary>Drops every click callback registered for this layout.</summary>",
        "void UnhookClicks();",
    ],
    "CBaseEntity": [
        "/// <summary>Creates entity by classname.</summary>",
        "static CBaseEntity* CreateEntityByName(const char* pszClassName);",
        "/// <summary>Create entity by classname.</summary>",
        "template<typename T>\n    static T* New(const char* pszClassName)\n    {    \n        return reinterpret_cast<T*>(CreateEntityByName(pszClassName));\n    }",
        "/// <summary>Get entity by index.</summary>",
        "template<typename T>\n    static T* FromIndex(int iIndex)\n    {    \n        return reinterpret_cast<T*>(GetEntitySystem()->GetEntityInstance(CEntityIndex(iIndex)));\n    }",
        "/// <summary>Get entity by entity index.</summary>",
        "template<typename T>\n    static T* FromIndex(CEntityIndex index)\n    {    \n        return FromIndex<T>(index.Get());\n    }",
        "/// <summary>Accepts entity input.</summary>",
        'void AcceptInput(const char* pszInput, CEntityInstance* pActivator = nullptr, CEntityInstance* pCaller = nullptr, const char* pszValue = "", HookChain eChain = HookChain::Run);',
        "/// <summary>Add delayed entity IO event.</summary>",
        'void AddEntityIOEvent(const char* pszInput, CEntityInstance* pActivator = nullptr, CEntityInstance* pCaller = nullptr, const char* pszValue = "", float flDelay = 0.0f, HookChain eChain = HookChain::Run);',
        "/// <summary>Add signle entity IO listener.</summary>",
        "CEntityIOListenerHandle* AddSingleEntityIOListener(const char* pszOutput, std::function<Action(const char*, CEntityInstance*, CEntityInstance*, float, bool)> callback, bool post);",
        "/// <summary>Get absolute origin.</summary>",
        "Vector GetAbsOrigin();",
        "/// <summary>Get local rotation.</summary>",
        "QAngle GetAngRotation();",
        "/// <summary>Get absolute rotation.</summary>",
        "QAngle GetAbsRotation();",
        "/// <summary>Get absolute velocity.</summary>",
        "Vector GetAbsVelocity();",
        "/// <summary>Set absolute origin.</summary>",
        "void SetAbsOrigin(Vector vecOrigin);",
        "/// <summary>Set absolute rotation.</summary>",
        "void SetAbsRotation(QAngle angAbsRotation);",
        "/// <summary>Set local rotation.</summary>",
        "void SetAngRotation(QAngle angRotation);",
        "/// <summary>Set absolute velocity.</summary>",
        "void SetAbsVelocity(Vector vecVelocity);",
        "/// <summary>Set base velocity.</summary>",
        "void SetBaseVelocity(Vector vecVelocity);",
        "/// <summary>Get entity VData.</summary>",
        "CEntitySubclassVDataBase* GetVData();",
        "/// <summary>Spawn entity.</summary>",
        "void DispatchSpawn(CEntityKeyValues* pEntityKeyValues = nullptr, HookChain eChain = HookChain::Run);",
        "/// <summary>Teleport entity.</summary>",
        "void Teleport(const Vector* pPosition, const QAngle* pAngles, const Vector* pVelocity, HookChain eChain = HookChain::Run);",
        "/// <summary>Set move type.</summary>",
        "void SetMoveType(MoveType_t nMoveType);",
        "/// <summary>Get collision group.</summary>",
        "uint8 GetCollisionGroup();",
        "/// <summary>Set collision group.</summary>",
        "void SetCollisionGroup(uint8 nCollisionGroup);",
        "/// <summary>Notify collision rules changed.</summary>",
        "void CollisionRulesChanged();",
        "/// <summary>Get entity index.</summary>",
        "int GetIndex();",
        "/// <summary>Get entity handle. Templated the same way New() is: every generated\n    /// entity class shadows it with a plain GetHandle() returning CHandle of its own\n    /// type, so player->GetHandle() on a CCSPlayerController* is a CHandle<CCSPlayerController>.</summary>",
        "template<typename T = CBaseEntity>\n    CHandle<T> GetHandle()\n    {    \n        return CHandle<T>(GetRefEHandle());\n    }",
        "/// <summary>Get entity name.</summary>",
        "const char* GetName() const;",
        "/// <summary>Removes the entity (UTIL_Remove).</summary>",
        "void Remove(HookChain eChain = HookChain::Run);",
        "/// <summary>Dispatches a particle effect on this entity. A null filter sends it to everyone.</summary>",
        "void DispatchParticleEffect(const char* pszParticleName, ParticleAttachment_t nAttachType = ParticleAttachment_t::PATTACH_ABSORIGIN_FOLLOW, uint8 nAttachmentPoint = 0, bool bResetAllParticlesOnEntity = false, IRecipientFilter* pFilter = nullptr, HookChain eChain = HookChain::Run);",
    ],
    "CBaseModelEntity": [
        "/// <summary>Get model name.</summary>",
        "CUtlSymbolLarge GetModelName();",
        "/// <summary>Get eye position.</summary>",
        "Vector GetEyePosition();",
        "/// <summary>Set entity model.</summary>",
        "void SetModel(const char* pszModel, HookChain eChain = HookChain::Run);",
        "/// <summary>Switch a bodygroup on the model.</summary>",
        "void SetBodyGroup(const char* pszName, int nValue);",
    ],
    "CBasePlayerController": [
        "/// <summary>Set pawn for controller.</summary>",
        "void SetPawn(CBasePlayerPawn* pPawn, HookChain eChain = HookChain::Run);",
        "/// <summary>Get pawn.</summary>",
        "CCSPlayerPawn* GetPawn();",
        "/// <summary>Print to console.</summary>",
        "void PrintToConsole(const char* pszMessage);",
        "/// <summary>Print to chat.</summary>",
        "void PrintToChat(const char* pszMessage);",
        "/// <summary>Print to center.</summary>",
        "void PrintToCenter(const char* pszMessage);",
        "/// <summary>Print alert.</summary>",
        "void PrintToCenterAlert(const char* pszMessage);",
        "/// <summary>Is bot.</summary>",
        "bool IsBot();",
        "/// <summary>Disconnect player.</summary>",
        "void Disconnect(ENetworkDisconnectionReason eReason, const char* pszInternalReason = nullptr);",
        "/// <summary>Execute client command.</summary>",
        "void ExecuteClientCommand(const char* pszCommand);",
        "/// <summary>Execute command from server.</summary>",
        "void ExecuteClientCommandFromServer(const char* pszCommand);",
        "/// <summary>Get server side engine client.</summary>",
        "CServerSideClient* GetServerSideClient();",
        "/// <summary>Get player index.</summary>",
        "CEntityIndex GetPlayerIndex();",
        "/// <summary>Get slot.</summary>",
        "int GetSlot();",
        "/// <summary>Get player slot.</summary>",
        "CPlayerSlot GetPlayerSlot();",
        "/// <summary>Get steamid.</summary>",
        "int GetUserID();",
        "/// <summary>Get player userid.</summary>",
        "CPlayerUserId GetPlayerUserID();",
        "/// <summary>Get steamid.</summary>",
        "uint64 GetSteamID();",
        "/// <summary>Get player steamid.</summary>",
        "CSteamID GetPlayerSteamID();",
        "/// <summary>Get player name.</summary>",
        "const char* GetPlayerName();",
        "/// <summary>Get IP address.</summary>",
        "CUtlString GetIpAddress();",
        "/// <summary>Replicate convar.</summary>",
        "void ReplicateConVar(const char* pszConVar, const char* pszValue);",
        "/// <summary>Fires gameEvent to client's legacy listener. Does not take ownership: the caller still frees pEvent.</summary>",
        "void FireEventToClient(IGameEvent* pEvent);",
    ],
    "CBasePlayerPawn": [
        "/// <summary>Force suicide.</summary>",
        "void CommitSuicide(bool bExplode, bool bForce, HookChain eChain = HookChain::Run);",
        "/// <summary>Snaps the pawn's view angles. No-op where the engine function could not be resolved.</summary>",
        "void SnapViewAngles(const QAngle& angEyeAngles, HookChain eChain = HookChain::Run);",
        "/// <summary>Get controller.</summary>",
        "CCSPlayerController* GetController();",
        "/// <summary>Get default controller.</summary>",
        "CCSPlayerController* GetDefaultController();",
    ],
    "CBasePlayerWeapon": [
        "/// <summary>Get weapon VData.</summary>",
        "CCSWeaponBaseVData* GetWeaponVData();",
        "/// <summary>Get weapon classname.</summary>",
        "const char* GetWeaponClassname();",
    ],
    "CCSGameRules": [
        "/// <summary>Ends the match and moves everyone to the intermission screen.</summary>",
        "void GoToIntermission(bool bAbortedMatch = false, HookChain eChain = HookChain::Run);",
        "/// <summary>Terminate round.</summary>",
        "void TerminateRound(float flDelay, int32_t eRoundEndReason, uint32 nTeamId = 0, HookChain eChain = HookChain::Bypass);",
        "/// <summary>Find entity player is aiming at.</summary>",
        "CBaseEntity* FindPickerEntity(CBasePlayerController* pPlayer);",
        "/// <summary>Get aim target.</summary>",
        "CCSPlayerController* GetClientAimTarget(CCSPlayerController* pPlayer);",
    ],
    "CCSPlayerController": [
        "/// <summary>Get controller from pawn.</summary>",
        "static CCSPlayerController* FromPawn(CCSPlayerPawn* pPawn);",
        "/// <summary>Get controller from slot.</summary>",
        "static CCSPlayerController* FromSlot(int iSlot);",
        "/// <summary>Get controller from slot.</summary>",
        "static CCSPlayerController* FromSlot(CPlayerSlot slot);",
        "/// <summary>Get controller from user id.</summary>",
        "static CCSPlayerController* FromUserId(int iUserId);",
        "/// <summary>Get controller from user id.</summary>",
        "static CCSPlayerController* FromUserId(CPlayerUserId userId);",
        "/// <summary>Get controller from steam id.</summary>",
        "static CCSPlayerController* FromSteamId(uint64 uSteamId);",
        "/// <summary>Get controller from steam id.</summary>",
        "static CCSPlayerController* FromSteamId(CSteamID steamId);",
        "/// <summary>Print to center in HTML.</summary>",
        "void PrintToCenterHtml(const char* pszMessage, int iDuration = 5, bool bMenu = false);",
        "/// <summary>Take damage from player</summary>",
        "void TakeDamage(CCSPlayerController* pAttacker, int iDamage, DamageTypes_t bitsDamageType, HookChain eChain = HookChain::Bypass);",
        "/// <summary>Respawn player.</summary>",
        "void Respawn(HookChain eChain = HookChain::Run);",
        "/// <summary>Switch team without killing.</summary>",
        "void SwitchTeam(int nTeam, HookChain eChain = HookChain::Run);",
        "/// <summary>Change team like jointeam.</summary>",
        "void ChangeTeam(int nTeam, HookChain eChain = HookChain::Run);",
        "/// <summary>Get player pawn.</summary>",
        "CCSPlayerPawn* GetPlayerPawn();",
        "/// <summary>Get observer pawn.</summary>",
        "CCSObserverPawn* GetObserverPawn();",
    ],
    "CCSPlayerPawn": [
        "/// <summary>Get the angles the pawn is actually looking along.</summary>",
        "QAngle GetEyeAngles();",
        "/// <summary>Runs CCSPlayerPawn::PostThink.</summary>",
        "void PostThink(HookChain eChain = HookChain::Run);",
    ],
    "CCSPlayerPawnBase": [
        "/// <summary>Get original controller.</summary>",
        "CCSPlayerController* GetOriginalController();",
        "/// <summary>Whether the pawn may move (false while frozen, defusing, ...).</summary>",
        "bool CanMove();",
    ],
    "CPlayerControllerComponent": [
        "/// <summary>Get player controller.</summary>",
        "CCSPlayerController* GetPlayerController();",
    ],
    "CPlayerPawnComponent": [
        "/// <summary>Get player pawn.</summary>",
        "CCSPlayerPawn* GetPlayerPawn();",
    ],
    "CPlayer_ItemServices": [
        "/// <summary>Drop active weapon, recommended to use DropWeapon instead (parameter is ignored here).</summary>",
        "void DropActivePlayerWeapon(CBasePlayerWeapon* pActiveWeapon, HookChain eChain = HookChain::Run);",
        "/// <summary>Remove all weapons.</summary>",
        "void RemoveWeapons(bool bRemoveSuit, HookChain eChain = HookChain::Run);",
        "/// <summary>Give item.</summary>",
        "CBasePlayerWeapon* GiveNamedItem(const char* pszItem, HookChain eChain = HookChain::Run);",
    ],
    "CPlayer_WeaponServices": [
        "/// <summary>Checks if player can use weapon (fire and maybe acquire).</summary>",
        "bool CanUse(CBasePlayerWeapon *pWeapon);",
        "/// <summary>Drop weapon.</summary>",
        "void DropWeapon(CBasePlayerWeapon *pWeapon, Vector *pVecTarget = nullptr, Vector *pVelocity = nullptr, HookChain eChain = HookChain::Run);",
        "/// <summary>Probably checks weapon validity after CPlayer_ItemServices::GiveNamedItem invoke, may return 2 if CSGameRules()->IsPlayingGunGameDeathmatch, meaning that pWeapon will be deleted.</summary>",
        "int BumpWeapon(CBasePlayerWeapon *pWeapon, HookChain eChain = HookChain::Run);",
        "/// <summary>Select weapon, If unk1 is equal to 3 some code will be executed.</summary>",
        "void SelectItem(CBasePlayerWeapon *pWeapon, int unk1 = 0, HookChain eChain = HookChain::Run);",
        "/// <summary>Remove and destroy weapon from player.</summary>",
        "void Destroy(CBasePlayerWeapon *pWeapon, HookChain eChain = HookChain::Run);",
    ],
}

# Classes whose headers are written by hand, not generated. They stay part of
# the type graph -- referenced, forward-declared and included like any other --
# the generator just neither deletes nor rewrites their files.
#
# This is for engine value types we construct ourselves and hand to engine-owned
# CUtlVectors: a SCHEMA_FIELD wrapper carries no storage, so the generated class
# would be a few bytes wide against the engine's real size, and every stack
# instance, AddToTail() and Element() stride would be wrong.
HANDWRITTEN_CLASSES: frozenset[str] = frozenset({
    "HUDPanelDialogVariableString_t",
    "HUDPanelHasClass_t",
})

# Classes manually whitelisted after BFS
EXTRA_WHITELIST: frozenset[str] = frozenset({
    "CEntitySubclassVDataBase",
    "CFiringModeFloat",
    "CFiringModeInt",
    "CSkillFloat",
    "CSkillInt",
    "CRangeFloat",
    "CNavLinkAnimgraphVar",
    "DecalGroupOption_t",
    "DestructibleHitGroupToDestroy_t",
    "DestructiblePartDamageRequest_t",
    "PrecipitationFilter_t"
})

# ---------------------------------------------------------------------------
# Helper functions
# ---------------------------------------------------------------------------

def sanitise_type_name(name: str) -> str:
    return name.replace(":", "").replace("< ", "<").replace(" >", ">")

def contains_ignored_type(t: SchemaFieldType) -> bool:
    if t.name in IGNORE_CLASSES:
        return True
    if t.inner is not None:
        return contains_ignored_type(t.inner)
    return False

def _field_has_ignored_wildcard(t: SchemaFieldType) -> bool:
    return any(w in t.name for w in IGNORE_CLASS_WILDCARDS)

# ---------------------------------------------------------------------------
# Schema conversion
# ---------------------------------------------------------------------------

def convert_subtype_to_field_type(
        subtype: SchemaSubtype,
        def_lookup: dict[int, SchemaDef],
) -> SchemaFieldType:
    if subtype.type == "ref" and subtype.ref_idx is not None:
        ref_def = def_lookup.get(subtype.ref_idx)
        if ref_def is not None:
            resolved_type = (
                "declared_class" if ref_def.type == "class"
                else ("declared_enum" if ref_def.type == "enum" else ref_def.type)
            )
            return convert_subtype_to_field_type(
                SchemaSubtype(
                    type=resolved_type,
                    name=ref_def.name,
                    size=ref_def.size,
                    alignment=ref_def.alignment,
                    template=None,
                    ref_idx=None,
                    element_size=None,
                    count=None,
                    subtype=None,
                ),
                def_lookup,
            )

    category = {
        "builtin": SchemaTypeCategory.Builtin,
        "atomic": SchemaTypeCategory.Atomic,
        "ptr": SchemaTypeCategory.Ptr,
        "fixed_array": SchemaTypeCategory.FixedArray,
        "declared_class": SchemaTypeCategory.DeclaredClass,
        "declared_enum": SchemaTypeCategory.DeclaredEnum,
        "bitfield": SchemaTypeCategory.Bitfield,
    }.get(subtype.type, SchemaTypeCategory.NoneType)

    atomic: Optional[SchemaAtomicCategory] = None
    if subtype.type == "atomic" and subtype.name:
        name = subtype.name
        if "CUtlVector" in name or "CNetworkUtlVectorBase" in name:
            atomic = SchemaAtomicCategory.Collection
        elif "CHandle" in name or "CWeakHandle" in name or "CStrongHandle" in name:
            atomic = SchemaAtomicCategory.T
        else:
            atomic = SchemaAtomicCategory.Basic

    inner: Optional[SchemaFieldType] = None
    if subtype.template:
        inner = convert_subtype_to_field_type(subtype.template[0], def_lookup)
    elif subtype.subtype is not None:
        inner = convert_subtype_to_field_type(subtype.subtype, def_lookup)

    type_name = subtype.name or "unknown"
    if category == SchemaTypeCategory.FixedArray and subtype.count is not None and inner is not None:
        type_name = f"{inner.name}[{subtype.count}]"

    return SchemaFieldType(type_name, category, atomic, inner)

def convert_new_schema_to_old(
        defs: list[SchemaDef],
) -> tuple[dict[str, SchemaEnum], dict[str, SchemaClass]]:
    enums: dict[str, SchemaEnum] = {}
    classes: dict[str, SchemaClass] = {}
    def_lookup: dict[int, SchemaDef] = {i: d for i, d in enumerate(defs)}

    for def_ in defs:
        if def_.type == "enum" and def_.traits and def_.traits.fields:
            items = [SchemaEnumItem(f.name, f.value) for f in def_.traits.fields]
            enums[def_.name] = SchemaEnum(def_.alignment or 4, items)

    for i, def_ in enumerate(defs):
        if def_.type == "class" and def_.traits is not None:
            parent_name: Optional[str] = None
            if def_.traits.baseclasses:
                parent_idx = def_.traits.baseclasses[0].ref_idx
                parent_def = def_lookup.get(parent_idx)
                if parent_def:
                    parent_name = parent_def.name

            fields: list[SchemaField] = []
            if def_.traits.members:
                for member in def_.traits.members:
                    if member.traits and member.traits.subtype:
                        ft = convert_subtype_to_field_type(member.traits.subtype, def_lookup)
                        metadata = (
                            {t.name: t.value or "" for t in member.traits.metatags}
                            if member.traits.metatags else {}
                        )
                        fields.append(SchemaField(member.name, ft, metadata))

            classes[def_.name] = SchemaClass(i, def_.name, parent_name, fields)

    return enums, classes

# ---------------------------------------------------------------------------
# Graph / BFS
# ---------------------------------------------------------------------------

def build_graph_and_bfs(
        all_classes: dict[str, SchemaClass],
) -> set[str]:
    # adjacency list: class -> set of reachable names
    graph: dict[str, set[str]] = {}

    def add_edge(src: str, dst: str) -> None:
        graph.setdefault(src, set()).add(dst)

    parent_to_children: dict[str, list[str]] = {}
    for name, cls in all_classes.items():
        if cls.parent:
            parent_to_children.setdefault(cls.parent, []).append(name)

    for class_name, cls in all_classes.items():
        if cls.parent:
            add_edge(class_name, cls.parent)

        for f in cls.fields:
            cur = f.type
            while cur is not None:
                if cur.is_declared:
                    add_edge(class_name, cur.name)
                cur = cur.inner

            if f.type.category == SchemaTypeCategory.Ptr and f.type.inner:
                ptr_target = f.type.inner.name
                queue: deque[tuple[str, str]] = deque(
                    (class_name, child)
                    for child in parent_to_children.get(ptr_target, [])
                )
                while queue:
                    parent, child = queue.popleft()
                    add_edge(parent, child)
                    for grandchild in parent_to_children.get(child, []):
                        queue.append((child, grandchild))

    # BFS from each NetworkClass
    visited: set[str] = set()

    def bfs(start: str) -> None:
        if start not in graph:
            return
        q: deque[str] = deque([start])
        while q:
            node = q.popleft()
            if node in visited:
                continue
            visited.add(node)
            for neighbor in graph.get(node, ()):
                if neighbor not in visited:
                    q.append(neighbor)

    for nc in NETWORK_CLASSES:
        bfs(nc)

    return visited

# ---------------------------------------------------------------------------
# Code generation helpers
# ---------------------------------------------------------------------------

def make_header_guard(name: str) -> str:
    safe = sanitise_type_name(name).replace("<", "").replace(">", "").replace(" ", "_")
    return f"_INCLUDE_{safe.upper()}_H"

def header_guard_open(guard: str) -> list[str]:
    """Opening lines of a header: include guard, plus #pragma once for MSVC only."""
    return [
        f"#ifndef {guard}",
        f"#define {guard}",
        "#ifdef _WIN32",
        "#pragma once",
        "#endif",
        "",
    ]

def header_guard_close(guard: str) -> list[str]:
    return [f"#endif // {guard}", ""]

def format_includes(own: set[str], external: set[str], std: set[str]) -> list[str]:
    """
    Include block in the project's order: own/project "..." headers, then
    external <...> (SDK) headers, then <...> standard library headers. Groups
    are separated by a blank line, sorted case-insensitively within, and the
    block ends with a blank line. Entries are bare paths, without quotes or
    angle brackets.
    """
    lines: list[str] = []
    for group, fmt in ((own, '#include "{}"'), (external, "#include <{}>"), (std, "#include <{}>")):
        if not group:
            continue
        lines += [fmt.format(inc) for inc in sorted(group, key=lambda s: (s.lower(), s))]
        lines.append("")
    return lines

def _enum_type_cpp(align: int, items: list[SchemaEnumItem]) -> str:
    is_signed = any(it.value < 0 for it in items)
    table = {
        (1, True): "int8_t",
        (1, False): "uint8_t",
        (2, True): "int16_t",
        (2, False): "uint16_t",
        (4, True): "int32_t",
        (4, False): "uint32_t",
        (8, True): "int64_t",
        (8, False): "uint64_t",
    }
    return table.get((align, is_signed), "int32_t")

def write_enum(enum_name: str, schema_enum: SchemaEnum) -> str:
    underlying = _enum_type_cpp(schema_enum.align, schema_enum.items)
    guard = make_header_guard(enum_name)
    lines = header_guard_open(guard)
    lines += format_includes(set(), set(), {"cstdint"})
    lines += [
        f"enum class {enum_name} : {underlying}",
        "{",
    ]
    for item in schema_enum.items:
        lines.append(f"    {item.name} = {item.value},")
    lines.append("};")
    lines.append("")
    lines += header_guard_close(guard)
    return LICENSE_HEADER + "\n".join(lines)

def collect_referenced_types(
        t: SchemaFieldType,
        includes: set[str],
        forwards: set[str],
        is_pointer_context: bool,
) -> None:
    if t.category == SchemaTypeCategory.DeclaredEnum and t.name in HARD_SKIP_ENUMS:
        header = SKIPPED_ENUM_HEADERS.get(t.name)
        if header:
            # "#" marks a raw header rather than a schema type name.
            includes.add("#" + header)
        return

    is_handle_template = t.atomic == SchemaAtomicCategory.T

    if t.is_declared and t.name not in HARD_SKIP_CLASSES:
        if is_pointer_context or is_handle_template:
            forwards.add(t.name)
        else:
            includes.add(t.name)

    if t.inner is not None:
        inner_is_pointer = t.category == SchemaTypeCategory.Ptr or is_handle_template
        collect_referenced_types(t.inner, includes, forwards, inner_is_pointer)

def collect_types_from_methods(
        methods: list[str],
        all_enums: dict[str, SchemaEnum],
        all_classes: dict[str, SchemaClass],
        includes: set[str],
        forwards: set[str],
) -> None:
    for method in methods:
        for cls_name in all_classes:
            if cls_name in method:
                if cls_name not in includes:
                    forwards.add(cls_name)
        for enum_name in all_enums:
            if enum_name in method:
                # Same treatment as collect_referenced_types: a hard-skipped enum
                # needs the header that really declares it, not "../enums/X.h".
                header = SKIPPED_ENUM_HEADERS.get(enum_name) if enum_name in HARD_SKIP_ENUMS else None
                includes.add("#" + header if header else enum_name)

def inherits_from_base_entity(
        class_name: str,
        all_classes: dict[str, SchemaClass],
) -> bool:
    while True:
        cls = all_classes.get(class_name)
        if cls is None or cls.parent is None:
            return False
        if cls.parent == "CBaseEntity":
            return True
        class_name = cls.parent

def write_class(
        class_name: str,
        schema_class: SchemaClass,
        all_enums: dict[str, SchemaEnum],
        all_classes: dict[str, SchemaClass],
) -> str:
    includes: set[str] = set()
    forwards: set[str] = set()

    if schema_class.parent and schema_class.parent not in HARD_SKIP_CLASSES:
        includes.add(schema_class.parent)

    for f in schema_class.fields:
        if f.type.category == SchemaTypeCategory.Bitfield:
            continue
        if _field_has_ignored_wildcard(f.type):
            continue
        if contains_ignored_type(f.type):
            continue
        collect_referenced_types(
            f.type,
            includes,
            forwards,
            f.type.category == SchemaTypeCategory.Ptr,
            )

    if class_name in MANUAL_METHODS:
        collect_types_from_methods(
            MANUAL_METHODS[class_name], all_enums, all_classes, includes, forwards
        )

    includes.discard(class_name)
    forwards.discard(class_name)
    forwards -= includes

    inherits = class_name == "CBaseEntity" or inherits_from_base_entity(class_name, all_classes)

    guard = make_header_guard(class_name)
    lines = header_guard_open(guard)

    # Own/project headers ("..."): generated siblings and the toolkit's own.
    own_includes: set[str] = {
        "source2toolkit/IToolkitPlugin.h",
        "source2toolkit/schema/entityio.h",
        "source2toolkit/schema/schema.h",
    }
    # External SDK headers (<...>).
    external_includes: set[str] = {
        "ehandle.h",
        "entityhandle.h",
        "igameevents.h",
        "utlbinaryblock.h",
        "utlstring.h",
        "utlstringtoken.h",
        "utlsymbol.h",
        "utlsymbollarge.h",
        "vector.h",
    }
    std_includes: set[str] = {"cstdint"}

    if inherits and class_name != "CBaseEntity":
        own_includes.add("CBaseEntity.h")

    for inc in includes:
        if inc.startswith("#"):
            # Raw SDK header declaring a hard-skipped enum (SKIPPED_ENUM_HEADERS).
            external_includes.add(inc[1:])
        elif inc in all_enums and inc not in HARD_SKIP_ENUMS:
            own_includes.add(f"../enums/{sanitise_type_name(inc)}.h")
        elif inc in all_classes:
            own_includes.add(f"{sanitise_type_name(inc)}.h")

    lines += format_includes(own_includes, external_includes, std_includes)

    for fwd in sorted(set(forwards) | set(MANUAL_FORWARDS.get(class_name, []))):
        lines.append(f"class {fwd};")

    if forwards:
        lines.append("")

    # Class declaration line
    if class_name == "CBaseEntity":
        parent_part = f" : public {schema_class.parent}" if schema_class.parent else ""
        lines.append(f"class {class_name}{parent_part}")
    elif schema_class.parent:
        lines.append(f"class {class_name} : public {schema_class.parent}")
    else:
        if inherits:
            lines.append(f"class {class_name} : public CBaseEntity::Factory<{class_name}>")
        else:
            lines.append(f"class {class_name}")

    # DECLARE_SCHEMA_CLASS makes the field setters reach NetworkStateChanged
    # through CEntityInstance, which is only valid when `this` really is an
    # entity. A non-entity schema class has a vtable of its own (its NetworkVar
    # wrapper's; the NetworkStateChanged slot is found at runtime), and calling CEntityInstance's slot on it
    # reads past the end of that vtable -- a near-null jump on the first
    # networked write, not a wrong value. Non-entity classes without a vtable
    # at all have no networked fields, so neither form ever fires for them.
    is_entity_class = inherits or class_name == "CEntityInstance"
    schema_macro = "DECLARE_SCHEMA_CLASS" if is_entity_class else "DECLARE_SCHEMA_CLASS_INLINE"

    lines += ["{", "public:", f"    {schema_macro}({class_name});", ""]

    for f in schema_class.fields:
        if f.type.category == SchemaTypeCategory.Bitfield:
            continue
        if _field_has_ignored_wildcard(f.type):
            continue
        if contains_ignored_type(f.type):
            continue

        if f.type.category == SchemaTypeCategory.FixedArray or f.type.name == "CUtlStringToken":
            lines.append(f"    SCHEMA_FIELD_POINTER({f.type.cpp_type_name}, {f.name});")
        else:
            lines.append(f"    SCHEMA_FIELD({f.type.cpp_type_name}, {f.name});")

    if class_name in MANUAL_METHODS:
        lines += ["", "public:"]
        for method in MANUAL_METHODS[class_name]:
            lines.append(f"    {method}")

    if inherits and class_name != "CBaseEntity":
        lines += [
            "",
            "public:",
            f"    static {class_name}* New(const char* className)",
            "    {",
            f"        return CBaseEntity::New<{class_name}>(className);",
            "    }",
            "",
            f"    static {class_name}* FromIndex(int iIndex)",
            "    {",
            f"        return CBaseEntity::FromIndex<{class_name}>(iIndex);",
            "    }",
            "",
            f"    static {class_name}* FromIndex(CEntityIndex index)",
            "    {",
            "        return FromIndex(index.Get());",
            "    }",
            "",
            f"    CHandle<{class_name}> GetHandle()",
            "    {",
            f"        return CBaseEntity::GetHandle<{class_name}>();",
            "    }",
        ]

    lines.append("};")
    lines.append("")
    lines += header_guard_close(guard)
    return LICENSE_HEADER + "\n".join(lines)

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> None:
    script_dir = os.path.dirname(os.path.abspath(__file__))
    output_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        script_dir, "../../public/source2toolkit/schema/entity"
    )

    schema_path = os.path.join(script_dir, "server.json")
    with open(schema_path, encoding="utf-8") as fh:
        raw = json.load(fh)

    defs = _parse_schema_defs(raw["defs"])
    enums, classes = convert_new_schema_to_old(defs)

    all_enums: dict[str, SchemaEnum] = {
        k: v for k, v in sorted(enums.items()) if k not in IGNORE_CLASSES
    }
    all_classes: dict[str, SchemaClass] = {
        k: v for k, v in sorted(classes.items()) if k not in IGNORE_CLASSES
    }

    visited = build_graph_and_bfs(all_classes)
    visited |= EXTRA_WHITELIST

    # Clear existing .h files, leaving the hand-written ones in place
    handwritten_files = {f"{sanitise_type_name(n)}.h" for n in HANDWRITTEN_CLASSES}
    if os.path.isdir(output_path):
        for root, _, files in os.walk(output_path):
            for fname in files:
                if fname.endswith(".h") and fname not in handwritten_files:
                    os.remove(os.path.join(root, fname))

    enums_dir = os.path.join(output_path, "enums")
    classes_dir = os.path.join(output_path, "classes")
    os.makedirs(enums_dir, exist_ok=True)
    os.makedirs(classes_dir, exist_ok=True)

    for enum_name, schema_enum in all_enums.items():
        if enum_name in HARD_SKIP_ENUMS:
            continue
        content = write_enum(enum_name, schema_enum)
        out_file = os.path.join(enums_dir, f"{sanitise_type_name(enum_name)}.h")
        with open(out_file, "w", encoding="utf-8", newline="") as fh:
            fh.write(content)

    for class_name, schema_class in all_classes.items():
        if class_name in HARD_SKIP_CLASSES:
            continue
        if class_name in HANDWRITTEN_CLASSES:
            continue
        if class_name not in visited and "VData" not in class_name:
            continue
        content = write_class(class_name, schema_class, all_enums, all_classes)
        out_file = os.path.join(classes_dir, f"{sanitise_type_name(class_name)}.h")
        with open(out_file, "w", encoding="utf-8", newline="") as fh:
            fh.write(content)

    print(f"Done. Classes: {len(all_classes)}, Enums: {len(all_enums)}, Classes written to {output_path}")

if __name__ == "__main__":
    main()