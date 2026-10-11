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

* @file IToolkitTypes.h
* @brief Types shared by every toolkit callback.
*
* Defines:
* * Action    -- what a callback tells the toolkit to do with the original (KHook::Action)
* * HookChain -- whether an SDK helper's call into the game runs the hooks on it
  */

#ifndef _INCLUDE_ITOOLKIT_TYPES_H
#define _INCLUDE_ITOOLKIT_TYPES_H

#pragma once
#include "IToolkitKHook.h"

#include <khook.hpp>

#include <cstdint>
#include <cstring>
#include <functional>
#include <type_traits>
#include <utility>

/// A plugin's id; the core hands it to Load().
using PluginId = int;

/// What a Register* / Hook* call returns and the matching Unregister* /
/// Unhook* takes: the one handler it registered. 0 is never handed out.
using ToolkitHookId = int;

/* =========================
Callbacks
========================= */

namespace toolkit_detail
{
    /// One per callable type, instantiated where the callable is: its address
    /// is a code address inside the plugin that made the callable, which is
    /// how the core tells whose a handler is. Hidden, so the dynamic linker
    /// never hands another module's copy out for it.
    template <typename T>
#if !defined(_MSC_VER)
    __attribute__((visibility("hidden"), noinline))
#endif
    void CallbackOrigin()
    {
    }

    /// A code address of a method: its entry, or for a virtual method the
    /// object's vtable (both live in the module that defined the class).
    template <typename M>
    const void* MethodOrigin(const void* object, M method)
    {
#if defined(_MSC_VER)
        // MSVC: the code, or the vcall thunk in the module that formed the pointer.
        (void)object;
        const void* code = nullptr;
        std::memcpy(&code, &method, sizeof(code));
        return code;
#else
        // Itanium: an odd pointer is 1 + a vtable offset.
        std::uintptr_t ptr = 0;
        std::memcpy(&ptr, &method, sizeof(ptr));
        if (ptr & 1)
            return object ? *static_cast<const void* const*>(object) : nullptr;
        return reinterpret_cast<const void*>(ptr);
#endif
    }
}

template <typename SIG>
class ToolkitCallback;

/**
 * @brief A toolkit handler: a lambda, a free or static function, or an
 * object and one of its methods -- SourceHook's SH_STATIC / SH_MEMBER.
 *
 * @code
 * g_pToolkitCommands->RegisterConCommand("jbtestend", TestEndRound);               // function
 * g_pToolkitCommands->RegisterConCommand("jbtestend", &Commands::TestEndRound);    // static method
 * g_pToolkitCommands->RegisterConCommand("jbtestend", TOOLKIT_MEMBER(this, &Plugin::OnTest));
 * g_pToolkitCommands->RegisterConCommand("jbtestend", [](auto&, auto&, bool) {});  // lambda
 * @endcode
 *
 * Whose a handler is, the core reads off the handler itself: Origin() is a
 * code address inside the plugin that made it, which the core maps to that
 * plugin's library -- so no call takes a plugin id.
 *
 * A function or an object-and-method is also remembered by what it is, so
 * the same one handed to Unregister* / Unhook* finds it again; a lambda has
 * no such identity and is removed by the id its Register* / Hook* returned.
 */
template <typename R, typename... A>
class ToolkitCallback<R(A...)>
{
public:
    ToolkitCallback() = default;

    /// A free function or a static method. Its signature only has to be
    /// callable as this one (a game hook handler may answer a bare Action).
    template <typename FR, typename... FA,
              typename = std::enable_if_t<std::is_invocable_r_v<R, FR (*)(FA...), A...>>>
    ToolkitCallback(FR (*fn)(FA...))
        : m_fn(fn), m_origin(reinterpret_cast<const void*>(fn))
    {
        if (fn)
            Key(nullptr, &fn, sizeof(fn));
    }

    /// An object and one of its methods.
    template <typename C, typename MR, typename... MA>
    ToolkitCallback(C* object, MR (C::*method)(MA...))
        : m_fn([object, method](A... args) -> R { return (object->*method)(std::forward<A>(args)...); }),
          m_origin(toolkit_detail::MethodOrigin(object, method))
    {
        Key(object, &method, sizeof(method));
    }

    template <typename C, typename MR, typename... MA>
    ToolkitCallback(const C* object, MR (C::*method)(MA...) const)
        : m_fn([object, method](A... args) -> R { return (object->*method)(std::forward<A>(args)...); }),
          m_origin(toolkit_detail::MethodOrigin(object, method))
    {
        Key(object, &method, sizeof(method));
    }

    /// A lambda or any other callable.
    template <typename F,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, ToolkitCallback> &&
                                          !std::is_pointer_v<std::decay_t<F>> &&
                                          !std::is_function_v<std::remove_reference_t<F>> &&
                                          std::is_invocable_r_v<R, F&, A...>>>
    ToolkitCallback(F&& callable)
        : m_fn(std::forward<F>(callable)),
          m_origin(reinterpret_cast<const void*>(&toolkit_detail::CallbackOrigin<std::decay_t<F>>))
    {
    }

    R operator()(A... args) const { return m_fn(std::forward<A>(args)...); }

    explicit operator bool() const { return static_cast<bool>(m_fn); }

    /// A code address inside the plugin that made this handler; the core
    /// finds the owner by it.
    const void* Origin() const { return m_origin; }

    /**
     * @brief For a handler made on behalf of another plugin -- a wrapper one
     * plugin puts around a handler another one handed it: the registration
     * then belongs to whoever made `of`, and goes when that plugin unloads.
     *
     * @code
     * ChatHandler wrapped = [handler](auto& ctx, auto& args, bool post) { if (Allowed(ctx)) handler(ctx, args, post); };
     * g_pToolkitCommands->RegisterConCommand(name, std::move(wrapped.OwnedAs(handler)));
     * @endcode
     */
    template <typename SIG>
    ToolkitCallback& OwnedAs(const ToolkitCallback<SIG>& of)
    {
        m_origin = of.Origin();
        return *this;
    }

    /// A function or object-and-method the Unregister* / Unhook* calls can
    /// find again; false for a lambda.
    bool HasIdentity() const { return m_keyed; }

    /// The same function, or the same method of the same object.
    bool SameAs(const ToolkitCallback& other) const
    {
        return m_keyed && other.m_keyed && m_object == other.m_object && std::memcmp(m_key, other.m_key, sizeof(m_key)) == 0;
    }

private:
    void Key(const void* object, const void* bytes, std::size_t size)
    {
        static_assert(sizeof(void (ToolkitCallback::*)()) <= sizeof(m_key), "member pointer too large");
        m_object = object;
        std::memcpy(m_key, bytes, size < sizeof(m_key) ? size : sizeof(m_key));
        m_keyed = true;
    }

    std::function<R(A...)> m_fn;
    const void* m_origin = nullptr;
    const void* m_object = nullptr;
    unsigned char m_key[16] = {};
    bool m_keyed = false;
};

/**
 * @brief SourceHook's SH_STATIC / SH_MEMBER, for a toolkit handler argument.
 *
 * @code
 * g_pToolkitEvents->HookGameEvent("round_end", TOOLKIT_STATIC(OnRoundEnd), true);
 * g_pToolkitEvents->HookGameEvent("round_end", TOOLKIT_MEMBER(this, &Plugin::OnRoundEnd), true);
 * // ... and later the same delegate takes it off again:
 * g_pToolkitEvents->UnhookGameEvent("round_end", TOOLKIT_MEMBER(this, &Plugin::OnRoundEnd), true);
 * @endcode
 *
 * Plain `OnRoundEnd` / `{ this, &Plugin::OnRoundEnd }` do the same; these
 * only spell it the SourceHook way. The owner is the plugin whose code the
 * function or method is, as for every handler.
 */
#define TOOLKIT_STATIC(func) (func)
#define TOOLKIT_MEMBER(inst, func) { inst, func }

/* =========================
Hook control
========================= */

/**
 * @brief What a toolkit callback (command listener, game event hook, net
 * message hook, entity output listener) did about the original.
 *
 * KHook's own result type, so a KHook handler hands a listener's answer straight
 * through and a listener returns what a hook would:
 *
 * * Ignore    -- did nothing; the original runs as it would have.
 * * Override  -- changed something (the event, the return value) but the
 *                original still runs. Pre only.
 * * Supersede -- block the original entirely. Pre only.
 */
using Action = KHook::Action;

/**
 * @brief Whether an SDK helper's call into the game goes through the hooks
 * plugins placed on the function it calls.
 *
 * Every schema helper that calls a hookable game function (TakeDamage,
 * GiveNamedItem, TerminateRound, SetModel, ...) takes one as its last
 * argument:
 *
 * * Run    -- through the Pre/Post chain, as if the game made the call. Right
 *             when the plugin stands in for the game -- a weapon dealing
 *             damage, a round reward handing out an item -- and other plugins'
 *             rules should apply.
 * * Bypass -- straight to the original, past every hook (inline detours and
 *             vtable hooks alike). Right when the server was told to do it --
 *             an admin command, a forced round end -- or when calling the very
 *             function you are hooking, where Run would recurse.
 *
 * The default of each helper is what it always did, so existing calls keep
 * their behaviour; see the helper's declaration.
 *
 * @code
 * pItemServices->GiveNamedItem("weapon_ak47");                        // hooks run
 * pController->TakeDamage(pAttacker, 50, DMG_BULLET, HookChain::Run); // opt in
 * @endcode
 */
enum class HookChain : uint8_t
{
    Run,
    Bypass,
};

/**
 * @brief The function to call for a resolved address: the address itself, or,
 * for HookChain::Bypass, the original behind any detour on it.
 *
 * Resolve at the call, not once: a hook installed later changes the answer.
 */
template <typename FN>
inline FN ResolveHookChain(FN fn, HookChain eChain)
{
    if (eChain != HookChain::Bypass || !fn)
        return fn;

    return reinterpret_cast<FN>(KHook::FindOriginal(reinterpret_cast<void*>(fn)));
}

/**
 * @brief The function to call for vtable slot nIndex of pThis: whatever the
 * slot holds, or, for HookChain::Bypass, the original behind both a vtable hook
 * on the slot and a detour on the function it points at.
 */
inline void* ResolveHookChainVirtual(void* pThis, int nIndex, HookChain eChain)
{
    void** pVTable = pThis ? *static_cast<void***>(pThis) : nullptr;
    if (!pVTable || nIndex < 0)
        return nullptr;

    if (eChain != HookChain::Bypass)
        return pVTable[nIndex];

    return KHook::FindOriginal(KHook::FindOriginalVirtual(pVTable, nIndex));
}


// A raw KHook hook carries the hooked member function and the callbacks in its
// type. CTAD would derive that from the constructor arguments, but MSVC cannot,
// so this takes it from the member the hook is stored in instead -- for a
// constructor's initialiser list:
//
//     KHOOK_NEW(m_hGameFrame, &ISource2Server::GameFrame, this, nullptr, &Plugin::Hook_GameFrame)
//
// The address lookup, Add/Configure and Remove/delete stay yours. The
// KHOOK_VIRTUAL / KHOOK_MEMBER / KHOOK_FUNCTION macros in IToolkitHooks.h do all
// of that in one line, which is what a plugin normally wants.
#define KHOOK_NEW(member, ...) member(new std::remove_pointer_t<decltype(member)>(__VA_ARGS__))

#endif //_INCLUDE_ITOOLKIT_TYPES_H
