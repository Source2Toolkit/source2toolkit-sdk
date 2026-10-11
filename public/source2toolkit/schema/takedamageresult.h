//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: CTakeDamageResult
//
//=============================================================================

#ifndef TAKEDAMAGERESULT_H
#define TAKEDAMAGERESULT_H
#ifdef _WIN32
#pragma once
#endif

#include <ehandle.h>
#include <gametrace.h>
#include <shareddefs.h>
#include <tier0/mempool.h>

class CTakeDamageInfo;

class CTakeDamageResult
{
public:
	CTakeDamageInfo* m_pOriginatingInfo;
	CUtlLeanVector<int> m_DestructibleHitGroupRequests; // CUtlLeanVector<DestructiblePartDamageRequest_t>
	int32_t m_nHealthLost;
	int32_t m_nHealthBefore;
	float m_flDamageDealt;
	float m_flPreModifiedDamage;
	int32_t m_nTotalledHealthLost;
	float m_flTotalledDamageDealt;
	float m_flTotalledPreModifiedDamage;
	float m_flNewDamageAccumulatorValue;
	TakeDamageFlags_t m_nDamageFlags;
	bool m_bWasDamageSuppressed;
	bool m_bSuppressFlinch;
	HitGroup_t m_nOverrideFlinchHitGroup;

private:
	[[maybe_unused]] uint8_t m_nUnknown0[0x8];

public:
	CTakeDamageResult() = delete;

	CTakeDamageResult(float damage) :
		m_pOriginatingInfo(nullptr),
		m_nHealthLost(static_cast<int32_t>(damage)),
		m_nHealthBefore(0),
		m_flDamageDealt(damage),
		m_flPreModifiedDamage(damage),
		m_nTotalledHealthLost(static_cast<int32_t>(damage)),
		m_flTotalledDamageDealt(damage),
		m_bWasDamageSuppressed(false)
	{
	}

	void CopyFrom(CTakeDamageInfo* pInfo);
};

#endif // TAKEDAMAGERESULT_H
