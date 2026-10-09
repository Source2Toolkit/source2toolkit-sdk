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

* @file entityio.h
* @brief Entity I/O system (inputs/outputs) for Source2Toolkit.
*
* Provides:
* * Access to entity outputs (OnTrigger, OnUse, etc.)
* * Hooking system for listening to outputs
* * Filtering by entity + classname + output name
*
* @note Equivalent to Hammer I/O system (FireOutput / AcceptInput).
*/

#ifndef ENTITYIO_H
#define ENTITYIO_H

#pragma once

#ifndef NULL
#define NULL 0
#endif

#include "entityhandle.h"
#include "entityinstance.h"
#include "entitysystem.h"
#include "entity2/ientity2_entityio.h"
#include "entity2/entitycomponent.h"
#include "string_t.h"
#include "source2toolkit/IToolkitPlugin.h"

/* =========================
Connections and outputs
========================= */

// EntityIOConnectionDesc_t, EntityIOConnection_t, CEntityIOOutput (whose m_pDesc
// is an EntOutput_t) and CEntityOutputTemplate<T> are s2sdk's:
// entity2/ientity2_entityio.h and entity2/entitycomponent.h.

/* =========================
Input system
========================= */

/**

* @brief Data passed to inputs.
  */
class InputData_t
{
public:
    CBaseEntity* pActivator; ///< Entity that triggered the input
    CBaseEntity* pCaller; ///< Entity owning the output
    variant_t value; ///< Passed value
    int nOutputID;
};

/* =========================
Listener system
========================= */

/**

* @brief Interface for listening to entity outputs.
  */
class IEntityIOListener
{
public:
    virtual ~IEntityIOListener() = default;

    /**

    * @brief Called when an entity fires an output.
    *
    * @param pchOutputName Output name (e.g. "OnTrigger")
    * @param pActivator Activator entity
    * @param pCaller Caller entity
    * @param flDelay Delay before execution
    * @param post false when called before the output fires, true after
    *
    * @return Action (Action::Ignore / Action::Override / Action::Supersede)
      */
    virtual Action OnEntityOutput(const char* pchOutputName,
                                  CEntityInstance* pActivator,
                                  CEntityInstance* pCaller,
                                  float flDelay,
                                  bool post)
    {
        return Action::Ignore;
    }
};

/**

* @brief Listener bound to a specific entity.
  */
class CSingleEntityIOListener : public IEntityIOListener
{
public:
    CEntityInstance* m_pTarget;

    /// Callback handler
    std::function<Action(const char*, CEntityInstance*, CEntityInstance*, float, bool)> m_Callback;

    CSingleEntityIOListener(CEntityInstance* target,
                            std::function<Action(const char*, CEntityInstance*, CEntityInstance*, float, bool)> cb)
        : m_pTarget(target), m_Callback(std::move(cb))
    {
    }

    Action OnEntityOutput(const char* outputName,
                          CEntityInstance* pActivator,
                          CEntityInstance* pCaller,
                          float delay,
                          bool post) override
    {
        if (pCaller != m_pTarget)
            return Action::Ignore;

        return m_Callback(outputName, pActivator, pCaller, delay, post);
    }
};

/**

* @brief RAII handle for entity output listeners.
*
* Automatically unregisters listener on destruction.
  */
class CEntityIOListenerHandle
{
public:
    CSingleEntityIOListener* m_pListener = nullptr;
    std::string m_szClassname;
    std::string m_szOutput;
    bool m_bPost = false;

    /**

    * @brief Removes listener.
      */
    void Unhook();

    /**

    * @brief Destructor auto-unhooks listener.
      */
    ~CEntityIOListenerHandle()
    {
        Unhook();
    }
};

/* =========================
Internal storage
========================= */

/**

* @brief Stores listeners per output.
  */
struct EntityIOCallbackPair
{
    std::vector<IEntityIOListener*> m_vecPre;
    std::vector<IEntityIOListener*> m_vecPost;
};

/**

* @brief Key for output lookup.
  */
struct OutputKey
{
    std::string m_szClassName;
    std::string m_szOutputName;

    bool operator==(const OutputKey& other) const
    {
        return m_szClassName == other.m_szClassName &&
            m_szOutputName == other.m_szOutputName;
    }
};

/**

* @brief Hash function for OutputKey.
  */
struct OutputKeyHash
{
    size_t operator()(const OutputKey& k) const
    {
        return std::hash<std::string>()(k.m_szClassName) ^
            (std::hash<std::string>()(k.m_szOutputName) << 1);
    }
};

#endif // ENTITYIO_H
