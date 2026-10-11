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

/**
 * @file IToolkitTransmit.h
 * @brief Interface for deciding which entities each player receives.
 *
 * Every tick the server works out, per player, the set of entities to send
 * (CheckTransmit). This interface sits after that decision and takes entities
 * back out of it:
 *
 * * an entity is hooked once, and from then on has a visibility per viewer --
 *   hidden from some players, shown to others, with up to eight independent
 *   "channels" so two features (a !hide command, a spectator mode, a
 *   team-only marker) can each hide the same entity without stepping on
 *   each other's toes: the entity shows only when no channel hides it;
 * * a player is hidden through their controller: hiding a controller from a
 *   viewer hides the pawn that controller is driving, the observer pawn and a
 *   dead player pawn, while the controller itself stays -- the scoreboard
 *   keeps working;
 * * an entity can be tied to an owner entity, and then disappears wherever
 *   its owner is hidden (a player's weapons, a beam attached to them);
 * * BlockAll takes an entity away from everybody, the one-line "server-side
 *   only" switch;
 * * and for anything that does not fit the model, HookCheckTransmit() hands
 *   over each viewer's transmit set, wrapped, so a plugin never has to lay
 *   out CCheckTransmitInfo itself.
 *
 * Hiding is done the way the client expects: the entity leaves the transmit
 * set and is put on the "non-transmitted" list at the same time, so the
 * client keeps a record of it while it is hidden and takes it back without
 * the "missing client entity" crash when it comes back. Both of those bits
 * are what IToolkitTransmitInfo::BlockTransmit() does.
 *
 * @note Everything here belongs on the main thread.
 */

#ifndef _INCLUDE_ITOOLKIT_TRANSMIT_H
#define _INCLUDE_ITOOLKIT_TRANSMIT_H

#pragma once
#include "IToolkitPlugin.h"
#include "IToolkitTypes.h"

#include <bitvec.h>
#include <const.h>
#include <playerslot.h>

#include <cstdint>
#include <functional>

/* =========================
Forward declarations
========================= */

class CEntityInstance;

/* =========================
Types
========================= */

/**
 * @brief Visibility channels.
 *
 * A channel is a number a caller picks for its own reason to hide something.
 * An entity shows to a viewer only when no channel hides it there, so two
 * independent features never un-hide each other's work. Most callers only
 * ever need channel 0.
 */
inline constexpr int TRANSMIT_CHANNEL_ALL = -1;
inline constexpr int TRANSMIT_MAX_CHANNELS = 8;

/* =========================
Transmit info
========================= */

/**
 * @brief One viewer's slice of a CheckTransmit call: who it is for, and
 *        which entities are on their way to them.
 *
 * Handed to a CheckTransmitHook, one per viewer, valid for the duration of
 * the callback only.
 */
class IToolkitTransmitInfo
{
public:
    virtual ~IToolkitTransmitInfo() = default;

    /**
     * @brief The player this transmit set belongs to.
     */
    virtual CPlayerSlot GetPlayerSlot() const = 0;

    /**
     * @brief True when the player is being sent everything from scratch
     *        (first snapshot, reconnect), false for an ordinary delta.
     */
    virtual bool IsFullUpdate() const = 0;

    /**
     * @brief Whether the entity is currently in the set.
     */
    virtual bool IsTransmitting(int entityIndex) const = 0;

    /**
     * @brief Takes an entity out of the set, and lets the client keep track
     *        of it while it is gone, so it can come back later.
     *
     * This is the right way to hide an entity. Clearing the transmit bit alone
     * makes the client forget the entity, and retransmitting it afterwards
     * crashes the client on "missing client entity".
     */
    virtual void BlockTransmit(int entityIndex) = 0;
    virtual void BlockTransmit(CEntityInstance* entity) = 0;

    /**
     * @brief Puts an entity (back) into the set.
     */
    virtual void AllowTransmit(int entityIndex) = 0;
    virtual void AllowTransmit(CEntityInstance* entity) = 0;

    /* =========================
    Raw access
    ========================= */

    /**
     * @brief The four bit sets of CCheckTransmitInfo, for whatever the
     *        methods above do not cover. Entity index = bit index.
     *
     * TransmitEntity     -- entities sent this tick.
     * TransmitNonPlayers -- entities the client keeps a record of without
     *                       receiving them (what BlockTransmit() sets).
     * TransmitOutOfPVS   -- entities that left the PVS but still get a delta.
     * TransmitAlways     -- HLTV/replay only, may be nullptr.
     */
    virtual CBitVec<MAX_EDICTS>* GetTransmitEntity() = 0;
    virtual CBitVec<MAX_EDICTS>* GetTransmitNonPlayers() = 0;
    virtual CBitVec<MAX_EDICTS>* GetTransmitOutOfPVS() = 0;
    virtual CBitVec<MAX_EDICTS>* GetTransmitAlways() = 0;
};

/* =========================
Callback types
========================= */

/**
 * @brief Hook called once per CheckTransmit, after the toolkit has applied
 *        the hooked entities' visibility.
 *
 * @param infos         One transmit set per viewer this tick
 * @param infoCount     Number of entries in @p infos
 * @param entityIndices Indices of the entities the engine considered this
 *                      tick -- the candidates; anything not listed here is
 *                      not being sent to anybody
 * @param entityCount   Number of entries in @p entityIndices
 *
 * Runs every tick for every viewer, so do the expensive part (who owns what,
 * who is on which team) once at the top and only the bit work per viewer.
 */
using CheckTransmitHook = ToolkitCallback<void(IToolkitTransmitInfo* const* infos, int infoCount, const uint16_t* entityIndices, int entityCount)>;

/* =========================
Core Toolkit Transmit
========================= */

#define TOOLKIT_TRANSMIT_INTERFACE "IToolkitTransmit002"

class IToolkitTransmit
{
public:
    virtual ~IToolkitTransmit() = default;

    /* =========================
    Hooking entities
    ========================= */

    /**
     * @brief Puts an entity under this interface's control.
     *
     * Until an entity is hooked, the setters below refuse it and it is sent
     * as the engine decides. The hook goes away with the entity, with the
     * plugin, or with UnhookEntity().
     *
     * Hook a player's controller, not their pawn: a controller's visibility
     * is applied to whatever pawn it is driving. Pawns are refused.
     *
     * @param owner          Plugin ID that owns the hook
     * @param entity         Entity to hook
     * @param defaultVisible Whether viewers see it until told otherwise. false
     *                       is for "personal" entities: hidden from everybody,
     *                       then SetVisible(entity, ownerSlot, true).
     *
     * @return false when the entity is a pawn, already hooked, or about to be
     *         deleted.
     */
    virtual bool HookEntity(PluginId owner, CEntityInstance* entity, bool defaultVisible = true) = 0;

    /**
     * @brief Forgets an entity: sent as the engine decides from now on.
     */
    virtual void UnhookEntity(CEntityInstance* entity) = 0;

    virtual bool IsEntityHooked(CEntityInstance* entity) = 0;

    /* =========================
    Visibility
    ========================= */

    /**
     * @brief Shows or hides a hooked entity for one viewer.
     *
     * @param entity  Hooked entity
     * @param viewer  The player it is shown to / hidden from
     * @param visible false hides it on @p channel; true withdraws that
     *                channel's objection, and for an entity hidden by
     *                default also marks the viewer as allowed
     * @param channel Channel 0..TRANSMIT_MAX_CHANNELS-1, or
     *                TRANSMIT_CHANNEL_ALL for every channel at once
     *
     * @return false when the entity is not hooked.
     */
    virtual bool SetVisible(CEntityInstance* entity, CPlayerSlot viewer, bool visible, int channel = 0) = 0;

    /**
     * @brief SetVisible() for every player slot at once.
     */
    virtual bool SetVisibleToAll(CEntityInstance* entity, bool visible, int channel = 0) = 0;

    /**
     * @brief Whether a viewer would receive the entity, the channels, the
     *        default and BlockAll all taken into account. An unhooked entity
     *        is visible. The owner is not consulted -- ask about it directly.
     */
    virtual bool IsVisible(CEntityInstance* entity, CPlayerSlot viewer) = 0;

    /**
     * @brief Whether one channel hides the entity from the viewer.
     */
    virtual bool IsHiddenOnChannel(CEntityInstance* entity, CPlayerSlot viewer, int channel) = 0;

    /**
     * @brief Hides a hooked entity from everybody, whatever the channels say.
     */
    virtual bool SetBlockAll(CEntityInstance* entity, bool block) = 0;
    virtual bool GetBlockAll(CEntityInstance* entity) = 0;

    /**
     * @brief Ties a hooked entity to an owner: wherever the owner is hidden,
     *        so is the entity. The owner has to be hooked for that to apply.
     *
     * @param owner Owner entity, nullptr to untie
     */
    virtual bool SetOwner(CEntityInstance* entity, CEntityInstance* owner) = 0;
    virtual CEntityInstance* GetOwner(CEntityInstance* entity) = 0;

    /**
     * @brief Puts every hooked entity back to its default for one viewer.
     *
     * Done by the toolkit when the player disconnects.
     */
    virtual void ResetViewer(CPlayerSlot viewer) = 0;

    /* =========================
    Hooks
    ========================= */

    /**
     * @brief Hooks CheckTransmit itself.
     *
     * Handlers run after the toolkit's own pass, in the order they were
     * added; a plugin may add as many as it likes.
     *
     * Whose it is, the core reads off the handler (see ToolkitCallback); what
     * a plugin still holds at unload the core drops for it.
     *
     * @param handler A function, an object and a method, or a lambda
     * @return The id UnhookCheckTransmit(id) takes
     */
    virtual ToolkitHookId HookCheckTransmit(CheckTransmitHook handler) = 0;

    /**
     * @brief Drops the hook with this handler -- a function or an object and
     * a method; a lambda goes by its id.
     *
     * @return true when one was found
     */
    virtual bool UnhookCheckTransmit(const CheckTransmitHook& handler) = 0;

    /**
     * @brief Drops the hook HookCheckTransmit() returned the id for.
     *
     * @return true when it was still there
     */
    virtual bool UnhookCheckTransmit(ToolkitHookId id) = 0;
};

/* =========================
Helpers
========================= */

/**
 * @brief Shorthand accessors via g_pToolkitTransmit.
 */
#define TRANSMIT_HOOK_ENTITY(entity, ...)   g_pToolkitTransmit->HookEntity(g_PluginID, entity, ##__VA_ARGS__)
#define TRANSMIT_UNHOOK_ENTITY(entity)      g_pToolkitTransmit->UnhookEntity(entity)
#define TRANSMIT_SET_VISIBLE(entity, ...)   g_pToolkitTransmit->SetVisible(entity, __VA_ARGS__)
#define TRANSMIT_SET_BLOCK_ALL(entity, on)  g_pToolkitTransmit->SetBlockAll(entity, on)

#define HOOK_CHECK_TRANSMIT(handler)   g_pToolkitTransmit->HookCheckTransmit(handler)
#define UNHOOK_CHECK_TRANSMIT(handler) g_pToolkitTransmit->UnhookCheckTransmit(handler)

#endif //_INCLUDE_ITOOLKIT_TRANSMIT_H
