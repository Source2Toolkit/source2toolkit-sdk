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

* @file IToolkitConVars.h
* @brief Interface for managing and interacting with console variables (ConVars).
*
* Provides functionality for:
* * Querying ConVars by name
* * Reading and writing values
* * Accessing metadata (type, flags, help text)
* * Creating and deleting ConVars
*
* @note ConVars are identified by an internal access index.
  */

#ifndef _INCLUDE_ITOOLKIT_CONVARS_H
#define _INCLUDE_ITOOLKIT_CONVARS_H

#pragma once
#include "IToolkitPlugin.h"

#include <convar.h>

#include <string>
#include <type_traits>

#include "convar.h"
#include "eiface.h"

/* =========================
Forward declarations
========================= */

class ConVarRefAbstract;

/* =========================
Core Toolkit ConVars
========================= */

/**

* @brief Interface for interacting with engine ConVars.
  */
/**
 * @brief Callback type for ConVar value changes.
 *
 * @param ref The ConVar that changed
 * @param slot Split-screen slot the change applies to
 * @param pszNewValue Value being set
 * @param pszOldValue Value being replaced
 */
using ConVarChangeHandler = ToolkitCallback<void(ConVarRefAbstract* ref, CSplitScreenSlot slot,
                                                  const char* pszNewValue, const char* pszOldValue)>;

#define TOOLKIT_CONVARS_INTERFACE "IToolkitConVars002"

/// Access index meaning "no such ConVar" -- the engine's own invalid index.
/// 0 is a real ConVar, so test against this, never against zero.
inline constexpr uint16 TOOLKIT_INVALID_CONVAR_INDEX = 0xFFFF;

class IToolkitConVars
{
public:
    virtual ~IToolkitConVars() = default;

    /**

    * @brief Retrieves access index of a ConVar by name.
    *
    * @param name ConVar name
    * @return Access index, or TOOLKIT_INVALID_CONVAR_INDEX if no
    *         ConVar has that name. 0 is a valid index, so compare against the
    *         sentinel rather than testing for zero.
      */
    virtual uint16 GetConvarAccessIndexByName(const char* name) = 0;

    /**

    * @brief Gets a ConVar reference wrapper.
      */
    virtual ConVarRefAbstract GetConvarRef(uint16 accessIndex) = 0;

    /**

    * @brief Gets ConVar name.
      */
    virtual const char* GetName(uint16 accessIndex) = 0;

    /**

    * @brief Gets help/description text.
      */
    virtual const char* GetHelpText(uint16 accessIndex) = 0;

    /**

    * @brief Gets ConVar type.
      */
    virtual EConVarType GetType(uint16 accessIndex) = 0;

    /**

    * @brief Gets flags (FCVAR_*).
      */
    virtual uint64 GetFlags(uint16 accessIndex) = 0;

    /**

    * @brief Sets flags (FCVAR_*).
      */
    virtual void SetFlags(uint16 accessIndex, uint64 flags) = 0;

    /**

    * @brief Gets pointer to underlying value.
    *
    * @note Type depends on ConVar type.
      */
    virtual void* GetValueAddress(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;

    /* =========================
    Typed getters
    ========================= */

    virtual const char* GetString(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;
    virtual bool GetBool(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;
    virtual int32 GetInt(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;
    virtual float GetFloat(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;
    virtual double GetDouble(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;

    /* =========================
    Typed setters
    ========================= */

    virtual void SetString(uint16 accessIndex, const char* value, CSplitScreenSlot slot = -1) = 0;
    virtual void SetBool(uint16 accessIndex, bool value, CSplitScreenSlot slot = -1) = 0;
    virtual void SetInt(uint16 accessIndex, int32 value, CSplitScreenSlot slot = -1) = 0;
    virtual void SetFloat(uint16 accessIndex, float value, CSplitScreenSlot slot = -1) = 0;
    virtual void SetDouble(uint16 accessIndex, double value, CSplitScreenSlot slot = -1) = 0;

    /* =========================
    Complex types
    ========================= */

    virtual Vector2D GetVector2(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;
    virtual Vector GetVector3(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;
    virtual Vector4D GetVector4(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;
    virtual QAngle GetQAngle(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;
    virtual Color GetColor(uint16 accessIndex, CSplitScreenSlot slot = -1) = 0;

    virtual void SetVector2(uint16 accessIndex, const Vector2D& value, CSplitScreenSlot slot = -1) = 0;
    virtual void SetVector3(uint16 accessIndex, const Vector& value, CSplitScreenSlot slot = -1) = 0;
    virtual void SetVector4(uint16 accessIndex, const Vector4D& value, CSplitScreenSlot slot = -1) = 0;
    virtual void SetQAngle(uint16 accessIndex, const QAngle& value, CSplitScreenSlot slot = -1) = 0;
    virtual void SetColor(uint16 accessIndex, const Color& value, CSplitScreenSlot slot = -1) = 0;

    /* =========================
    Generic access
    ========================= */

    /**

    * @brief Gets value into user-provided buffer.
      */
    virtual void GetValue(uint16 accessIndex, void* outValue, CSplitScreenSlot slot = -1) = 0;

    /* =========================
    Change callbacks
    ========================= */

    /**
     * @brief Registers a listener for every ConVar value change.
     *
     * The engine only takes plain function pointers here, and only keeps one
     * list of them for the whole process. The toolkit installs a single one of
     * those and fans out to the handlers registered through this, so a plugin
     * can use a capturing lambda and does not have to unregister on unload.
     *
     * A plugin may register as many as it likes.
     *
     * Whose it is, the core reads off the handler (see ToolkitCallback); what
     * a plugin still holds at unload the core drops for it.
     *
     * @param handler A function, an object and a method, or a lambda
     * @return The id UnhookConVarChange(id) takes
     */
    virtual ToolkitHookId HookConVarChange(ConVarChangeHandler handler) = 0;

    /**
     * @brief Drops the change listener with this handler -- a function or an
     * object and a method; a lambda goes by its id.
     *
     * @return true when one was found
     */
    virtual bool UnhookConVarChange(const ConVarChangeHandler& handler) = 0;

    /**
     * @brief Drops the change listener HookConVarChange() returned the id for.
     *
     * @return true when it was still there
     */
    virtual bool UnhookConVarChange(ToolkitHookId id) = 0;

    /**

    * @brief Sets value from user-provided buffer.
      */
    virtual void SetValue(uint16 accessIndex, const void* value, CSplitScreenSlot slot = -1) = 0;

    /* =========================
    Creation / destruction
    ========================= */

    /**

    * @brief Creates a new ConVar.
    *
    * @param name ConVar name
    * @param type Variable type
    * @param help Description text
    * @param flags FCVAR flags
    * @param hasMin Whether min value is enforced
    * @param hasMax Whether max value is enforced
    * @param defaultValue Default value pointer (const char* for EConVarType_String)
    * @param minValue Minimum value pointer; only read when hasMin is true, may be nullptr otherwise
    * @param maxValue Maximum value pointer; only read when hasMax is true, may be nullptr otherwise
    *
    * @return Access index of the created ConVar. If a ConVar with that name
    *         already exists, its index is returned and the arguments are
    *         ignored. TOOLKIT_INVALID_CONVAR_INDEX on bad arguments or an
    *         unsupported type.
    *
    * @note The engine has no way to unregister a ConVar, so one created here
    *       outlives the plugin that created it. When the plugin loads again,
    *       the same call returns the existing ConVar with its current value.
      */
    virtual uint16 CreateConVar(const char* name, EConVarType type, const char* help, uint64 flags, bool hasMin,
                                bool hasMax, void* defaultValue, const void* minValue, const void* maxValue) = 0;

    /**

    * @brief Deletes a ConVar.
      */
    virtual void DeleteConVar(uint16 accessIndex) = 0;
};

#define CVAR_IDX(name)                  g_pToolkitConVars->GetConvarAccessIndexByName(name)
#define CVAR_REF(idx)                   g_pToolkitConVars->GetConvarRef(idx)
#define CVAR_GET_STR(idx, ...)          g_pToolkitConVars->GetString(idx, ##__VA_ARGS__)
#define CVAR_GET_BOOL(idx, ...)         g_pToolkitConVars->GetBool(idx, ##__VA_ARGS__)
#define CVAR_GET_INT(idx, ...)          g_pToolkitConVars->GetInt(idx, ##__VA_ARGS__)
#define CVAR_GET_FLOAT(idx, ...)        g_pToolkitConVars->GetFloat(idx, ##__VA_ARGS__)
#define CVAR_GET_DOUBLE(idx, ...)       g_pToolkitConVars->GetDouble(idx, ##__VA_ARGS__)
#define CVAR_SET_STR(idx, v, ...)       g_pToolkitConVars->SetString(idx, v, ##__VA_ARGS__)
#define CVAR_SET_BOOL(idx, v, ...)      g_pToolkitConVars->SetBool(idx, v, ##__VA_ARGS__)
#define CVAR_SET_INT(idx, v, ...)       g_pToolkitConVars->SetInt(idx, v, ##__VA_ARGS__)
#define CVAR_SET_FLOAT(idx, v, ...)     g_pToolkitConVars->SetFloat(idx, v, ##__VA_ARGS__)
#define CVAR_SET_DOUBLE(idx, v, ...)    g_pToolkitConVars->SetDouble(idx, v, ##__VA_ARGS__)
#define CVAR_CREATE(name, type, help, flags, hasMin, hasMax, def, minVal, maxVal) \
    g_pToolkitConVars->CreateConVar(name, type, help, flags, hasMin, hasMax, def, minVal, maxVal)
#define CVAR_DELETE(idx)                g_pToolkitConVars->DeleteConVar(idx)

/**
 * @brief Macro for listening to ConVar value changes.
 *
 * @param passfunc Callback function.
 */
#define HOOK_CONVAR_CHANGE(passfunc) \
    g_pToolkitConVars->HookConVarChange(passfunc)

/**
 * @brief Macro for dropping a ConVar change hook (a function or an object
 * and a method; a lambda goes by the id HOOK_CONVAR_CHANGE returned).
 */
#define UNHOOK_CONVAR_CHANGE(passfunc) \
    g_pToolkitConVars->UnhookConVarChange(passfunc)


/* =========================
Name-addressed helpers

The interface above is index-addressed, which is the right shape when a plugin
holds on to a ConVar. These are for the one-off case -- flip a cvar by name and
move on -- and do the lookup themselves. They go through s2sdk's ConVarRef
directly rather than the interface, so they work before the toolkit's ConVar
manager is up.
========================= */

/**
 * @brief Clears flags on a ConVar found by name, e.g. FCVAR_CHEAT.
 *
 * @return false when there is no such ConVar, or its data is not available.
 */
inline bool UTIL_RemoveConVarFlags(const char* pszName, uint64 nFlags)
{
    if (!pszName) return false;

    ConVarRef ref(pszName);
    if (!ref.IsValidRef()) return false;

    ConVarRefAbstract var(ref);
    if (!var.IsConVarDataAvailable()) return false;

    if (!var.IsFlagSet(nFlags)) return true;

    var.RemoveFlags(nFlags);
    return true;
}

/**
 * @brief Sets flags on a ConVar found by name.
 */
inline bool UTIL_AddConVarFlags(const char* pszName, uint64 nFlags)
{
    if (!pszName) return false;

    ConVarRef ref(pszName);
    if (!ref.IsValidRef()) return false;

    ConVarRefAbstract var(ref);
    if (!var.IsConVarDataAvailable()) return false;

    var.AddFlags(nFlags);
    return true;
}

/**
 * @brief Sets a ConVar by name.
 *
 * The type is what decides how the value is written, so call it as
 * UTIL_SetConVar<int>("mp_freezetime", 5) when the literal would deduce to
 * something else.
 *
 * @return false when there is no such ConVar, or its data is not available.
 */
template <typename T>
inline bool UTIL_SetConVar(const char* pszName, const T& value, CSplitScreenSlot slot = -1)
{
    if (!pszName) return false;

    ConVarRef ref(pszName);
    if (!ref.IsValidRef()) return false;

    ConVarRefAbstract var(ref, TranslateConVarType<T>());
    if (!var.IsConVarDataAvailable()) return false;

    if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>)
        return var.SetString(CUtlString{value ? value : ""}, slot);
    else if constexpr (std::is_same_v<T, std::string>)
        return var.SetString(CUtlString{value.c_str()}, slot);
    else
    {
        var.SetAs<T>(value, slot);
        return true;
    }
}

inline bool UTIL_SetConVar(const char* pszName, double v, CSplitScreenSlot slot = -1) { return UTIL_SetConVar<float64>(pszName, static_cast<float64>(v), slot); }
inline bool UTIL_SetConVar(const char* pszName, float v, CSplitScreenSlot slot = -1)  { return UTIL_SetConVar<float32>(pszName, static_cast<float32>(v), slot); }
inline bool UTIL_SetConVar(const char* pszName, int v, CSplitScreenSlot slot = -1)    { return UTIL_SetConVar<int32>(pszName, static_cast<int32>(v), slot); }
inline bool UTIL_SetConVar(const char* pszName, unsigned v, CSplitScreenSlot slot = -1) { return UTIL_SetConVar<uint32>(pszName, static_cast<uint32>(v), slot); }

#endif //_INCLUDE_ITOOLKIT_CONVARS_H
