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

* @file IToolkitAPI.h
* @brief Main entry point for Source2Toolkit API.
*
* This interface provides access to:
* * Core engine interfaces
* * Toolkit subsystems (Commands, Events, ConVars, etc.)
* * Logging and console output
* * Interface querying and factory access
*
* @note This is the primary interface passed to plugins during initialization.
  */

#ifndef _INCLUDE_ITOOLKIT_API_H
#define _INCLUDE_ITOOLKIT_API_H

#pragma once
#include "IToolkitPaths.h"
#include "IToolkitPlugin.h"

#include <engine2/isource2server.h>
#include <engine2/ivengineserver2.h>

/* =========================
Windows macro collisions
========================= */

/*
 * windows.h, dragged in by the engine headers above, defines FreeModule as a
 * function-like macro aliasing FreeLibrary. Left alone it rewrites the
 * declaration below into FreeLibrary((IToolkitModule* module)) and every call
 * site with it. Nothing here wants the Win32 original, so drop it -- and drop
 * it at the point of use rather than in a shared header, so it happens after
 * whatever pulled windows.h in.
 */
#ifdef FreeModule
#undef FreeModule
#endif

/* =========================
Forward declarations
========================= */

using PluginId = int;

class IGameEventManager2;
class CGlobalVars;
class ICvar;
class ISource2Server;
class IGameEventSystem;
class INetworkMessages;
class INetworkServerService;
class CGameEntitySystem;
class CSchemaSystem;
class IVEngineServer2;
class ISource2ServerConfig;
class ISource2GameClients;
class ISource2GameEntities;
class IGameResourceService;
class INetworkSystem;
class INetworkStringTableContainer;
class IEngineServiceMgr;
class IHostStateMgr;
class ILocalize;
class IFileSystem;

class IToolkitAddresses;
class IToolkitCommands;
class IToolkitConVars;
class IToolkitCustomHud;
class IToolkitGameConfig;
class IToolkitGameSystems;
class IToolkitHTTP;
class IToolkitEntities;
class IToolkitEvents;
class IToolkitListener;
class IToolkitMenus;
class IToolkitModule;
class IToolkitMySQL;
class IToolkitNetworkMessages;
class IToolkitPlugin;
class IToolkitScheduler;
class IToolkitSounds;
class IToolkitTrace;
class IToolkitTransmit;
class IToolkitGameHooks;

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

/* =========================
Core Toolkit API
========================= */

/**

* @brief Main API interface for Source2Toolkit.
*
* Provides access to all toolkit subsystems and engine interfaces.
*
* @code
* api->Commands()->RegisterConCommand(...);
* api->Events()->HookGameEvent(...);
* @endcode
  */
class IToolkitAPI
{
public:
    virtual ~IToolkitAPI() = default;

    /* =========================
    Logging & console
    ========================= */

    /**

    * @brief Logs a formatted message with plugin context.
    *
    * @param plugin Plugin instance
    * @param msg Format string
      */
    virtual void Log(IToolkitPlugin* plugin, const char* msg, ...) = 0;

    /**

    * @brief Prints message to console.
      */
    virtual void ConPrint(const char* msg) = 0;

    /**

    * @brief Prints formatted message to console.
      */
    virtual void ConPrintf(const char* fmt, ...) = 0;

    /* =========================
    Listeners
    ========================= */

    /**

    * @brief Registers a toolkit listener.
    *
    * @param plugin Owner plugin
    * @param pListener Listener instance
      */
    virtual void AddListener(IToolkitPlugin* plugin, IToolkitListener* pListener) = 0;

    /* =========================
    Interface system
    ========================= */

    /**

    * @brief Queries interface from factory.
    *
    * @param fn Factory function
    * @param iface Interface name
    * @param min Minimum version (optional)
      */
    virtual void* QueryInterface(CreateInterfaceFn fn, const char* iface, int min = -1) = 0;

    /**

    * @brief Searches for interface by iterating versions.
      */
    virtual void* InterfaceSearch(CreateInterfaceFn fn, const char* iface, int max, int* ret) = 0;

    /**

    * @brief Formats interface name with version.
      */
    virtual int FormatIface(char iface[], size_t maxlength) = 0;

    /* =========================
    Engine factories
    ========================= */

    virtual CreateInterfaceFn GetEngineFactory(bool syn = true) = 0;
    virtual CreateInterfaceFn GetPhysicsFactory(bool syn = true) = 0;
    virtual CreateInterfaceFn GetFileSystemFactory(bool syn = true) = 0;
    virtual CreateInterfaceFn GetServerFactory(bool syn = true) = 0;

    /**

    * @brief Toolkit interface factory.
      */
    virtual void* ToolkitFactory(const char* iface, int* ret, PluginId* id) = 0;

    /**

    * @brief Metamod interface factory.
      */
    virtual void* MetaFactory(const char* iface, int* ret, PluginId* id) = 0;

    /* =========================
    Toolkit subsystems
    ========================= */

    /**

    * @brief Access address/pattern system.
      */
    virtual IToolkitAddresses* Addresses() = 0;

    /**

    * @brief Access command system.
      */
    virtual IToolkitCommands* Commands() = 0;

    /**

    * @brief Access ConVar system.
      */
    virtual IToolkitConVars* ConVars() = 0;

    /**

    * @brief Access custom HUD layout system.
      */
    virtual IToolkitCustomHud* CustomHud() = 0;

    /**

    * @brief Access entities and Entity I/O system.
      */
    virtual IToolkitEntities* Entities() = 0;

    /**

    * @brief Access event system.
      */
    virtual IToolkitEvents* Events() = 0;

    /**

    * @brief Access game configuration system.
      */
    virtual IToolkitGameConfig* GameConfig() = 0;

    /**

    * @brief Access game system registry.
      */
    virtual IToolkitGameSystems* GameSystems() = 0;

    /**

    * @brief Access HTTP system.
      */
    virtual IToolkitHTTP* HTTP() = 0;

    /**

    * @brief Access menu system.
      */
    virtual IToolkitMenus* Menus() = 0;

    /**

    * @brief Access mysql system.
      */
    virtual IToolkitMySQL* MySQL() = 0;

    /**

    * @brief Access network messages system.
      */
    virtual IToolkitNetworkMessages* NetworkMessages() = 0;

    /**

    * @brief Where the toolkit keeps things on disk.
      */
    virtual IToolkitPaths* Paths() = 0;

    /**

    * @brief Access scheduler system.
      */
    virtual IToolkitScheduler* Scheduler() = 0;

    /**

    * @brief Access sound system.
      */
    virtual IToolkitSounds* Sounds() = 0;

    /**

    * @brief Access tracing system.
      */
    virtual IToolkitTrace* Trace() = 0;

    /**

    * @brief Access transmit system (per-viewer entity visibility).
      */
    virtual IToolkitTransmit* Transmit() = 0;

    /* =========================
    Module factory
    ========================= */

    /**

    * @brief Creates and initialises a module wrapper by name.
    *
    * The returned object must be released with FreeModule().
    *
    * @param name Module name without extension (e.g. "server").
    * @return Heap-allocated IToolkitModule, or nullptr on failure.
      */
    virtual IToolkitModule* LoadModule(const char* name) = 0;

    /**

    * @brief Creates and initialises a module wrapper from an address inside it.
    *
    * The returned object must be released with FreeModule().
    *
    * @param ptr Any address that belongs to the target module.
    * @return Heap-allocated IToolkitModule, or nullptr on failure.
      */
    virtual IToolkitModule* LoadModuleFromMemory(uintptr_t ptr) = 0;

    /**

    * @brief Releases a module previously obtained from LoadModule() / LoadModuleFromMemory().
      */
    virtual void FreeModule(IToolkitModule* module) = 0;

    /* =========================
    Engine interfaces
    ========================= */

    virtual ISource2Server* GetSource2Server() = 0;
    virtual ISource2ServerConfig* GetSource2ServerConfig() = 0;
    virtual ISource2GameClients* GetSource2GameClients() = 0;
    virtual ISource2GameEntities* GetSource2GameEntities() = 0;
    virtual IVEngineServer2* GetEngineServer() = 0;
    virtual ICvar* GetCVar() = 0;
    virtual CSchemaSystem* GetSchemaSystem() = 0;
    virtual IGameResourceService* GetGameResourceService() = 0;
    virtual INetworkServerService* GetNetworkServerService() = 0;
    virtual INetworkSystem* GetNetworkSystem() = 0;
    virtual INetworkMessages* GetNetworkMessages() = 0;
    virtual INetworkStringTableContainer* GetNetworkStringTableServer() = 0;
    virtual IEngineServiceMgr* GetEngineServiceMgr() = 0;
    virtual IHostStateMgr* GetHostStateMgr() = 0;
    virtual ILocalize* GetLocalize() = 0;
    virtual IFileSystem* GetFileSystem() = 0;

    virtual IGameEventSystem* GetGameEventSystem() = 0;
    virtual IGameEventManager2* GetGameEventManager() = 0;
    virtual CGameEntitySystem* GetEntitySystem() = 0;
    virtual CGlobalVars* GetGlobalVars() = 0;

    /**

    * @brief Returns base directory of the server.
      */
    virtual const char* GetBaseDir() = 0;

    /* =========================
    Formatting utilities
    ========================= */

    /**

    * @brief Formats string into buffer.
      */
    virtual size_t Format(char* buffer, size_t maxlength, const char* format, ...) = 0;

    /**

    * @brief Formats string using va_list.
      */
    virtual size_t FormatArgs(char* buffer, size_t maxlength, const char* format, va_list ap) = 0;

    /* =========================
    Added in plugin API 2 -- at the end, so a plugin built against API 1
    still finds every slot above where it left it.
    ========================= */

    /**

    * @brief Access the core's hooks on common game functions.
      */
    virtual IToolkitGameHooks* GameHooks() = 0;
};

#endif //_INCLUDE_ITOOLKIT_API_H
