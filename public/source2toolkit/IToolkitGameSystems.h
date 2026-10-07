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

* @file IToolkitGameSystems.h
* @brief Interface for registering game systems with the engine.
*
* A game system is how the engine lets code run at fixed points of a session --
* BuildGameSessionManifest for precaching, ServerGamePostSimulate per frame, and
* the rest of the IGameSystem event set. Registering one means getting a factory
* into the engine's own linked list, whose head lives in a static the engine
* never exports.
*
* The toolkit finds that head once, by signature, and hands it over here. It has
* to be handed over rather than used directly, because
* CBaseGameSystemFactory::sm_pFirst is a static member: every binary that
* includes the s2sdk header gets its own copy, so a plugin's copy is null until
* it is pointed at the engine's.
  */

#ifndef _INCLUDE_ITOOLKIT_GAMESYSTEMS_H
#define _INCLUDE_ITOOLKIT_GAMESYSTEMS_H

#pragma once
#include "IToolkitPlugin.h"

#include "igamesystemfactory.h"

/* =========================
Core Toolkit Game Systems
========================= */

/**

* @brief Access to the engine's game system registry.
*
* Registering a game system of your own is three steps:
*
* @code
* // 1. Declare it, the way s2sdk expects.
* class MyGameSystem : public CAutoGameSystem
* {
* public:
*     DECLARE_GAME_SYSTEM();
*     GS_EVENT(BuildGameSessionManifest);
*
*     void SetGameSystemGlobalPtrs(void* pValue) override
*     {
*         if (sm_Factory) sm_Factory->SetGlobalPtr(pValue);
*     }
*     bool DoesGameSystemReallocate() override { return sm_Factory->ShouldAutoAdd(); }
*
*     // Not IGameSystemFactory*: that is abstract and has no virtual
*     // destructor, so deleting your factory through it is undefined. Hold a
*     // final type of your own instead.
*     static MyGameSystemFactory* sm_Factory;
* };
*
* class MyGameSystemFactory final : public CGameSystemStaticFactory<MyGameSystem>
* {
* public:
*     using CGameSystemStaticFactory<MyGameSystem>::CGameSystemStaticFactory;
* };
*
* MyGameSystem g_MyGameSystem;
* MyGameSystemFactory* MyGameSystem::sm_Factory = nullptr;
* CBaseGameSystemFactory** CBaseGameSystemFactory::sm_pFirst = nullptr;
*
* // 2. Point this binary's copy of sm_pFirst at the engine's list, in Load().
* CBaseGameSystemFactory::sm_pFirst = api->GameSystems()->GetFactoryListHead();
*
* // 3. Register. The factory adds itself to the list its constructor sees.
* MyGameSystem::sm_Factory =
*     new MyGameSystemFactory("MyPlugin_GameSystem", &g_MyGameSystem);
* @endcode
*
* @note Registration only takes effect for sessions started afterwards; the
*       engine walks the list when it initialises all systems.
  */
#define TOOLKIT_GAMESYSTEMS_INTERFACE "IToolkitGameSystems001"

class IToolkitGameSystems
{
public:
    virtual ~IToolkitGameSystems() = default;

    /**

    * @brief Whether the factory list head was found.
    *
    * False means the signature scan failed -- usually a game update -- and
    * registering a game system cannot work. Check before registering.
      */
    virtual bool IsAvailable() const = 0;

    /**

    * @brief The engine's game system factory list head.
    *
    * Assign this to your own binary's CBaseGameSystemFactory::sm_pFirst before
    * constructing any factory. Returns nullptr when IsAvailable() is false.
      */
    virtual CBaseGameSystemFactory** GetFactoryListHead() = 0;

    /**

    * @brief Finds an already-registered game system by the name it was
    *        registered under.
    *
    * @param name Registration name, e.g. "MyPlugin_GameSystem".
    * @return The system, or nullptr if nothing is registered under that name.
      */
    virtual IGameSystem* FindByName(const char* name) = 0;
};

#endif //_INCLUDE_ITOOLKIT_GAMESYSTEMS_H
