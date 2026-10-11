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
 * @file IToolkitNetworkMessages.h
 * @brief Interface for manipulating and sending Source2 network (protobuf) messages.
 *
 * Provides functionality for:
 * * Allocating and deallocating network messages by ID or partial name
 * * Reading and writing protobuf fields via reflection (scalar, repeated, nested)
 * * Sending messages to individual players or a set of players
 * * Hooking outgoing server/client network messages
 */

#ifndef _INCLUDE_ITOOLKIT_NETWORK_MESSAGES_H
#define _INCLUDE_ITOOLKIT_NETWORK_MESSAGES_H

#pragma once
#include "IToolkitPlugin.h"
#include "IToolkitTypes.h"

#include <color.h>
#include <engine2/isource2server.h>
#include <engine2/ivengineserver2.h>
#include <mathlib/vector.h>

#include <cstdint>
#include <functional>

/* =========================
Windows macro collisions
========================= */

/*
 * windows.h defines SendMessage as SendMessageA/SendMessageW. It does not
 * error out the way FreeModule does -- it quietly renames the method below and
 * every call through NET_MSG_SEND -- which is worse, because a translation
 * unit that did not pull windows.h in then sees a different class. Drop it.
 */
#ifdef SendMessage
#undef SendMessage
#endif

/* =========================
Callback types
========================= */

/**
 * @brief Hook called when the server sends a message to a set of clients.
 *
 * @param clients  Bitmask of recipient client slots (64-bit, one bit per slot).
 * @param messageid Network message ID.
 * @param msg       Opaque pointer to the protobuf message (cast to google::protobuf::Message*).
 *
 * @return Action::Ignore to allow, Action::Supersede to block.
 */
using NetMessageServerHook = ToolkitCallback<Action(uint64_t* clients, int messageid, void* msg)>;

/**
 * @brief Hook called when a client sends a message to the server, or for internal server sends.
 *
 * @param playerid  Sender/recipient slot index.
 * @param messageid Network message ID.
 * @param msg       Opaque pointer to the protobuf message.
 *
 * @return Action::Ignore to allow, Action::Supersede to block.
 */
using NetMessageClientHook = ToolkitCallback<Action(CPlayerSlot slot, int messageid, void* msg)>;

/* =========================
Core Toolkit Network Messages
========================= */

/**
 * @brief Interface for working with Source2 network messages.
 *
 * All field access is done via protobuf reflection using field names.
 * The @p msg pointer is an opaque handle to an allocated protobuf message;
 * always deallocate with DeallocateNetMessage() when done.
 */
#define TOOLKIT_NETWORKMESSAGES_INTERFACE "IToolkitNetworkMessages002"

class IToolkitNetworkMessages
{
public:
    virtual ~IToolkitNetworkMessages() = default;

    /* =========================
    Allocation
    ========================= */

    /**
     * @brief Allocates a network message by its numeric ID.
     *
     * @param msgid Message ID.
     * @return Opaque message pointer, or nullptr on failure.
     */
    virtual void* AllocateNetMessageByID(int msgid) = 0;

    /**
     * @brief Allocates a network message by a partial name match.
     *
     * @param name Partial name string (e.g. "CSVCMsg_UserMessage").
     * @return Opaque message pointer, or nullptr on failure.
     */
    virtual void* AllocateNetMessageByPartialName(const char* name) = 0;

    /**
     * @brief Deallocates a previously allocated network message.
     *
     * @param msg Message pointer returned by Allocate*.
     */
    virtual void DeallocateNetMessage(void* msg) = 0;

    /* =========================
    Field presence
    ========================= */

    /**
     * @brief Returns true if a singular field is set on the message.
     */
    virtual bool HasField(void* msg, const char* fieldName) = 0;

    /* =========================
    int32
    ========================= */

    virtual int GetInt32(void* msg, const char* fieldName) = 0;
    virtual int GetRepeatedInt32(void* msg, const char* fieldName, int index) = 0;
    virtual void SetInt32(void* msg, const char* fieldName, int value) = 0;
    virtual void SetRepeatedInt32(void* msg, const char* fieldName, int index, int value) = 0;
    virtual void AddInt32(void* msg, const char* fieldName, int value) = 0;

    /* =========================
    int64
    ========================= */

    virtual int64_t GetInt64(void* msg, const char* fieldName) = 0;
    virtual int64_t GetRepeatedInt64(void* msg, const char* fieldName, int index) = 0;
    virtual void SetInt64(void* msg, const char* fieldName, int64_t value) = 0;
    virtual void SetRepeatedInt64(void* msg, const char* fieldName, int index, int64_t value) = 0;
    virtual void AddInt64(void* msg, const char* fieldName, int64_t value) = 0;

    /* =========================
    uint32
    ========================= */

    virtual uint32_t GetUInt32(void* msg, const char* fieldName) = 0;
    virtual uint32_t GetRepeatedUInt32(void* msg, const char* fieldName, int index) = 0;
    virtual void SetUInt32(void* msg, const char* fieldName, uint32_t value) = 0;
    virtual void SetRepeatedUInt32(void* msg, const char* fieldName, int index, uint32_t value) = 0;
    virtual void AddUInt32(void* msg, const char* fieldName, uint32_t value) = 0;

    /* =========================
    uint64
    ========================= */

    virtual uint64_t GetUInt64(void* msg, const char* fieldName) = 0;
    virtual uint64_t GetRepeatedUInt64(void* msg, const char* fieldName, int index) = 0;
    virtual void SetUInt64(void* msg, const char* fieldName, uint64_t value) = 0;
    virtual void SetRepeatedUInt64(void* msg, const char* fieldName, int index, uint64_t value) = 0;
    virtual void AddUInt64(void* msg, const char* fieldName, uint64_t value) = 0;

    /* =========================
    bool
    ========================= */

    virtual bool GetBool(void* msg, const char* fieldName) = 0;
    virtual bool GetRepeatedBool(void* msg, const char* fieldName, int index) = 0;
    virtual void SetBool(void* msg, const char* fieldName, bool value) = 0;
    virtual void SetRepeatedBool(void* msg, const char* fieldName, int index, bool value) = 0;
    virtual void AddBool(void* msg, const char* fieldName, bool value) = 0;

    /* =========================
    float
    ========================= */

    virtual float GetFloat(void* msg, const char* fieldName) = 0;
    virtual float GetRepeatedFloat(void* msg, const char* fieldName, int index) = 0;
    virtual void SetFloat(void* msg, const char* fieldName, float value) = 0;
    virtual void SetRepeatedFloat(void* msg, const char* fieldName, int index, float value) = 0;
    virtual void AddFloat(void* msg, const char* fieldName, float value) = 0;

    /* =========================
    double
    ========================= */

    virtual double GetDouble(void* msg, const char* fieldName) = 0;
    virtual double GetRepeatedDouble(void* msg, const char* fieldName, int index) = 0;
    virtual void SetDouble(void* msg, const char* fieldName, double value) = 0;
    virtual void SetRepeatedDouble(void* msg, const char* fieldName, int index, double value) = 0;
    virtual void AddDouble(void* msg, const char* fieldName, double value) = 0;

    /* =========================
    string
    ========================= */

    virtual std::string GetString(void* msg, const char* fieldName) = 0;
    virtual std::string GetRepeatedString(void* msg, const char* fieldName, int index) = 0;
    virtual void SetString(void* msg, const char* fieldName, const char* value) = 0;
    virtual void SetRepeatedString(void* msg, const char* fieldName, int index, const char* value) = 0;
    virtual void AddString(void* msg, const char* fieldName, const char* value) = 0;

    /* =========================
    Vector2D
    ========================= */

    virtual Vector2D GetVector2D(void* msg, const char* fieldName) = 0;
    virtual Vector2D GetRepeatedVector2D(void* msg, const char* fieldName, int index) = 0;
    virtual void SetVector2D(void* msg, const char* fieldName, Vector2D value) = 0;
    virtual void SetRepeatedVector2D(void* msg, const char* fieldName, int index, Vector2D value) = 0;
    virtual void AddVector2D(void* msg, const char* fieldName, Vector2D value) = 0;

    /* =========================
    Vector
    ========================= */

    virtual Vector GetVector(void* msg, const char* fieldName) = 0;
    virtual Vector GetRepeatedVector(void* msg, const char* fieldName, int index) = 0;
    virtual void SetVector(void* msg, const char* fieldName, Vector value) = 0;
    virtual void SetRepeatedVector(void* msg, const char* fieldName, int index, Vector value) = 0;
    virtual void AddVector(void* msg, const char* fieldName, Vector value) = 0;

    /* =========================
    Color
    ========================= */

    virtual Color GetColor(void* msg, const char* fieldName) = 0;
    virtual Color GetRepeatedColor(void* msg, const char* fieldName, int index) = 0;
    virtual void SetColor(void* msg, const char* fieldName, Color value) = 0;
    virtual void SetRepeatedColor(void* msg, const char* fieldName, int index, Color value) = 0;
    virtual void AddColor(void* msg, const char* fieldName, Color value) = 0;

    /* =========================
    QAngle
    ========================= */

    virtual QAngle GetQAngle(void* msg, const char* fieldName) = 0;
    virtual QAngle GetRepeatedQAngle(void* msg, const char* fieldName, int index) = 0;
    virtual void SetQAngle(void* msg, const char* fieldName, QAngle value) = 0;
    virtual void SetRepeatedQAngle(void* msg, const char* fieldName, int index, QAngle value) = 0;
    virtual void AddQAngle(void* msg, const char* fieldName, QAngle value) = 0;

    /* =========================
    bytes
    ========================= */

    /**
     * @brief Copies a bytes field into @p out.
     *
     * Pass @p out as nullptr to query the size without copying.
     * @return Number of bytes in the field.
     */
    virtual int GetBytes(uint8_t* out, void* msg, const char* fieldName) = 0;
    virtual int GetRepeatedBytes(uint8_t* out, void* msg, const char* fieldName, int index) = 0;
    virtual void SetBytes(void* msg, const char* fieldName, const char* value, int valueLength) = 0;
    virtual void SetRepeatedBytes(void* msg, const char* fieldName, int index, const char* value, int valueLength) = 0;
    virtual void AddBytes(void* msg, const char* fieldName, const char* value, int valueLength) = 0;

    /* =========================
    Nested messages
    ========================= */

    /**
     * @brief Returns a mutable pointer to a singular nested message field.
     */
    virtual void* GetNestedMessage(void* msg, const char* fieldName) = 0;

    /**
     * @brief Returns a mutable pointer to a repeated nested message element.
     */
    virtual void* GetRepeatedNestedMessage(void* msg, const char* fieldName, int index) = 0;

    /**
     * @brief Appends a new element to a repeated nested message field and returns it.
     */
    virtual void* AddNestedMessage(void* msg, const char* fieldName) = 0;

    /* =========================
    Repeated field utilities
    ========================= */

    virtual int GetRepeatedFieldSize(void* msg, const char* fieldName) = 0;
    virtual void ClearRepeatedField(void* msg, const char* fieldName) = 0;

    /**
     * @brief Clears all fields on the message.
     */
    virtual void Clear(void* msg) = 0;

    /* =========================
    Sending
    ========================= */

    /**
     * @brief Sends a network message to a single player.
     *
     * @param msg      Allocated message to send (not freed by this call).
     * @param msgid    Message ID matching the allocation.
     * @param playerid Recipient slot index.
     */
    virtual void SendMessage(void* msg, int msgid, CPlayerSlot slot) = 0;

    /**
     * @brief Sends a network message to a set of players.
     *
     * @param msg        Allocated message to send (not freed by this call).
     * @param msgid      Message ID matching the allocation.
     * @param playermask 64-bit bitmask; bit N = slot N.
     */
    virtual void SendMessageToPlayers(void* msg, int msgid, uint64_t playermask) = 0;

    /* =========================
    Hooks
    ========================= */

    /**
     * @brief Hooks outgoing server-to-client messages.
     *
     * A plugin may hook as often as it likes; each handler runs. The
     * toolkit drops what a plugin still holds when it unloads, so a plugin
     * that forgets to unhook does not leave the engine calling into a closed
     * library. Whose a hook is, the core reads off the handler (see
     * ToolkitCallback).
     *
     * @param handler A function, an object and a method, or a lambda
     * @return The id UnhookServerMessage(id) takes
     */
    virtual ToolkitHookId HookServerMessage(NetMessageServerHook handler) = 0;

    /**
     * @brief Drops the hook with this handler -- a function or an object and
     * a method; a lambda goes by its id.
     *
     * @return true when one was found
     */
    virtual bool UnhookServerMessage(const NetMessageServerHook& handler) = 0;

    /**
     * @brief Drops the hook HookServerMessage() returned the id for.
     *
     * @return true when it was still there
     */
    virtual bool UnhookServerMessage(ToolkitHookId id) = 0;

    /**
     * @brief Hooks incoming client-to-server messages (as HookServerMessage()).
     *
     * @return The id UnhookClientMessage(id) takes
     */
    virtual ToolkitHookId HookClientMessage(NetMessageClientHook handler) = 0;

    /**
     * @brief Drops the hook with this handler -- a function or an object and
     * a method; a lambda goes by its id.
     *
     * @return true when one was found
     */
    virtual bool UnhookClientMessage(const NetMessageClientHook& handler) = 0;

    /**
     * @brief Drops the hook HookClientMessage() returned the id for.
     *
     * @return true when it was still there
     */
    virtual bool UnhookClientMessage(ToolkitHookId id) = 0;

    /**
     * @brief Hooks internal server-side message sends, per client (as
     * HookServerMessage()).
     *
     * @return The id UnhookServerInternalMessage(id) takes
     */
    virtual ToolkitHookId HookServerInternalMessage(NetMessageClientHook handler) = 0;

    /**
     * @brief Drops the hook with this handler -- a function or an object and
     * a method; a lambda goes by its id.
     *
     * @return true when one was found
     */
    virtual bool UnhookServerInternalMessage(const NetMessageClientHook& handler) = 0;

    /**
     * @brief Drops the hook HookServerInternalMessage() returned the id for.
     *
     * @return true when it was still there
     */
    virtual bool UnhookServerInternalMessage(ToolkitHookId id) = 0;
};

/**
 * @brief Shorthand accessors via g_ToolkitAPI.
 */
#define NET_MSG_ALLOC_BY_ID(id)              g_pToolkitNetworkMessages->AllocateNetMessageByID(id)
#define NET_MSG_ALLOC_BY_NAME(name)          g_pToolkitNetworkMessages->AllocateNetMessageByPartialName(name)
#define NET_MSG_FREE(msg)                    g_pToolkitNetworkMessages->DeallocateNetMessage(msg)
#define NET_MSG_SEND(msg, id, slot)          g_pToolkitNetworkMessages->SendMessage(msg, id, slot)
#define NET_MSG_SEND_PLAYERS(msg, id, mask)  g_pToolkitNetworkMessages->SendMessageToPlayers(msg, id, mask)

/**
 * @brief Hook macros, matching HOOK_GAME_EVENT and HOOK_CONVAR_CHANGE.
 */
#define HOOK_SERVER_MESSAGE(handler)             g_pToolkitNetworkMessages->HookServerMessage(handler)
#define UNHOOK_SERVER_MESSAGE(handler)           g_pToolkitNetworkMessages->UnhookServerMessage(handler)
#define HOOK_CLIENT_MESSAGE(handler)             g_pToolkitNetworkMessages->HookClientMessage(handler)
#define UNHOOK_CLIENT_MESSAGE(handler)           g_pToolkitNetworkMessages->UnhookClientMessage(handler)
#define HOOK_SERVER_INTERNAL_MESSAGE(handler)    g_pToolkitNetworkMessages->HookServerInternalMessage(handler)
#define UNHOOK_SERVER_INTERNAL_MESSAGE(handler)  g_pToolkitNetworkMessages->UnhookServerInternalMessage(handler)

#endif //_INCLUDE_ITOOLKIT_NETWORK_MESSAGES_H
