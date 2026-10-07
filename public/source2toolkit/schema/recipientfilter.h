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
#pragma once
#include <cstdint>
#include "irecipientfilter.h"
#include "playerslot.h"

// The game's own CRecipientFilter, member for member (libserver.so: ctor,
// CopyFrom, Reset and the four IRecipientFilter getters read these offsets).
// s2sdk's game/server/recipientfilter.h is still the Source 1 class and does
// not compile, so the toolkit carries its own. Only what works from the
// filter's own state is here; the game's helpers that walk the player list
// (AddAllPlayers, AddRecipientsByTeam/PVS/PAS, UsePredictionRules) are not.
//
// Unlike the game, the default buffer is BUF_RELIABLE (the game's is
// BUF_DEFAULT): every caller so far relies on it.
class CRecipientFilter : public IRecipientFilter
{
public:
    CRecipientFilter(NetChannelBufType_t nBufType = BUF_RELIABLE, bool bInitMessage = false)
        : m_nPredictedPlayerSlot(-1), m_nBufType(nBufType), m_bInitMessage(bInitMessage),
          m_bUsingPredictionRules(false), m_bIgnorePredictionCull(false)
    {
        m_Recipients.ClearAll();
    }

    ~CRecipientFilter() override
    {
    }

    NetChannelBufType_t GetNetworkBufType(void) const override { return m_nBufType; }
    bool IsInitMessage(void) const override { return m_bInitMessage; }
    const CPlayerBitVec& GetRecipients(void) const override { return m_Recipients; }
    CPlayerSlot GetPredictedPlayerSlot(void) const override { return m_nPredictedPlayerSlot; }

    // Like the game: the recipients and the flags, not the predicted slot.
    void CopyFrom(const CRecipientFilter& src)
    {
        m_Recipients = src.m_Recipients;
        m_nBufType = src.GetNetworkBufType();
        m_bInitMessage = src.IsInitMessage();
        m_bUsingPredictionRules = src.m_bUsingPredictionRules;
        m_bIgnorePredictionCull = src.m_bIgnorePredictionCull;
    }

    void Reset()
    {
        m_Recipients.ClearAll();
        m_nBufType = BUF_DEFAULT;
        m_bInitMessage = false;
        m_bUsingPredictionRules = false;
        m_bIgnorePredictionCull = false;
    }

    void MakeInitMessage() { m_bInitMessage = true; }
    void MakeReliable() { m_nBufType = BUF_RELIABLE; }

    void AddRecipient(CPlayerSlot slot)
    {
        if (slot.IsValid())
            m_Recipients.Set(slot.Get());
    }

    void RemoveRecipient(CPlayerSlot slot)
    {
        if (slot.IsValid())
            m_Recipients.Clear(slot.Get());
    }

    void RemoveAllRecipients() { m_Recipients.ClearAll(); }

    bool HasRecipient(CPlayerSlot slot) const
    {
        return slot.IsValid() && m_Recipients.IsBitSet(slot.Get());
    }

    int GetRecipientCount() const
    {
        int count = 0;
        for (int i = m_Recipients.FindNextSetBit(0); i != -1; i = m_Recipients.FindNextSetBit(i + 1))
            count++;
        return count;
    }

    // CBitVec's Or/And/Not do not compile under clang (unqualified
    // ValidateOperand in a dependent base), so bit by bit.
    void AddPlayersFromBitMask(const CPlayerBitVec& playerbits)
    {
        for (int i = playerbits.FindNextSetBit(0); i != -1; i = playerbits.FindNextSetBit(i + 1))
            m_Recipients.Set(i);
    }

    void RemovePlayersFromBitMask(const CPlayerBitVec& playerbits)
    {
        for (int i = playerbits.FindNextSetBit(0); i != -1; i = playerbits.FindNextSetBit(i + 1))
            m_Recipients.Clear(i);
    }

    void SetFromBitmask(uint64_t mask)
    {
        // CPlayerBitVec is CBitVec<64> backed by 2x uint32; reinterpret as a single uint64
        *reinterpret_cast<uint64_t*>(m_Recipients.Base()) = mask;
    }

    // The player whose client already predicted the event: dropped from the
    // recipients and reported through GetPredictedPlayerSlot(), as the game does.
    void SetPredictedPlayer(CPlayerSlot slot)
    {
        if (!HasRecipient(slot))
            return;
        RemoveRecipient(slot);
        m_nPredictedPlayerSlot = slot;
    }

    bool IsUsingPredictionRules() const { return m_bUsingPredictionRules; }

    bool IgnorePredictionCull() const { return m_bIgnorePredictionCull; }
    void SetIgnorePredictionCull(bool ignore) { m_bIgnorePredictionCull = ignore; }

protected:
    CPlayerBitVec m_Recipients;         // +8
    CPlayerSlot m_nPredictedPlayerSlot; // +16
    NetChannelBufType_t m_nBufType;     // +20, int8
    bool m_bInitMessage;                // +21
    bool m_bUsingPredictionRules;       // +22
    bool m_bIgnorePredictionCull;       // +23
};

static_assert(sizeof(CRecipientFilter) == 24, "CRecipientFilter has to match the game's layout");

class CSingleRecipientFilter : public CRecipientFilter
{
public:
    CSingleRecipientFilter(CPlayerSlot nRecipientSlot, NetChannelBufType_t nBufType = BUF_RELIABLE,
                           bool bInitMessage = false)
        : CRecipientFilter(nBufType, bInitMessage)
    {
        AddRecipient(nRecipientSlot);
    }
};
