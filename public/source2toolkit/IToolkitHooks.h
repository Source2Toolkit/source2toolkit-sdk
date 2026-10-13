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

* @file IToolkitHooks.h
* @brief One-line KHook hooks: declared, resolved, attached and taken down
* from a single place.
*
* A raw KHook hook needs four places: the type in the header, `new` in the
* constructor, the address lookup plus Configure/Add in Init, and
* Remove/delete in Unload. The wrappers here fold all of that into one
* member declared with a macro in the class body:
*
* @code
* class CHooks
* {
* public:
*     KHook::Return<void> OnPostThink(CCSPlayerPawn* pThis);
*     KHook::Return<void> OnGameFrame(ISource2Server* pThis, bool a, bool b, bool c);
*     KHook::Return<int>  OnLoadEventsFromFile(IGameEventManager2* pThis, const char* pszFile, bool bSearchAll);
*     KHook::Return<void> OnRespawn(CCSPlayerController* pThis);
*
*     KHOOK_MEMBER(m_hPostThink, "CCSPlayerPawn::PostThink", &CHooks::OnPostThink, nullptr);
*     KHOOK_VIRTUAL(m_hGameFrame, &ISource2Server::GameFrame, &g_pSource2Server, nullptr, &CHooks::OnGameFrame);
*     KHOOK_VIRTUAL(m_hLoadEventsFromFile, &IGameEventManager2::LoadEventsFromFile, KHOOK_VTABLE("server", "CGameEventManager"), &CHooks::OnLoadEventsFromFile, nullptr);
*     KHOOK_VIRTUAL(m_hRespawn, "CCSPlayerController::Respawn", KHOOK_VTABLE("server", "CCSPlayerController"), &CHooks::OnRespawn, nullptr);
* };
* @endcode
*
* The hook's type is taken from the handler's signature (the first parameter
* is the hooked class, the rest are the function's own arguments), so nothing
* is spelled twice and MSVC needs no CTAD. The handler has to be declared
* above the macro that names it. The context is the enclosing object.
*
* `KHOOK_INIT()` in Load() (after TOOLKIT_SAVEVARS()) resolves and installs
* every hook declared this way; `KHOOK_DESTRUCT()` in Unload() takes them
* all down. A hook whose target is `nullptr` is left to the caller:
* `m_hX.Init(address)`, `m_hX.Init(pInstance)` or `m_hX.InitGlobal(vtable)`.
*
* KHOOK_INIT() only sees the hooks that exist when it runs. A hook that is a
* member of an object created later (`new CFeature()` after Load()) registers
* itself but stays uninstalled until `m_hX.Init()` -- in that object's
* constructor, say -- resolves the target the macro was given.
* `m_hX.Destruct()` takes one hook down for good; destroying the owner does
* the same and unregisters it, so KHOOK_DESTRUCT() never touches a hook that
* is already gone.
*
* Targets:
* * KHOOK_MEMBER / KHOOK_FUNCTION -- a gamedata entry name (resolved through
*   IToolkitGameConfig::ResolveSignature), an IToolkitMemory / void* / typed
*   function pointer (`ADDR_TAKE_DAMAGE_OLD()` -- what the toolkit already
*   resolved, evaluated at KHOOK_INIT()), or `nullptr`.
* * KHOOK_VIRTUAL -- the function is a member function pointer (index read
*   from the vtable), an integer index, or a gamedata offset name; the
*   target is a pointer to the instance pointer (`&g_pSource2Server`, read
*   at Init so it may still be null when the hook is constructed),
*   `KHOOK_VTABLE(module, class)` for a whole vtable by RTTI name -- the
*   module as a name or as `&pModule` (an IToolkitModule* filled in at
*   load) -- or `nullptr`.
*
* `KHOOK_NEW` (IToolkitTypes.h) is still there for a raw KHook object with
* none of this on top.
  */

#ifndef _INCLUDE_ITOOLKIT_HOOKS_H
#define _INCLUDE_ITOOLKIT_HOOKS_H

#pragma once

#ifndef SOURCE2TOOLKIT_CORE

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <type_traits>
#include <vector>

// Deliberately nothing from IToolkitPlugin.h / IToolkitGameConfig.h /
// IToolkitApi.h here: this header lands at the end of IToolkitPlugin.h, which
// most SDK headers include first, so anything from them would still be
// incomplete. Whatever needs the plugin globals is in src/utils/hooks.cpp.
#include "IToolkitTypes.h"
#include "IToolkitMemory.h"
#include "IToolkitModule.h"

// Everything below is per plugin binary: the hook list must never be shared
// with (or interposed by) another plugin's copy of this header.
#if defined(__GNUC__)
#pragma GCC visibility push(hidden)
#endif

/* =========================
Base + registry
========================= */

/**

* @brief A hook the plugin declared through one of the KHOOK_* macros.
*
* Every instance registers itself on construction; KHOOK_INIT() and
* KHOOK_DESTRUCT() walk that list.
  */
class IToolkitHook
{
public:
    virtual ~IToolkitHook()
    {
        std::vector<IToolkitHook*>& vecHooks = List();
        for (size_t i = 0; i < vecHooks.size(); ++i)
        {
            if (vecHooks[i] == this)
            {
                vecHooks.erase(vecHooks.begin() + i);
                break;
            }
        }
    }

    /**

    * @brief Resolves the target given at construction and installs the hook.
    *
    * KHOOK_INIT() calls this on every hook that exists at that point; a hook
    * constructed later is installed by calling it directly. Once installed
    * it does nothing, and after a failure (an instance still null, say) it
    * can simply be called again.
    *
    * @return false when the target could not be resolved. A hook with no
    *         target (nullptr) is left alone and counts as success.
      */
    virtual bool Init() = 0;

    /**

    * @brief Detaches and deletes the underlying KHook object.
    *
    * Deleting is what takes the detour down. Safe to call more than once,
    * and final: the hook cannot be installed again afterwards. The wrapper's
    * destructor calls it too, so destroying the owning object takes its
    * hooks down.
      */
    virtual void Destruct() = 0;

    /**

    * @brief Whether the hook is currently installed.
      */
    virtual bool IsInstalled() const = 0;

    /**

    * @brief Init() on every hook declared in this plugin.
    *
    * @return false when any of them failed; the rest are installed anyway.
      */
    static bool InitAll()
    {
        bool bOk = true;
        for (IToolkitHook* pHook : List())
        {
            if (!pHook->Init())
                bOk = false;
        }
        return bOk;
    }

    /**

    * @brief Destruct() on every hook declared in this plugin, last first.
      */
    static void DestructAll()
    {
        std::vector<IToolkitHook*>& vecHooks = List();
        for (size_t i = vecHooks.size(); i-- > 0;)
            vecHooks[i]->Destruct();
    }

    /// How many hooks this plugin declared -- IToolkitPlugin::GetRawHookCount().
    static int Count()
    {
        return static_cast<int>(List().size());
    }

protected:
    IToolkitHook()
    {
        List().push_back(this);
    }

    IToolkitHook(const IToolkitHook&) = delete;
    IToolkitHook& operator=(const IToolkitHook&) = delete;

    // Function-local so it exists before the first hook (a member of some
    // global object) registers, whatever the translation-unit order.
    static std::vector<IToolkitHook*>& List()
    {
        static std::vector<IToolkitHook*> s_vecHooks;
        return s_vecHooks;
    }

    // These go through IToolkitAPI and IToolkitGameConfig, which are only
    // forward-declared where this header lands (the end of IToolkitPlugin.h),
    // so they are defined in the SDK's src/utils/hooks.cpp.
    static void Warn(const char* pszFormat, const char* pszName);
    // windows.h defines FreeModule as a macro; IToolkitApi.h drops it, but
    // this header can land first (IToolkitMenus.h -> IToolkitTypes.h).
#ifdef FreeModule
#undef FreeModule
#endif
    static void FreeModule(IToolkitModule* pModule);
    static int GetOffset(const char* pszName);
    static IToolkitMemory ResolveSignature(const char* pszName);
};

/**

* @brief The vtable a global virtual hook attaches to: an RTTI class name
* inside a module ("server", "engine2", ...).
  */
struct CToolkitVTableName
{
    CToolkitVTableName() = default;

    /// The module by name -- loaded through IToolkitModule::New at Init.
    CToolkitVTableName(const char* module, const char* name) :
        pszModule(module), pszClass(name)
    {
    }

    /// The module through a pointer to an IToolkitModule* the plugin fills
    /// in at load (read at Init, so it may still be null now).
    CToolkitVTableName(IToolkitModule** module, const char* name) :
        ppModule(module), pszClass(name)
    {
    }

    /// The table of a base class sub-object inside the class -- for an
    /// interface that is not its first base (IToolkitModule::GetVirtualTableByBase).
    CToolkitVTableName(const char* module, const char* name, const char* base) :
        pszModule(module), pszClass(name), pszBase(base)
    {
    }

    CToolkitVTableName(IToolkitModule** module, const char* name, const char* base) :
        ppModule(module), pszClass(name), pszBase(base)
    {
    }

    const char* pszModule = nullptr;
    IToolkitModule** ppModule = nullptr;
    const char* pszClass = nullptr;
    const char* pszBase = nullptr;
};

namespace toolkithook
{
    // What a deferred target evaluates to: an IToolkitMemory, anything that
    // converts to one (void*, uintptr_t), or a typed function pointer such as
    // the ADDR_* getters return.
    inline IToolkitMemory ToMemory(IToolkitMemory address)
    {
        return address;
    }

    template <typename R, typename... A>
    inline IToolkitMemory ToMemory(R (*pfnFunction)(A...))
    {
        return IToolkitMemory(reinterpret_cast<const void*>(pfnFunction));
    }

    // KHOOK_MEMBER / KHOOK_FUNCTION wrap their target in a capture-less lambda
    // so it is evaluated at KHOOK_INIT(), not when the (usually global) owner
    // is constructed -- that is what lets ADDR_TAKE_DAMAGE_OLD() sit in the
    // macro although g_pToolkitAddresses is still null at that point. A target
    // that is itself a lambda is unwrapped the same way.
    // Only class types (lambdas) are unwrapped: a plain function pointer with
    // no arguments is a hook target, not something to call.
    template <typename T>
    inline decltype(auto) Evaluate(T&& value)
    {
        if constexpr (std::is_class_v<std::decay_t<T>> && std::is_invocable_v<T>)
            return Evaluate(value());
        else
            return std::forward<T>(value);
    }

    template <typename RESOLVER>
    using ResolvedType = std::decay_t<decltype(Evaluate(RESOLVER{}()))>;
} // namespace toolkithook

/* =========================
Virtual
========================= */

/**

* @brief A KHook::Virtual plus where it goes.
*
* @tparam CLASS  The hooked class -- the handler's first parameter.
* @tparam RETURN The hooked function's return type.
* @tparam ARGS   The hooked function's own arguments.
  */
/**

* @brief A virtual hook's slot given both ways: the member function pointer
* the interface header compiles to, and the name of a gamedata offset for the
* same function. Gamedata wins at KHOOK_INIT(); a difference between the two
* is reported (one of them is stale), and a missing entry for this platform
* leaves the interface's index in place. KHOOK_GAMEDATA() makes one.
  */
template <typename MFP>
struct CToolkitVirtualIndex
{
    MFP mfp;
    const char* pszName;
};

template <typename MFP>
constexpr CToolkitVirtualIndex<MFP> ToolkitVirtualIndex(MFP mfp, const char* pszName)
{
    return { mfp, pszName };
}

template <typename CLASS, typename RETURN, typename... ARGS>
class CToolkitVirtualHook : public IToolkitHook
{
public:
    using HookType = ToolkitKHook::Virtual<CLASS, RETURN, ARGS...>;

    /**

    * @param function A member function pointer, a vtable index, or the name
    *                 of a gamedata offset.
    * @param target   `&pInstance` (read at Init), KHOOK_VTABLE(module, class),
    *                 or nullptr.
    * @param pContext The object the callbacks are members of.
    * @param pre      Callback before the original, or nullptr.
    * @param post     Callback after the original, or nullptr.
      */
    template <typename FUNCTION, typename TARGET, typename CONTEXT, typename PRE, typename POST>
    CToolkitVirtualHook(FUNCTION function, TARGET target, CONTEXT* pContext, PRE pre, POST post)
    {
        Create(function, pContext, pre, post);
        SetTarget(target);
    }

    ~CToolkitVirtualHook() override
    {
        Destruct();
    }

    bool Init() override
    {
        if (!m_pHook)
            return false;

        if (m_bInstalled)
            return true;

        if (m_pfnInstance)
        {
            CLASS* pInstance = m_pfnInstance(m_pInstanceSource);
            if (!pInstance)
            {
                Warn("KHook: the instance to hook is still null at init; hook not installed (%s)\n", m_pszIndexName ? m_pszIndexName : "virtual hook");
                return false;
            }

            return Init(pInstance);
        }

        if (m_VTable.pszClass)
        {
            if (m_VTable.ppModule)
                return InitGlobal(*m_VTable.ppModule, m_VTable.pszClass, m_VTable.pszBase);

            return InitGlobal(m_VTable.pszModule, m_VTable.pszClass, m_VTable.pszBase);
        }

        return true;
    }

    /**

    * @brief Installs the hook on one instance.
      */
    bool Init(CLASS* pInstance)
    {
        if (!m_pHook || !pInstance || !ResolveIndex())
            return false;

        m_pHook->Add(pInstance);
        m_pInstance = pInstance;
        m_bInstalled = true;
        return true;
    }

    /**

    * @brief Installs the hook on a whole vtable -- every object sharing it.
      */
    bool InitGlobal(IToolkitMemory vtable)
    {
        if (!m_pHook || !vtable || !ResolveIndex())
            return false;

        // AddGlobal only ever reads the first pointer of what it is handed,
        // so a plain pointer holding the vtable is a valid stand-in.
        m_pVTable = vtable.GetPtr();
        m_pHook->AddGlobal(reinterpret_cast<CLASS*>(&m_pVTable));
        m_bInstalled = true;
        return true;
    }

    /**

    * @brief Installs the hook on the vtable of an RTTI class name in a module, or of its base pszBase.
      */
    bool InitGlobal(IToolkitModule* pModule, const char* pszClass, const char* pszBase = nullptr)
    {
        if (!pModule)
        {
            Warn("KHook: no module to look '%s' up in; hook not installed\n", pszClass);
            return false;
        }

        const IToolkitMemory vtable = pszBase ? pModule->GetVirtualTableByBase(pszClass, pszBase) : pModule->GetVirtualTableByName(pszClass);
        if (!vtable)
        {
            if (pszBase)
                Warn("KHook: vtable of base '%s' was not found; hook not installed\n", pszBase);
            else
                Warn("KHook: vtable '%s' was not found; hook not installed\n", pszClass);
            return false;
        }

        return InitGlobal(vtable);
    }

    /**

    * @brief Same, with the module loaded by name for the lookup.
      */
    bool InitGlobal(const char* pszModule, const char* pszClass, const char* pszBase = nullptr)
    {
        IToolkitModule* pModule = IToolkitModule::New(pszModule);
        if (!pModule)
        {
            Warn("KHook: module '%s' is not loaded; hook not installed\n", pszModule);
            return false;
        }

        const bool bInstalled = InitGlobal(pModule, pszClass, pszBase);
        FreeModule(pModule);
        return bInstalled;
    }

    void Destruct() override
    {
        if (!m_pHook)
            return;

        if (m_pInstance)
            m_pHook->Remove(m_pInstance);

        if (m_pVTable)
            m_pHook->RemoveGlobal(reinterpret_cast<CLASS*>(&m_pVTable));

        delete m_pHook;
        m_pHook = nullptr;
        m_pInstance = nullptr;
        m_pVTable = nullptr;
        m_bIndexResolved = false;
        m_bInstalled = false;
    }

    bool IsInstalled() const override
    {
        return m_bInstalled;
    }

    /// The KHook object itself, for anything the wrapper does not cover.
    HookType* Get() const
    {
        return m_pHook;
    }

    HookType* operator->() const
    {
        return m_pHook;
    }

    /// Runs the original on pThis, bypassing the hook chain.
    RETURN CallOriginal(CLASS* pThis, ARGS... args)
    {
        return m_pHook->CallOriginal(pThis, args...);
    }

private:
    // A gamedata offset is read once, on the first install.
    bool ResolveIndex()
    {
        if (!m_pszIndexName || m_bIndexResolved)
            return true;

        // -1 must never reach the vtable: one slot before it is the typeinfo
        // pointer, and a hook there breaks every later by-name vtable lookup
        // on that class.
        const int index = GetOffset(m_pszIndexName);
        if (index < 0)
        {
            if (m_bIndexFromInterface)
            {
                // No entry for this platform: the index the interface header
                // compiled in stands, as it would have without the name.
                char msg[256];
                snprintf(msg, sizeof(msg), "KHook: gamedata has no offset '%s' for this platform; using the interface's index %d\n", m_pszIndexName, static_cast<int>(m_pHook->GetIndex()));
                Warn("%s", msg);
                m_bIndexResolved = true;
                return true;
            }

            Warn("KHook: offset '%s' is missing from gamedata; hook not installed\n", m_pszIndexName);
            return false;
        }

        // Gamedata is the one that can be fixed without a rebuild, so it wins;
        // a difference means the header or the entry is stale, and that is
        // worth a line either way.
        if (m_bIndexFromInterface && m_pHook->GetIndex() != index)
        {
            char msg[256];
            snprintf(msg, sizeof(msg), "KHook: gamedata offset '%s' is %d but the interface header compiled in %d; using gamedata\n", m_pszIndexName, index, static_cast<int>(m_pHook->GetIndex()));
            Warn("%s", msg);
        }

        m_pHook->Configure(index);
        m_bIndexResolved = true;
        return true;
    }

    template <typename MFP, typename CONTEXT, typename PRE, typename POST>
    void Create(CToolkitVirtualIndex<MFP> index, CONTEXT* pContext, PRE pre, POST post)
    {
        // The interface's index now, gamedata's at Init(); see ResolveIndex().
        Create(index.mfp, pContext, pre, post);
        m_pszIndexName = index.pszName;
        m_bIndexFromInterface = true;
    }

    template <typename CONTEXT, typename PRE, typename POST>
    void Create(RETURN (CLASS::*function)(ARGS...), CONTEXT* pContext, PRE pre, POST post)
    {
        m_pHook = new HookType(function, pContext, pre, post);
    }

    template <typename CONTEXT, typename PRE, typename POST>
    void Create(RETURN (CLASS::*function)(ARGS...) const, CONTEXT* pContext, PRE pre, POST post)
    {
        m_pHook = new HookType(function, pContext, pre, post);
    }

    template <typename INDEX, typename CONTEXT, typename PRE, typename POST,
              std::enable_if_t<std::is_integral_v<INDEX>, int> = 0>
    void Create(INDEX index, CONTEXT* pContext, PRE pre, POST post)
    {
        m_pHook = new HookType(static_cast<std::uint32_t>(index), pContext, pre, post);
    }

    template <typename CONTEXT, typename PRE, typename POST>
    void Create(const char* pszIndexName, CONTEXT* pContext, PRE pre, POST post)
    {
        // Placeholder index; Init() reads the real one from gamedata.
        m_pszIndexName = pszIndexName;
        m_pHook = new HookType(0u, pContext, pre, post);
    }

    // The instance is read at Init, not now: engine interfaces are only
    // filled in by Load(), long after a global object's constructor ran.
    // T may be a class derived from CLASS (ISource2Server for IServerGameDLL).
    template <typename T>
    void SetTarget(T** ppInstance)
    {
        m_pInstanceSource = ppInstance;
        m_pfnInstance = [](void* pSource) -> CLASS*
        {
            return static_cast<CLASS*>(*static_cast<T**>(pSource));
        };
    }

    void SetTarget(CToolkitVTableName vtable)
    {
        m_VTable = vtable;
    }

    void SetTarget(std::nullptr_t)
    {
    }

    HookType* m_pHook = nullptr;
    const char* m_pszIndexName = nullptr;
    // KHOOK_GAMEDATA(): the name is a check on the interface's index, not the
    // only source of it.
    bool m_bIndexFromInterface = false;
    void* m_pInstanceSource = nullptr;
    CLASS* (*m_pfnInstance)(void*) = nullptr;
    CToolkitVTableName m_VTable;
    CLASS* m_pInstance = nullptr;
    void* m_pVTable = nullptr;
    bool m_bIndexResolved = false;
    bool m_bInstalled = false;
};

/* =========================
Member
========================= */

/**

* @brief A KHook::Member (a function with a `this`, found by address) plus
* where its address comes from.
  */
template <typename CLASS, typename RETURN, typename... ARGS>
class CToolkitMemberHook : public IToolkitHook
{
public:
    using HookType = KHook::Member<CLASS, RETURN, ARGS...>;

    /**

    * @param target   A gamedata entry name, an IToolkitMemory address (or a
    *                 typed function pointer), nullptr, or a capture-less lambda
    *                 returning one of those -- the macro always passes a lambda
    *                 so the target is evaluated at KHOOK_INIT().
    * @param pContext The object the callbacks are members of.
    * @param pre      Callback before the original, or nullptr.
    * @param post     Callback after the original, or nullptr.
      */
    template <typename TARGET, typename CONTEXT, typename PRE, typename POST>
    CToolkitMemberHook(TARGET target, CONTEXT* pContext, PRE pre, POST post)
    {
        m_pHook = new HookType(pContext, pre, post);
        SetTarget(target);
    }

    ~CToolkitMemberHook() override
    {
        Destruct();
    }

    bool Init() override
    {
        if (!m_pHook)
            return false;

        if (m_bInstalled)
            return true;

        if (m_pfnName)
            m_pszName = m_pfnName();

        if (m_Address)
            return Init(m_Address);

        if (m_pfnResolve)
        {
            const IToolkitMemory address = m_pfnResolve();
            if (!address)
            {
                Warn("KHook: %s; hook not installed\n", "the address given for a hook is null at init");
                return false;
            }

            return Init(address);
        }

        if (m_pszName)
        {
            const IToolkitMemory address = ResolveSignature(m_pszName);
            if (!address)
            {
                Warn("KHook: '%s' was not resolved; hook not installed\n", m_pszName);
                return false;
            }

            return Init(address);
        }

        return true;
    }

    /**

    * @brief Places the detour at an address.
      */
    bool Init(IToolkitMemory address)
    {
        if (!m_pHook || !address)
            return false;

        m_pHook->Configure(address.RCast<const void*>());
        m_bInstalled = true;
        return true;
    }

    /// Same, from a typed function pointer (an ADDR_* getter, say).
    template <typename R, typename... A>
    bool Init(R (*pfnFunction)(A...))
    {
        return Init(IToolkitMemory(reinterpret_cast<const void*>(pfnFunction)));
    }

    void Destruct() override
    {
        delete m_pHook;
        m_pHook = nullptr;
        m_bInstalled = false;
    }

    bool IsInstalled() const override
    {
        return m_bInstalled;
    }

    HookType* Get() const
    {
        return m_pHook;
    }

    HookType* operator->() const
    {
        return m_pHook;
    }

    RETURN CallOriginal(CLASS* pThis, ARGS... args)
    {
        return m_pHook->CallOriginal(pThis, args...);
    }

private:
    void SetTarget(const char* pszName)
    {
        m_pszName = pszName;
    }

    void SetTarget(IToolkitMemory address)
    {
        m_Address = address;
    }

    // A deferred target: called at Init through a plain function pointer,
    // hence capture-less. What it evaluates to picks the path above.
    template <typename RESOLVER, std::enable_if_t<std::is_invocable_v<RESOLVER>, int> = 0>
    void SetTarget(RESOLVER)
    {
        static_assert(std::is_default_constructible_v<RESOLVER>, "a hook's target must capture nothing");
        using RESOLVED = toolkithook::ResolvedType<RESOLVER>;

        if constexpr (std::is_same_v<RESOLVED, std::nullptr_t>)
        {
        }
        else if constexpr (std::is_convertible_v<RESOLVED, const char*>)
        {
            m_pfnName = []() -> const char*
            {
                return toolkithook::Evaluate(RESOLVER{}());
            };
        }
        else
        {
            m_pfnResolve = []() -> IToolkitMemory
            {
                return toolkithook::ToMemory(toolkithook::Evaluate(RESOLVER{}()));
            };
        }
    }

    void SetTarget(std::nullptr_t)
    {
    }

    HookType* m_pHook = nullptr;
    const char* m_pszName = nullptr;
    const char* (*m_pfnName)() = nullptr;
    IToolkitMemory m_Address;
    IToolkitMemory (*m_pfnResolve)() = nullptr;
    bool m_bInstalled = false;
};

/* =========================
Function
========================= */

/**

* @brief A KHook::Function (a free function, found by address) plus where
* its address comes from.
  */
template <typename RETURN, typename... ARGS>
class CToolkitFunctionHook : public IToolkitHook
{
public:
    using HookType = KHook::Function<RETURN, ARGS...>;

    template <typename TARGET, typename CONTEXT, typename PRE, typename POST>
    CToolkitFunctionHook(TARGET target, CONTEXT* pContext, PRE pre, POST post)
    {
        m_pHook = new HookType(pContext, pre, post);
        SetTarget(target);
    }

    ~CToolkitFunctionHook() override
    {
        Destruct();
    }

    bool Init() override
    {
        if (!m_pHook)
            return false;

        if (m_bInstalled)
            return true;

        if (m_pfnName)
            m_pszName = m_pfnName();

        if (m_Address)
            return Init(m_Address);

        if (m_pfnResolve)
        {
            const IToolkitMemory address = m_pfnResolve();
            if (!address)
            {
                Warn("KHook: %s; hook not installed\n", "the address given for a hook is null at init");
                return false;
            }

            return Init(address);
        }

        if (m_pszName)
        {
            const IToolkitMemory address = ResolveSignature(m_pszName);
            if (!address)
            {
                Warn("KHook: '%s' was not resolved; hook not installed\n", m_pszName);
                return false;
            }

            return Init(address);
        }

        return true;
    }

    bool Init(IToolkitMemory address)
    {
        if (!m_pHook || !address)
            return false;

        m_pHook->Configure(address.RCast<const void*>());
        m_bInstalled = true;
        return true;
    }

    template <typename R, typename... A>
    bool Init(R (*pfnFunction)(A...))
    {
        return Init(IToolkitMemory(reinterpret_cast<const void*>(pfnFunction)));
    }

    void Destruct() override
    {
        delete m_pHook;
        m_pHook = nullptr;
        m_bInstalled = false;
    }

    bool IsInstalled() const override
    {
        return m_bInstalled;
    }

    HookType* Get() const
    {
        return m_pHook;
    }

    HookType* operator->() const
    {
        return m_pHook;
    }

    RETURN CallOriginal(ARGS... args)
    {
        return m_pHook->CallOriginal(args...);
    }

private:
    void SetTarget(const char* pszName)
    {
        m_pszName = pszName;
    }

    void SetTarget(IToolkitMemory address)
    {
        m_Address = address;
    }

    template <typename RESOLVER, std::enable_if_t<std::is_invocable_v<RESOLVER>, int> = 0>
    void SetTarget(RESOLVER)
    {
        static_assert(std::is_default_constructible_v<RESOLVER>, "a hook's target must capture nothing");
        using RESOLVED = toolkithook::ResolvedType<RESOLVER>;

        if constexpr (std::is_same_v<RESOLVED, std::nullptr_t>)
        {
        }
        else if constexpr (std::is_convertible_v<RESOLVED, const char*>)
        {
            m_pfnName = []() -> const char*
            {
                return toolkithook::Evaluate(RESOLVER{}());
            };
        }
        else
        {
            m_pfnResolve = []() -> IToolkitMemory
            {
                return toolkithook::ToMemory(toolkithook::Evaluate(RESOLVER{}()));
            };
        }
    }

    void SetTarget(std::nullptr_t)
    {
    }

    HookType* m_pHook = nullptr;
    const char* m_pszName = nullptr;
    const char* (*m_pfnName)() = nullptr;
    IToolkitMemory m_Address;
    IToolkitMemory (*m_pfnResolve)() = nullptr;
    bool m_bInstalled = false;
};

/* =========================
Type deduction for the macros
========================= */

namespace toolkithook
{
    // Whichever of pre/post is not nullptr carries the signature.
    template <typename PRE, typename POST>
    struct Callback
    {
        using type = PRE;
    };

    template <typename POST>
    struct Callback<std::nullptr_t, POST>
    {
        using type = POST;
    };

    // Handler with the hooked object first: Virtual and Member hooks. (The
    // parameter is not named CALLBACK: windef.h defines that as a macro, and
    // this header can land after windows.h.)
    template <typename HANDLER>
    struct MemberTraits;

    template <typename CONTEXT, typename RETURN, typename CLASS, typename... ARGS>
    struct MemberTraits<KHook::Return<RETURN> (CONTEXT::*)(CLASS*, ARGS...)>
    {
        using Virtual = CToolkitVirtualHook<std::remove_const_t<CLASS>, RETURN, ARGS...>;
        using Member = CToolkitMemberHook<std::remove_const_t<CLASS>, RETURN, ARGS...>;
    };

    // Handler with no hooked object: Function hooks.
    template <typename HANDLER>
    struct FunctionTraits;

    template <typename CONTEXT, typename RETURN, typename... ARGS>
    struct FunctionTraits<KHook::Return<RETURN> (CONTEXT::*)(ARGS...)>
    {
        using Function = CToolkitFunctionHook<RETURN, ARGS...>;
    };

    template <typename PRE, typename POST>
    using VirtualHookFor = typename MemberTraits<typename Callback<PRE, POST>::type>::Virtual;

    template <typename PRE, typename POST>
    using MemberHookFor = typename MemberTraits<typename Callback<PRE, POST>::type>::Member;

    template <typename PRE, typename POST>
    using FunctionHookFor = typename FunctionTraits<typename Callback<PRE, POST>::type>::Function;
} // namespace toolkithook

#if defined(__GNUC__)
#pragma GCC visibility pop
#endif

/* =========================
Macros
========================= */

/**

* @brief A whole vtable as a virtual hook's target: an RTTI class name in a
* module -- the module's name ("server", "engine2", "tier0", ...) or a
* pointer to an IToolkitModule* the plugin fills in at load (`&modules::server`).
  */
#define KHOOK_VTABLE(module, name) ::CToolkitVTableName{ module, name }

/**

* @brief The vtable of a base class sub-object inside an RTTI class: for an
* interface that is not the class's first base, whose table KHOOK_VTABLE cannot
* reach. The base's position comes from the RTTI. Needs a core with
* IToolkitModule::GetVirtualTableByBase.
  */
#define KHOOK_VTABLE_BASE(module, name, base) ::CToolkitVTableName{ module, name, base }

/**

* @brief Declares a virtual hook as a class member.
*
* @param member   Member name.
* @param function `&Class::Method`, a vtable index, a gamedata offset name,
*                 or KHOOK_GAMEDATA(&Class::Method, "gamedata name") -- both,
*                 gamedata checked against the header and taking precedence.
* @param target   `&pInstance`, KHOOK_VTABLE(module, class), KHOOK_VTABLE_BASE(module, class, base), or nullptr.
* @param pre      `&Self::Handler` or nullptr.
* @param post     `&Self::Handler` or nullptr.
*
* Which form to pick: a gamedata name (alone or with KHOOK_GAMEDATA) follows
* the core's gamedata, so an engine update that moves the slot is fixed by
* updating the core; a bare `&Class::Method` compiles the slot in from the
* SDK's header and needs a rebuild when it moves.
*
* @code
* KHOOK_VIRTUAL(m_hGameFrame, &ISource2Server::GameFrame, &g_pSource2Server, nullptr, &Plugin::Hook_GameFrame);
* KHOOK_VIRTUAL(m_hGameFrame, KHOOK_GAMEDATA(&ISource2Server::GameFrame, "ISource2Server::GameFrame"), &g_pSource2Server, nullptr, &Plugin::Hook_GameFrame);
* @endcode
  */
#define KHOOK_VIRTUAL(member, function, target, pre, post) \
    ::toolkithook::VirtualHookFor<decltype(pre), decltype(post)> member { function, target, this, pre, post }

/**

* @brief A virtual hook's slot from the interface header and from gamedata
* at once, for KHOOK_VIRTUAL's `function`. See there.
  */
#define KHOOK_GAMEDATA(function, name) ::ToolkitVirtualIndex(function, name)

/**

* @brief Declares a hook on a member function found by address.
*
* The target is evaluated at KHOOK_INIT(), not here, so an expression that
* needs the toolkit -- ADDR_TAKE_DAMAGE_OLD(), a GAMECONFIG_RESOLVE() -- is
* fine even though the owner is usually a global constructed long before Load().
*
* @param member Member name.
* @param target A gamedata entry name, an IToolkitMemory / void* / typed
*               function pointer (an ADDR_* getter), or nullptr for a manual
*               Init(address) later.
* @param pre    `&Self::Handler` or nullptr.
* @param post   `&Self::Handler` or nullptr.
*
* @code
* KHOOK_MEMBER(m_hPostThink, "CCSPlayerPawn::PostThink", &Plugin::Hook_PostThink, nullptr);
* KHOOK_MEMBER(m_hTakeDamageOld, ADDR_TAKE_DAMAGE_OLD(), &Plugin::Hook_TakeDamageOld, nullptr);
* @endcode
  */
#define KHOOK_MEMBER(member, target, pre, post) \
    ::toolkithook::MemberHookFor<decltype(pre), decltype(post)> member { [] { return (target); }, this, pre, post }

/**

* @brief Declares a hook on a free function found by address. Same
* parameters as KHOOK_MEMBER; the handler takes no hooked object.
  */
#define KHOOK_FUNCTION(member, target, pre, post) \
    ::toolkithook::FunctionHookFor<decltype(pre), decltype(post)> member { [] { return (target); }, this, pre, post }

/**

* @brief Installs every hook declared with the macros above. Call in Load(),
* after TOOLKIT_SAVEVARS() and once the instance pointers hooks read are set.
*
* This is also where the KHook check happens: a plugin that hooks through
* KHook shares the detour engine with the core and must have been built
* against the same khook.hpp (IKHook and the template layout; a KHook update
* confined to its src/ does not count), so a mismatch returns
* false out of Load() with the reason in `error`. A plugin that never calls
* this is not checked -- it does not hook, so it does not care. A plugin
* using raw KHook objects (KHOOK_NEW) without any KHOOK_* macro must call
* KHOOK_CHECK() itself before the first one.
*
* Each hook that could not be resolved is logged and skipped.
  */
#define KHOOK_CHECK() \
    do { \
        const char* toolkitKHookNote = nullptr; \
        if (!::ToolkitKHookVersionMatches(g_ToolkitCoreKHookHeader, g_ToolkitCoreKHookCommit, error, maxlen, &toolkitKHookNote)) \
            return false; \
        if (toolkitKHookNote) \
            g_ToolkitAPI->Log(g_PluginAPI, "%s\n", toolkitKHookNote); \
    } while (0)

#define KHOOK_INIT() \
    do { \
        KHOOK_CHECK(); \
        ::IToolkitHook::InitAll(); \
    } while (0)

/// See IToolkitPlugin::GetRawHookCount().
inline int ToolkitRawHookCount()
{
    return ::IToolkitHook::Count();
}

/**

* @brief Takes every hook down. Call in Unload().
  */
#define KHOOK_DESTRUCT() ::IToolkitHook::DestructAll()

#endif // SOURCE2TOOLKIT_CORE

#endif //_INCLUDE_ITOOLKIT_HOOKS_H
