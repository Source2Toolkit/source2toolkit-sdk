//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: AttackerInfo_t
//
//=============================================================================

#ifndef ATTACKERINFO_H
#define ATTACKERINFO_H
#ifdef _WIN32
#pragma once
#endif

#include <ehandle.h>
#include <tier0/mempool.h>

class CCSPlayerPawn;

struct AttackerInfo_t
{
    bool m_bNeedInit;
    bool m_bIsPawn;
    bool m_bIsWorld;
    CHandle<CCSPlayerPawn> m_hAttackerPawn;
    int32_t m_nAttackerPlayerSlot;
    int m_iTeamChecked;
    int m_nTeam;
};

#endif // ATTACKERINFO_H
