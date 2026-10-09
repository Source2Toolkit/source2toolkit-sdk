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

* @file IToolkitPlugin.h
* @brief Plugin interface and integration layer for Source2Toolkit.
*
* This file defines:
* * Plugin lifecycle interface (IToolkitPlugin)
* * Listener system (IToolkitListener)
* * Global variables and exposure macros
* * Interface helper macros
*
* @note This is the main entry point for plugin developers.
  */

#ifndef _INCLUDE_ITOOLKIT_PLUGIN_H
#define _INCLUDE_ITOOLKIT_PLUGIN_H

#pragma once
#include "interfaces/interfaces.h"
// The engine callbacks on IToolkitListener take the engine's own types.
#include "eiface.h"
#include "iserver.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>

// Action, the result every toolkit callback hands back, and KHook -- the
// detour library (metamod's) plugins hook with.
#include "IToolkitTypes.h"
#include "khook.hpp"

/* =========================
Forward declarations
========================= */

using PluginId = int;
class IToolkitAPI;

// The TOOLKIT_EXPOSE/TOOLKIT_GLOBALVARS globals below are plain pointers, so
// forward declarations are enough -- and they have to be here rather than rely
// on the includes underneath. A TU that reaches this header *through* one of
// those interface headers (e.g. IToolkitAddresses.h -> here) finds that
// header's guard already set, so its include below is a no-op and the class
// would otherwise still be unknown when the macros expand.
class IToolkitAddresses;
class IToolkitCommands;
class IToolkitConVars;
class IToolkitEntities;
class IToolkitEvents;
class IToolkitGameConfig;
class IToolkitMenus;
class IToolkitMySQL;
class IToolkitNetworkMessages;
class IToolkitPermissions;
class IToolkitScheduler;
class IToolkitScripts;
class IToolkitSounds;
class IToolkitTrace;
class IToolkitTransmit;

// Everything below this line may use Action and the forward declarations
// above. Nothing above it can: the interface headers include this
// one back, and by then its guard is already set, so anything declared after
// them would be invisible to them.

#include "IToolkitAddresses.h"
#include "IToolkitCommands.h"
#include "IToolkitConVars.h"
#include "IToolkitCustomHud.h"
#include "IToolkitEntities.h"
#include "IToolkitEvents.h"
#include "IToolkitGameConfig.h"
#include "IToolkitGameHooks.h"
#include "IToolkitGameSystems.h"
#include "IToolkitHTTP.h"
#include "IToolkitJSON.h"
#include "IToolkitMenus.h"
#include "IToolkitMySQL.h"
#include "IToolkitNetworkMessages.h"
#include "IToolkitPaths.h"
#include "IToolkitPermissions.h"
#include "IToolkitScheduler.h"
#include "IToolkitScripts.h"
#include "IToolkitSounds.h"
#include "IToolkitTrace.h"
#include "IToolkitTransmit.h"

/* =========================
Interface status
========================= */

enum
{
    /// Interface query successful
    TOOLKIT_IFACE_OK = 0,
    /// Interface query failed
    TOOLKIT_IFACE_FAILED
};

/* =========================
Export system
========================= */

/// KHook, served through ToolkitFactory.
///
/// The toolkit is a metamod plugin and gets metamod's detour engine handed
/// to it at load; this is how that same engine reaches a toolkit plugin, so
/// every hook on the server -- metamod's, the toolkit's, every plugin's --
/// runs on one instance. TOOLKIT_SAVEVARS() does the fetch.
#define TOOLKIT_GAMEHOOKS_INTERFACE "IToolkitGameHooks003"
#define TOOLKIT_KHOOK_INTERFACE "S2ToolkitKHook001"

/// The commit of the KHook the core was compiled against, as a const char*
/// (nullptr from a core that predates the check). TOOLKIT_SAVEVARS() compares
/// it with TOOLKIT_KHOOK_COMMIT, the commit this plugin was compiled against,
/// and refuses the load on a mismatch: the header and the engine are one
/// library, and a plugin that hooks through a different KHook than the one
/// running is undefined behaviour with a delay on it.
#define TOOLKIT_KHOOK_VERSION_INTERFACE "S2ToolkitKHookVersion001"

#ifndef TOOLKIT_KHOOK_COMMIT
/// Set by the SDK's CMakeLists.txt / AMBuildScript from vendor/khook (the
/// core: from metamod's copy). A build that bypassed both gets "unknown",
/// which the check reports and lets through.
#define TOOLKIT_KHOOK_COMMIT "unknown"
#endif

/// The load-time half of the KHook check; TOOLKIT_SAVEVARS() calls it.
///
/// Returns false and fills `error` only when both commits are known and
/// differ. A side that cannot say -- a core too old to answer, a plugin built
/// without git -- makes the check impossible, not failed: that goes out
/// through `note` and the load goes on.
inline bool ToolkitKHookVersionMatches(const char* coreCommit, char* error, size_t maxlen, const char** note)
{
    if (note)
        *note = nullptr;

    if (!coreCommit || !coreCommit[0])
    {
        if (note)
            *note = "the toolkit core does not report its KHook commit (older core), KHook version check skipped";
        return true;
    }

    if (strcmp(TOOLKIT_KHOOK_COMMIT, "unknown") == 0)
    {
        if (note)
            *note = "this plugin carries no KHook commit (built without git), KHook version check skipped";
        return true;
    }

    if (strcmp(coreCommit, TOOLKIT_KHOOK_COMMIT) == 0)
        return true;

    if (error && maxlen)
        snprintf(error, maxlen,
                 "KHook mismatch: plugin built against %.12s, toolkit core runs %.12s -- "
                 "rebuild the plugin with the SDK's vendor/khook at the core's commit, or update the core",
                 TOOLKIT_KHOOK_COMMIT, coreCommit);
    return false;
}

/// Plugin interface name
#define TOOLKIT_INTERFACE_NAME "S2ToolkitPlugin001"

/// Plugin API version
// 2 (v1.0.26): engine callbacks on IToolkitListener (OnGameFrame ...),
// IToolkitAPI::GameHooks(). A plugin built against an older version loads
// (the core calls nothing it does not have); one built against a newer
// version than the core's is refused.
// 3 (v1.0.37): OnClientAuthorized, OnClientAuthorizeFailed and
// OnPermissionsChanged on IToolkitListener.
#define TOOLKIT_PLAPI_VERSION 3

/// Plugin API interface name
#define TOOLKIT_PLAPI_NAME "IToolkitPlugin"

/* =========================
Plugin Interface
========================= */

/**

* @brief Main plugin interface.
*
* Every plugin must implement this interface.
  */
/// Defined by IToolkitHooks.h for a plugin; the core never asks itself.
int ToolkitRawHookCount();
#ifdef SOURCE2TOOLKIT_CORE
inline int ToolkitRawHookCount() { return 0; }
#endif

class IToolkitPlugin
{
public:
    virtual int GetApiVersion()
    {
        return TOOLKIT_PLAPI_VERSION;
    }

    virtual ~IToolkitPlugin() = default;

public:
    /**
    * @brief Called when plugin is loaded.
    *
    * @param id Plugin identifier
    * @param api Toolkit API instance
    * @param error Buffer for error message
    * @param maxlen Error buffer size
    * @param late False only when the toolkit's startup pass loads the plugin.
    *             True for every load after that: "toolkit load", "toolkit
    *             refresh" and the file watcher's hot reload.
    *             Players and entities may already exist.
    *
    * @return True on success, false on failure
    *
    * @note This is the main initialization function.
    */
    virtual bool Load(PluginId id, IToolkitAPI* api, char* error, size_t maxlen, bool late) = 0;

    /**
     * @brief Called when plugin is unloaded.
     *
     * @return True on success
     */
    virtual bool Unload(char* error, size_t maxlen)
    {
        return true;
    }

public:
    /// Plugin author name
    virtual const char* GetAuthor() = 0;

    /// Plugin name
    virtual const char* GetName() = 0;

    /// Plugin description
    virtual const char* GetDescription() = 0;

    /// Plugin version
    virtual const char* GetVersion() = 0;

    /// The KHook commit this plugin was built against (the SDK's
    /// vendor/khook), "" when built without git.
    virtual const char* GetKHookCommit()
    {
#ifdef TOOLKIT_KHOOK_COMMIT
        return TOOLKIT_KHOOK_COMMIT;
#else
        return "";
#endif
    }

    /// How many KHook hooks the plugin declared through the KHOOK_* macros:
    /// none means the plugin is on the stable tier and an engine update is
    /// the core's problem alone.
    virtual int GetRawHookCount()
    {
        return ::ToolkitRawHookCount();
    }
};

/* =========================
Listener Interface
========================= */

/**

* @brief Listener for toolkit and engine events.
*
* Allows plugins to react to lifecycle and map events.
  */
class IToolkitListener
{
public:
    virtual ~IToolkitListener() = default;

    /// Called when a toolkit plugin (.stx) has loaded, after its Load() returned.
    /// The id is a toolkit plugin id -- the one "toolkit list" shows.
    /// Fires for your own plugin too (a listener added in Load() gets it
    /// right after Load() returns); compare against your own id to skip it.
    /// The name is what the plugin's GetName() returns.
    virtual void OnPluginLoad(PluginId id, const char* name)
    {
    }

    /// Called when a toolkit plugin (.stx) is about to unload, before its
    /// Unload() runs. Fires for your own plugin too, like OnPluginLoad().
    virtual void OnPluginUnload(PluginId id, const char* name)
    {
    }

    /// Called when a Metamod:Source plugin is loaded.
    ///
    /// The id is Metamod's, from a different numbering than the toolkit's --
    /// which is why this is not the same callback as OnPluginLoad().
    virtual void OnMetamodPluginLoad(PluginId id)
    {
    }

    /// Called when a Metamod:Source plugin is unloaded.
    virtual void OnMetamodPluginUnload(PluginId id)
    {
    }

    /// Called when all toolkit plugins are loaded
    virtual void OnAllToolkitPluginsLoaded()
    {
    }

    /// Called when all Metamod plugins are loaded
    virtual void OnAllMetamodPluginsLoaded()
    {
    }

    /**

    * @brief Called when a map is initialized.
      */
    virtual void OnLevelInit(const char* mapName,
                             const char* mapEntities,
                             const char* oldLevel,
                             const char* landmarkName,
                             bool loadGame,
                             bool background)
    {
    }

    /// Called when a map is shutting down
    virtual void OnLevelShutdown()
    {
    }

    /**

    * @brief Called when an interface is requested from toolkit.
    *
    * @return Pointer to interface or nullptr
      */
    virtual void* OnToolkitQuery(const char* iface, int* ret)
    {
        if (ret)
        {
            *ret = TOOLKIT_IFACE_FAILED;
        }

        return nullptr;
    }

    /**

    * @brief Called when a Metamod:Source plugin asks for an interface.
    *
    * A Metamod plugin reaching for something through g_SMAPI->MetaFactory()
    * ends up here, so a toolkit plugin can hand its own interfaces to code
    * that did not load through the toolkit.
    *
    * By default this answers exactly like OnToolkitQuery(), which is what a
    * plugin usually wants -- an interface worth exposing is worth exposing to
    * either caller. Override it to answer a Metamod plugin differently, or
    * return nullptr to keep an interface for toolkit plugins only. The two are
    * separate callbacks precisely so that choice exists: OnToolkitQuery()
    * cannot tell who is asking.
    *
    * @return Pointer to interface or nullptr
      */
    virtual void* OnMetamodQuery(const char* iface, int* ret)
    {
        return OnToolkitQuery(iface, ret);
    }

    /// Post ISource2Server::GameFrame: once a frame, after the toolkit's own
    /// per-frame work (timers, menus).
    virtual void OnGameFrame(bool simulating, bool firstTick, bool lastTick)
    {
    }

    /// Post INetworkServerService::StartupServer: a new server session; the
    /// entity system of the new map is already the toolkit's.
    virtual void OnStartupServer(const GameSessionConfiguration_t& config, ISource2WorldSession* session, const char* mapName)
    {
    }

    /// Post IServerGameClients::ClientPutInServer.
    virtual void OnClientPutInServer(CPlayerSlot slot, const char* name, int type, uint64 xuid)
    {
    }

    /// Post IServerGameClients::ClientDisconnect.
    virtual void OnClientDisconnect(CPlayerSlot slot, ENetworkDisconnectionReason reason, const char* name, uint64 xuid, const char* networkId)
    {
    }

    /// Steam validated the player in this slot: from now on they are checked
    /// under `steamId` whatever SteamAuthMode says. Not called for bots.
    virtual void OnClientAuthorized(CPlayerSlot slot, uint64 steamId)
    {
    }

    /// Steam refused the ticket of the player in this slot; `steamId` is the
    /// one they claimed.
    virtual void OnClientAuthorizeFailed(CPlayerSlot slot, uint64 steamId)
    {
    }

    /// Post IServerGameClients::ClientVoice.
    virtual void OnClientVoice(CPlayerSlot slot)
    {
    }

    /// Post IServerGameClients::ClientSettingsChanged.
    virtual void OnClientSettingsChanged(CPlayerSlot slot)
    {
    }

    /// What this SteamID64 holds may have changed -- a grant, a group, a
    /// reload. 0 when a change can touch anybody (a group was edited,
    /// permissions.json was re-read).
    virtual void OnPermissionsChanged(uint64 steamId)
    {
    }

    /// Post ISource2Server::GameServerSteamAPIActivated: the Steam API is up.
    virtual void OnGameServerSteamAPIActivated()
    {
    }

    /// Pre ISource2Server::GameServerSteamAPIDeactivated: the Steam API is
    /// about to go.
    virtual void OnGameServerSteamAPIDeactivated()
    {
    }

    /// Post IGameEventManager2::LoadEventsFromFile, with the manager it was
    /// called on: the place to keep it, or to register a .gameevents file of
    /// your own.
    virtual void OnLoadEventsFromFile(IGameEventManager2* manager, const char* filename, bool searchAll)
    {
    }
};

/* =========================
Plugin exposure
========================= */

/**

* @brief Exposes plugin interface to engine.
  */
#define TOOLKIT_EXPOSURE_FUNC(name, var)	EXPOSE_SINGLE_INTERFACE_GLOBALVAR(IToolkitPlugin, IToolkitPlugin, TOOLKIT_PLAPI_NAME, var);

/* =========================
Globals
========================= */

/**

* @brief Defines the toolkit globals -- the API, the plugin identity and one
* pointer per toolkit interface.
*
* Part of TOOLKIT_EXPOSE for a plugin. The core expands it on its own too and
* fills the pointers with TOOLKIT_FILLVARS(), so SDK code reads the same
* globals (ADDR_*, g_pToolkitGameConfig, ...) whichever binary it is linked into.
  */
#define TOOLKIT_DEFINE_GLOBALVARS() \
    IToolkitAPI*             g_ToolkitAPI              = nullptr; \
    IToolkitPlugin*          g_PluginAPI               = nullptr; \
    PluginId                 g_PluginID                = 0; \
    const char*             g_ToolkitCoreKHookCommit = nullptr; \
    IToolkitAddresses*       g_pToolkitAddresses       = nullptr; \
    IToolkitCommands*        g_pToolkitCommands        = nullptr; \
    IToolkitConVars*         g_pToolkitConVars         = nullptr; \
    IToolkitCustomHud*       g_pToolkitCustomHud       = nullptr; \
    IToolkitEntities*        g_pToolkitEntities        = nullptr; \
    IToolkitEvents*          g_pToolkitEvents          = nullptr; \
    IToolkitGameConfig*      g_pToolkitGameConfig      = nullptr; \
    IToolkitGameHooks*       g_pToolkitGameHooks       = nullptr; \
    IToolkitGameSystems*     g_pToolkitGameSystems     = nullptr; \
    IToolkitHTTP*            g_pToolkitHTTP            = nullptr; \
    IToolkitJSON*            g_pToolkitJSON            = nullptr; \
    IToolkitMenus*           g_pToolkitMenus           = nullptr; \
    IToolkitMySQL*           g_pToolkitMySQL           = nullptr; \
    IToolkitNetworkMessages* g_pToolkitNetworkMessages = nullptr; \
    IToolkitPaths*           g_pToolkitPaths           = nullptr; \
    IToolkitPermissions*     g_pToolkitPermissions     = nullptr; \
    IToolkitScheduler*       g_pToolkitScheduler       = nullptr; \
    IToolkitScripts*         g_pToolkitScripts         = nullptr; \
    IToolkitSounds*          g_pToolkitSounds          = nullptr; \
    IToolkitTrace*           g_pToolkitTrace           = nullptr; \
    IToolkitTransmit*        g_pToolkitTransmit        = nullptr;

/**

* @brief Defines global plugin variables.
*
* Creates:
* * everything TOOLKIT_DEFINE_GLOBALVARS() does
* * KHook::__exported__khook, the detour engine (see TOOLKIT_KHOOK_INTERFACE)
    */
#define TOOLKIT_EXPOSE(name, var) \
    TOOLKIT_DEFINE_GLOBALVARS() \
    namespace KHook { KHook::IKHook* __exported__khook = nullptr; } \
    TOOLKIT_EXPOSURE_FUNC(name, var)

/**

* @brief Declares external globals.
  */
#define TOOLKIT_GLOBALVARS() \
    extern IToolkitAPI*             g_ToolkitAPI; \
    extern IToolkitPlugin*          g_PluginAPI; \
    extern PluginId                 g_PluginID; \
    extern const char*             g_ToolkitCoreKHookCommit; \
    extern IToolkitAddresses*       g_pToolkitAddresses; \
    extern IToolkitCommands*        g_pToolkitCommands; \
    extern IToolkitConVars*         g_pToolkitConVars; \
    extern IToolkitCustomHud*       g_pToolkitCustomHud; \
    extern IToolkitEntities*        g_pToolkitEntities; \
    extern IToolkitEvents*          g_pToolkitEvents; \
    extern IToolkitGameConfig*      g_pToolkitGameConfig; \
    extern IToolkitGameHooks*       g_pToolkitGameHooks; \
    extern IToolkitGameSystems*     g_pToolkitGameSystems; \
    extern IToolkitHTTP*            g_pToolkitHTTP; \
    extern IToolkitJSON*            g_pToolkitJSON; \
    extern IToolkitMenus*           g_pToolkitMenus; \
    extern IToolkitMySQL*           g_pToolkitMySQL; \
    extern IToolkitNetworkMessages* g_pToolkitNetworkMessages; \
    extern IToolkitPaths*           g_pToolkitPaths; \
    extern IToolkitPermissions*     g_pToolkitPermissions; \
    extern IToolkitScheduler*       g_pToolkitScheduler; \
    extern IToolkitScripts*         g_pToolkitScripts; \
    extern IToolkitSounds*          g_pToolkitSounds; \
    extern IToolkitTrace*           g_pToolkitTrace; \
    extern IToolkitTransmit*        g_pToolkitTransmit; \
    namespace KHook { extern KHook::IKHook* __exported__khook; }

/**

* @brief Points every toolkit interface global at what the API hands out.
*
* Part of TOOLKIT_SAVEVARS() for a plugin; the core calls it once its
* interfaces exist (see TOOLKIT_DEFINE_GLOBALVARS()).
  */
#define TOOLKIT_FILLVARS(api) \
    g_pToolkitAddresses       = (IToolkitAddresses*)      (api)->ToolkitFactory(TOOLKIT_ADDRESSES_INTERFACE,       nullptr, nullptr); \
    g_pToolkitCommands        = (IToolkitCommands*)       (api)->ToolkitFactory(TOOLKIT_COMMANDS_INTERFACE,        nullptr, nullptr); \
    g_pToolkitConVars         = (IToolkitConVars*)        (api)->ToolkitFactory(TOOLKIT_CONVARS_INTERFACE,         nullptr, nullptr); \
    g_pToolkitCustomHud       = (IToolkitCustomHud*)      (api)->ToolkitFactory(TOOLKIT_CUSTOMHUD_INTERFACE,       nullptr, nullptr); \
    g_pToolkitEntities        = (IToolkitEntities*)       (api)->ToolkitFactory(TOOLKIT_ENTITIES_INTERFACE,        nullptr, nullptr); \
    g_pToolkitEvents          = (IToolkitEvents*)         (api)->ToolkitFactory(TOOLKIT_EVENTS_INTERFACE,          nullptr, nullptr); \
    g_pToolkitGameConfig      = (IToolkitGameConfig*)     (api)->ToolkitFactory(TOOLKIT_GAMECONFIG_INTERFACE,      nullptr, nullptr); \
    g_pToolkitGameHooks       = (IToolkitGameHooks*)      (api)->ToolkitFactory(TOOLKIT_GAMEHOOKS_INTERFACE,       nullptr, nullptr); \
    g_pToolkitGameSystems     = (IToolkitGameSystems*)    (api)->ToolkitFactory(TOOLKIT_GAMESYSTEMS_INTERFACE,     nullptr, nullptr); \
    g_pToolkitHTTP            = (IToolkitHTTP*)           (api)->ToolkitFactory(TOOLKIT_HTTP_INTERFACE,            nullptr, nullptr); \
    g_pToolkitMenus           = (IToolkitMenus*)          (api)->ToolkitFactory(TOOLKIT_MENUS_INTERFACE,           nullptr, nullptr); \
    g_pToolkitMySQL           = (IToolkitMySQL*)          (api)->ToolkitFactory(TOOLKIT_MYSQL_INTERFACE,           nullptr, nullptr); \
    g_pToolkitNetworkMessages = (IToolkitNetworkMessages*)(api)->ToolkitFactory(TOOLKIT_NETWORKMESSAGES_INTERFACE, nullptr, nullptr); \
    g_pToolkitPaths           = (IToolkitPaths*)          (api)->ToolkitFactory(TOOLKIT_PATHS_INTERFACE,           nullptr, nullptr); \
    g_pToolkitPermissions     = (IToolkitPermissions*)    (api)->ToolkitFactory(TOOLKIT_PERMISSIONS_INTERFACE,     nullptr, nullptr); \
    g_pToolkitJSON            = (IToolkitJSON*)           (api)->ToolkitFactory(TOOLKIT_JSON_INTERFACE,            nullptr, nullptr); \
    g_pToolkitScheduler       = (IToolkitScheduler*)      (api)->ToolkitFactory(TOOLKIT_SCHEDULER_INTERFACE,       nullptr, nullptr); \
    g_pToolkitScripts         = (IToolkitScripts*)        (api)->ToolkitFactory(TOOLKIT_SCRIPTS_INTERFACE,         nullptr, nullptr); \
    g_pToolkitSounds          = (IToolkitSounds*)         (api)->ToolkitFactory(TOOLKIT_SOUNDS_INTERFACE,          nullptr, nullptr); \
    g_pToolkitTrace           = (IToolkitTrace*)          (api)->ToolkitFactory(TOOLKIT_TRACE_INTERFACE,           nullptr, nullptr); \
    g_pToolkitTransmit        = (IToolkitTransmit*)       (api)->ToolkitFactory(TOOLKIT_TRANSMIT_INTERFACE,        nullptr, nullptr);

/**

* @brief Initializes global variables inside Load().
*
* @note Must be called in plugin Load(), with its parameters under their
*       declared names (id, api, error, maxlen): the KHook version check in
*       here writes `error` and returns false out of Load() when the KHook
*       this plugin was compiled against is not the one the core runs
*       (see TOOLKIT_KHOOK_VERSION_INTERFACE).
  */
// The KHook commit check is not here any more: a plugin that never hooks
// (nothing but the toolkit's own interfaces) does not care which KHook the
// core runs. KHOOK_INIT() makes the check, the first time KHook is used.
#define TOOLKIT_SAVEVARS() \
    g_ToolkitAPI = api; \
    /* s2sdk's GameEntitySystem() (entity2/entitysystem.cpp) reads it */ \
    g_pGameResourceServiceServer = api->GetGameResourceService(); \
    g_PluginAPI  = static_cast<IToolkitPlugin*>(this); \
    g_PluginID   = id; \
    KHook::__exported__khook = static_cast<KHook::IKHook*>(api->ToolkitFactory(TOOLKIT_KHOOK_INTERFACE, nullptr, nullptr)); \
    g_ToolkitCoreKHookCommit = static_cast<const char*>(api->ToolkitFactory(TOOLKIT_KHOOK_VERSION_INTERFACE, nullptr, nullptr)); \
    TOOLKIT_FILLVARS(api)

/* =========================
Logging helpers
========================= */

/// Shortcut for logging
#define TOOLKIT_LOG        g_ToolkitAPI->Log

/// Console print
#define TOOLKIT_CONPRINT   g_ToolkitAPI->ConPrint

/// Console printf
#define TOOLKIT_CONPRINTF  g_ToolkitAPI->ConPrintf

/* =========================
Interface helpers
========================= */

/**
 * @brief Macro for automatically getting a current or newer Valve interface.
 *
 * @param v_factory		Factory method to use from IToolkitApi (such as engineFactory).
 * @param v_var			Variable name to store into.
 * @param v_type		Interface type (do not include the pointer/asterisk).
 * @param v_name		Interface name.
 */
#define GET_VALVE_IFACE_CURRENT(v_factory, v_var, v_type, v_name) \
	v_var = (v_type *)g_ToolkitAPI->QueryInterface(g_ToolkitAPI->v_factory(), v_name); \
	if (!v_var) \
	{ \
		if (error && maxlen) \
		{ \
			g_ToolkitAPI->Format(error, maxlen, "Could not find interface: %s", v_name); \
		} \
		return false; \
	}

/**
 * @brief Same as GET_IFACE, except searches for any.
 *
 * @param v_factory	Factory method to use from IToolkitApi (such as engineFactory).
 * @param v_var		Variable name to store into.
 * @param v_type		Interface type (do not include the pointer/asterisk).
 * @param v_name		Interface name.
 */
#define GET_VALVE_IFACE_ANY(v_factory, v_var, v_type, v_name) \
	v_var = (v_type *)g_ToolkitAPI->QueryInterface(g_ToolkitAPI->v_factory(), v_name, 0); \
	if (!v_var) \
	{ \
		if (error && maxlen) \
		{ \
			g_ToolkitAPI->Format(error, maxlen, "Could not find interface: %s", v_name); \
		} \
		return false; \
	}

/**
 * @brief Macro for automatically getting a Source2Toolkit plugin exposed interface.
 *
 * Unlike the GET_VALVE_IFACE_* macros this one never returns on its own, so it
 * is usable outside Load() -- in OnAllToolkitPluginsLoaded(), say, where the
 * interface belongs to another plugin and its absence is an ordinary outcome
 * rather than a fatal one. Test v_ret against TOOLKIT_IFACE_OK and decide.
 *
 * @param v_var		Variable name to store into.
 * @param v_type		Interface type (do not include the pointer/asterisk).
 * @param v_name		Interface name.
 * @param v_ret		int receiving TOOLKIT_IFACE_OK or TOOLKIT_IFACE_FAILED.
 */
#define GET_TOOLKIT_IFACE(v_var, v_type, v_name, v_ret) \
	v_var = (v_type *)g_ToolkitAPI->ToolkitFactory(v_name, &(v_ret), nullptr); \
	if (!v_var) \
	{ \
		(v_ret) = TOOLKIT_IFACE_FAILED; \
	}

/**
 * @brief Macro for automatically getting a Metamod:Source plugin exposed interface.
 *
 * Same contract as GET_TOOLKIT_IFACE: it never returns on its own, it only
 * reports through v_ret. Metamod fills v_ret with META_IFACE_OK/META_IFACE_FAILED,
 * which are binary-compatible with the TOOLKIT_IFACE_* values, so compare
 * against those rather than pulling a metamod header in here.
 *
 * @param v_var		Variable name to store into.
 * @param v_type		Interface type (do not include the pointer/asterisk).
 * @param v_name		Interface name.
 * @param v_ret		int receiving TOOLKIT_IFACE_OK or TOOLKIT_IFACE_FAILED.
 */
#define GET_METAMOD_IFACE(v_var, v_type, v_name, v_ret) \
	v_var = (v_type *)g_ToolkitAPI->MetaFactory(v_name, &(v_ret), nullptr); \
	if (!v_var) \
	{ \
		(v_ret) = TOOLKIT_IFACE_FAILED; \
	}

// Last, on purpose: the hook macros need the globals TOOLKIT_GLOBALVARS() above
// declares, and are meant to be there wherever this header is.
#include "IToolkitHooks.h"

#endif //_INCLUDE_ITOOLKIT_PLUGIN_H
