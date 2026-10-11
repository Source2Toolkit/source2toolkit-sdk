//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: Movement data passed through CCSPlayer_MovementServices.
//
//=============================================================================

#ifndef MOVEDATA_H
#define MOVEDATA_H
#ifdef _WIN32
#pragma once
#endif

#include <ehandle.h>
#include <gametrace.h>
#include <mathlib/vector.h>
#include <mathlib/vector2d.h>
#include <tier0/utlvector.h>

class CCSPlayerPawn;

// The layout below is ported from cs2kz-metamod's src/sdk/datatypes.h
// (lines 135-278), which is also where SwiftlyS2 takes its CMoveData from.
// Nothing here is schema-backed: the game does not expose these through the
// schema system, so the offsets are fixed by the declaration order and any
// change to it silently breaks every movement hook.

// One subtick input sample. Either a button press or an analog move, hence the
// union; IsAnalogInput() says which half is live.
struct SubtickMove
{
	float m_flWhen; // 0x00

private:
	[[maybe_unused]] int32_t __pad0004; // 0x04

public:
	uint64_t m_nButton; // 0x08

	union
	{
		bool m_bPressed; // 0x10

		struct
		{
			float m_flAnalogForwardDelta; // 0x10
			float m_flAnalogLeftDelta;	  // 0x14
			float m_flPitchDelta;		  // 0x18
			float m_flYawDelta;			  // 0x1c
		} m_AnalogMove;
	};

	bool IsAnalogInput() const { return m_nButton == 0; }
};

struct TouchList_t
{
	Vector m_vecDeltaVelocity;
	CGameTrace m_Trace;
};

class CMoveDataBase
{
public:
	// Bitfield: bit 0 is m_bHasZeroFrametime, bit 1 is m_bIsLateCommand.
	uint8_t m_nBitfield; // 0x00

	bool HasZeroFrametime() const { return (m_nBitfield & (1 << 0)) != 0; }
	bool IsLateCommand() const { return (m_nBitfield & (1 << 1)) != 0; }

	void SetHasZeroFrametime(bool bValue) { m_nBitfield = bValue ? (m_nBitfield | (1 << 0)) : (m_nBitfield & ~(1 << 0)); }
	void SetIsLateCommand(bool bValue) { m_nBitfield = bValue ? (m_nBitfield | (1 << 1)) : (m_nBitfield & ~(1 << 1)); }

public:
	CHandle<CCSPlayerPawn> m_nPlayerHandle;
	QAngle m_vecAbsViewAngles;
	QAngle m_vecViewAngles;
	Vector m_vecLastMovementImpulses;
	float m_flForwardMove;
	// Flipped compared to CS:GO: moving right gives a negative value.
	float m_flSideMove;
	float m_flUpMove;
	Vector m_vecVelocity;
	QAngle m_vecAngles;
	// Unused, likely pulled in from an engine upstream.
	Vector m_vecUnknown;
	CUtlVector<SubtickMove> m_SubtickMoves;
	CUtlVector<SubtickMove> m_AttackSubtickMoves;
	bool m_bHasSubtickInputs;
	// Set to 1.0 during SetupMove and never changed during gameplay; the
	// weapon services appear to read it.
	float m_flUnknown;
	CUtlVector<TouchList_t> m_TouchList;
	Vector m_vecCollisionNormal;
	Vector m_vecGroundNormal;
	Vector m_vecAbsOrigin;
	int32_t m_nTickCount;
	int32_t m_nTargetTick;
	float m_flSubtickStartFraction;
	float m_flSubtickEndFraction;
};

class CMoveData : public CMoveDataBase
{
public:
	Vector m_vecOutWishVel;
	QAngle m_vecOldAngles;
	Vector2D m_vecWalkWishedVelocity;
	Vector m_vecAcceleration;
	Vector m_vecContinuousAcceleration;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	float m_flFrictionDecel;

private:
	[[maybe_unused]] uint8_t __pad_frictiondecel[3 * 4];

public:
	bool m_bInAir;
	// True when the usercmd number equals m_nGameCodeHasMovedPlayerAfterCommand + 1.
	bool m_bGameCodeMovedPlayer;
};

#endif // MOVEDATA_H
