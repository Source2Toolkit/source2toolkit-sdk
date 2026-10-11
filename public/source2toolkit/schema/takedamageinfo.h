//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: CTakeDamageInfo
//
//=============================================================================

#ifndef TAKEDAMAGEINFO_H
#define TAKEDAMAGEINFO_H
#ifdef _WIN32
#pragma once
#endif

#include "attackerinfo.h"

// DamageTypes_t comes from the game headers; the SDK no longer emits a scoped
// copy of it (see HARD_SKIP_ENUMS in tools/schemagen/generator.py).
#include "shareddefs.h"

#include "ehandle.h"
#include "gametrace.h"
#include "tier0/mempool.h"

class CCSPlayerPawn;

class CTakeDamageInfo
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x8];

public:
	CTakeDamageInfo();
	CTakeDamageInfo(CBaseEntity* pInflictor, CBaseEntity* pAttacker, CBaseEntity* pAbility, float flDamage, DamageTypes_t bitsDamageType);

    HitGroup_t GetHitGroup() const;

public:

	Vector m_vecDamageForce;	  // 0x8  |  8
	Vector m_vecDamagePosition;	  // 0x14 | 20
	Vector m_vecReportedPosition; // 0x20 | 32
	Vector m_vecDamageDirection;  // 0x2c | 44
	CHandle<CBaseEntity> m_hInflictor;	  // 0x38 | 56
	CHandle<CBaseEntity> m_hAttacker;	  // 0x3c | 60
	CHandle<CBaseEntity> m_hAbility;		  // 0x40 | 64
	float m_flDamage;			  // 0x44 | 68
	float m_flTotalledDamage;	  // 0x48 | 72
	int32_t m_bitsDamageType;	  // 0x4c | 76
	int32_t m_iDamageCustom;	  // 0x50 | 80
	int8_t m_iAmmoType;			  // 0x54 | 84

private:
	[[maybe_unused]] uint8_t m_nUnknown0[0xb]; // 0x55 | 85

public:
	float m_flOriginalDamage; // 0x60 | 96
	bool m_bShouldBleed;	  // 0x64 | 100
	bool m_bShouldSpark;	  // 0x65 | 101

private:
	[[maybe_unused]] uint8_t m_nUnknown1[0x2]; // 0x66

public:
	CGameTrace* m_pTrace;						// 0x68 | 104
	TakeDamageFlags_t m_nDamageFlags;			// 0x70 | 112
	HitGroup_t m_iHitGroupId;					// 0x78 | 120
	int32_t m_nNumObjectsPenetrated;			// 0x7c | 124
	float m_flFriendlyFireDamageReductionRatio; // 0x80 | 128
	bool m_bStoppedBullet;						// 0x84 | 132

private:
	uint8_t m_nUnknown2[0x58]; // 0x88 | 136

public:
	void* m_hScriptInstance;							// 0xe0 | 224
	AttackerInfo_t m_AttackerInfo;						// 0xe8 | 232
	CUtlLeanVector<int> m_DestructibleHitGroupRequests; // 0x100 | 256   CUtlLeanVector<DestructiblePartDamageRequest_t>
	bool m_bInTakeDamageFlow;							// 0x110 | 272

private:
	[[maybe_unused]] int32_t m_nUnknown4; // 0x11c | 284
};

#endif // TAKEDAMAGEINFO_H
