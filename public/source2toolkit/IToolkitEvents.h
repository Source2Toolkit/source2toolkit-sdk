/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
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
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
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
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

/**

* @file IToolkitEvents.h
* @brief Interface for registering and handling game events.
*
* Provides functionality for hooking into Source2 game events,
* allowing plugins to inspect, modify, or block events.
*
* @note Events correspond to engine-defined events (e.g. "player_death").
  */

#ifndef _INCLUDE_ITOOLKIT_EVENTS_H
#define _INCLUDE_ITOOLKIT_EVENTS_H

#pragma once
#include "IToolkitPlugin.h"
#include "IToolkitTypes.h"

#include <engine2/isource2server.h>
#include <engine2/ivengineserver2.h>
#include <igameevents.h>

/* =========================
Forward declarations
========================= */

/**

* @brief Callback type for game events.
*
* @param event Pointer to the event data
* @param post false when called before the engine processes the event, true after
* @param dontBroadcast Set to true to prevent event from being sent to clients
*
* @return Action:
* * Action::Ignore: no changes
* * Action::Override: modify event but still allow original execution (pre only)
* * Action::Supersede: block original execution (pre only)
    */
using GameEventHandler = ToolkitCallback<Action(IGameEvent* event, bool post, bool& dontBroadcast)>;

/* =========================
Core Toolkit Events
========================= */

/**

* @brief Interface for registering game event listeners.
*
* Allows plugins to:
* * Listen to engine events
* * Modify event data
* * Block event propagation
    */
#define TOOLKIT_EVENTS_INTERFACE "IToolkitEvents002"

class IToolkitEvents
{
public:
    virtual ~IToolkitEvents() = default;

    /**
     * @brief Registers a listener for a game event. The same event may be
     * hooked as often as a plugin likes; each handler runs.
     *
     * Whose it is, the core reads off the handler (see ToolkitCallback); what
     * a plugin still holds at unload the core drops for it.
     *
     * @param pchName Event name (e.g. "player_death")
     * @param handler A function, an object and a method, or a lambda
     * @param post false to be called before the engine processes the event,
     *             true to be called after
     * @return The id UnhookGameEvent(id) takes
     */
    virtual ToolkitHookId HookGameEvent(const char* pchName, GameEventHandler handler, bool post) = 0;

    /**
     * @brief Drops the hook with this handler -- a function or an object and
     * a method; a lambda goes by its id.
     *
     * @return true when one was found
     */
    virtual bool UnhookGameEvent(const char* pchName, const GameEventHandler& handler, bool post) = 0;

    /**
     * @brief Drops the hook HookGameEvent() returned the id for.
     *
     * @return true when it was still there
     */
    virtual bool UnhookGameEvent(ToolkitHookId id) = 0;
};

/**
 * @brief Macro for hooking a game event.
 */
#define HOOK_GAME_EVENT(passname, passfunc, post) \
    g_pToolkitEvents->HookGameEvent(passname, passfunc, post)

/**
 * @brief Macro for dropping a game event hook (a function or an object and a
 * method; a lambda goes by the id HOOK_GAME_EVENT returned).
 */
#define UNHOOK_GAME_EVENT(passname, passfunc, post) \
    g_pToolkitEvents->UnhookGameEvent(passname, passfunc, post)

#endif //_INCLUDE_ITOOLKIT_EVENTS_H
