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
 * @file IToolkitScripts.h
 * @brief Interface for running cs_script (the game's server-side JavaScript).
 *
 * CS2 runs map and mode logic in JavaScript through `point_script` entities
 * (`import { Instance } from "cs_script/point_script"`). This interface lets a
 * plugin run scripts of its own and talk to them:
 *
 * * run a compiled script asset (`.vjs`) the way a map does -- this needs no
 *   signature and keeps working through game updates;
 * * run raw JavaScript source, or a `.js` file straight from the server's
 *   disk, and run it again after an edit (hot reload);
 * * fire an input a script listens for with `Instance.OnScriptInput(...)`
 *   (native -> script);
 * * receive what a script sends with `Instance.ServerCommand(...)`
 *   (script -> native);
 * * keep a script alive across round restarts and map changes.
 *
 * A script sends a message with
 * @code
 * Instance.ServerCommand("toolkit_script mychannel " + encodeURIComponent(JSON.stringify(data)));
 * @endcode
 * and every handler registered for `mychannel` receives everything after the
 * channel name. The line goes through the server's command buffer: it arrives
 * on a later tick, a `;` or a newline in it would split it into two commands,
 * and it is cut at the buffer's line limit -- encode the payload (as above)
 * and keep it short. Only the server itself can run `toolkit_script`; a
 * client typing it is ignored.
 *
 * @note Everything here belongs on the main thread.
 */

#ifndef _INCLUDE_ITOOLKIT_SCRIPTS_H
#define _INCLUDE_ITOOLKIT_SCRIPTS_H

#pragma once
#include "IToolkitPlugin.h"

#include <entityhandle.h>

#include <functional>

/* =========================
Types
========================= */

/**
 * @brief Called with a message a script sent through `toolkit_script`.
 *
 * @param pszChannel The channel the message was sent on
 * @param pszPayload Everything after the channel name, exactly as sent
 *                   ("" when there was nothing)
 */
using ScriptMessageHandler = ToolkitCallback<void(const char* pszChannel, const char* pszPayload)>;

/* =========================
Core Toolkit Scripts
========================= */

#define TOOLKIT_SCRIPTS_INTERFACE "IToolkitScripts002"

/**
 * @brief Runs cs_script scripts for plugins and carries messages both ways.
 *
 * A script is known by a name the plugin picks, which is also the targetname
 * of its `point_script` entity; running a script under a name already in use
 * replaces the old one. Every script and every message handler belongs to
 * the plugin that created it and goes away when that plugin unloads.
 */
class IToolkitScripts
{
public:
    virtual ~IToolkitScripts() = default;

    /**
     * @brief Whether raw source can be run (RunSource, RunFile).
     *
     * Needs the game's script loader (gamedata "CSScript::RunScript") and the
     * script component inside `point_script`. LoadAsset() works without both.
     */
    virtual bool CanRunSource() = 0;

    /**
     * @brief Runs a compiled script asset, exactly as a map's `point_script` does.
     *
     * @param owner       Owning plugin
     * @param pszName     Name to refer to the script by
     * @param pszAsset    Resource path of the compiled script, e.g. "scripts/myplugin/main.vjs"
     * @param bPersistent Spawn it again after a round restart or a map change
     * @return The `point_script` entity, or an invalid handle on failure
     */
    virtual CEntityHandle LoadAsset(PluginId owner, const char* pszName, const char* pszAsset, bool bPersistent) = 0;

    /**
     * @brief Runs raw JavaScript source in a new `point_script`.
     *
     * @param owner          Owning plugin
     * @param pszName        Name to refer to the script by
     * @param pszVirtualPath Path the script goes by in errors and stack traces,
     *                       e.g. "scripts/myplugin.js"
     * @param pszSource      The source, an ES module importing "cs_script/point_script"
     * @param bPersistent    Run it again after a round restart or a map change
     * @return The `point_script` entity, or an invalid handle when the source cannot be run
     *
     * @note A script that does not compile is reported by the game in the
     *       server console; the entity exists regardless.
     */
    virtual CEntityHandle RunSource(PluginId owner, const char* pszName, const char* pszVirtualPath, const char* pszSource, bool bPersistent) = 0;

    /**
     * @brief Runs a JavaScript file from the server's disk.
     *
     * @param owner       Owning plugin
     * @param pszName     Name to refer to the script by
     * @param pszFilePath Absolute path, or relative to the game directory (`game/csgo`)
     * @param bPersistent Run it again after a round restart or a map change; the
     *                    file is read again every time, so edits are picked up
     * @return The `point_script` entity, or an invalid handle when the file
     *         cannot be read or run
     */
    virtual CEntityHandle RunFile(PluginId owner, const char* pszName, const char* pszFilePath, bool bPersistent) = 0;

    /**
     * @brief Throws the script's entity away and runs the script again.
     *
     * A file is read again, which makes this the hot reload after an edit;
     * source and assets run as they were given.
     *
     * @return The new entity, or an invalid handle when there is no script by
     *         that name or it cannot be run
     */
    virtual CEntityHandle Reload(const char* pszName) = 0;

    /**
     * @brief Removes the script and its entity. Unknown names are ignored.
     */
    virtual void Remove(const char* pszName) = 0;

    /**
     * @brief The script's `point_script` entity, or an invalid handle.
     */
    virtual CEntityHandle Find(const char* pszName) = 0;

    /**
     * @brief Fires the callback the script registered with
     *        `Instance.OnScriptInput(pszInput, ...)`.
     *
     * This is the game's `RunScriptInput` entity input. Only the input name
     * reaches the script; data travels the other way (see
     * HookScriptMessage()), or stays on the script side.
     *
     * @return false when there is no live script by that name
     */
    virtual bool FireInput(const char* pszName, const char* pszInput) = 0;

    /**
     * @brief Receives what scripts send on a channel through `toolkit_script`.
     *
     * Several handlers may hook one channel, from one plugin or more; each
     * gets every message.
     *
     * Whose it is, the core reads off the handler (see ToolkitCallback); what
     * a plugin still holds at unload the core drops for it.
     *
     * @return The id UnhookScriptMessage(id) takes
     */
    virtual ToolkitHookId HookScriptMessage(const char* pszChannel, ScriptMessageHandler handler) = 0;

    /**
     * @brief Drops the hook with this handler -- a function or an object and
     * a method; a lambda goes by its id.
     *
     * @return true when one was found
     */
    virtual bool UnhookScriptMessage(const char* pszChannel, const ScriptMessageHandler& handler) = 0;

    /**
     * @brief Drops the hook HookScriptMessage() returned the id for.
     *
     * @return true when it was still there
     */
    virtual bool UnhookScriptMessage(ToolkitHookId id) = 0;
};

/**
 * @brief Shorthand accessors via g_pToolkitScripts.
 */
#define SCRIPTS_LOAD_ASSET(name, asset, persistent)       g_pToolkitScripts->LoadAsset(g_PluginID, name, asset, persistent)
#define SCRIPTS_RUN_SOURCE(name, path, source, persistent) g_pToolkitScripts->RunSource(g_PluginID, name, path, source, persistent)
#define SCRIPTS_RUN_FILE(name, file, persistent)          g_pToolkitScripts->RunFile(g_PluginID, name, file, persistent)
#define SCRIPTS_FIRE_INPUT(name, input)                   g_pToolkitScripts->FireInput(name, input)

#define HOOK_SCRIPT_MESSAGE(channel, handler)   g_pToolkitScripts->HookScriptMessage(channel, handler)
#define UNHOOK_SCRIPT_MESSAGE(channel, handler) g_pToolkitScripts->UnhookScriptMessage(channel, handler)

#endif //_INCLUDE_ITOOLKIT_SCRIPTS_H
