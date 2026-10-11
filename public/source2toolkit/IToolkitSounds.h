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
 * @file IToolkitSounds.h
 * @brief Interface for playing, stopping and rewriting sound events.
 *
 * A sound event reaches a client as one network message
 * (CMsgSosStartSoundEvent): the hash of its name, the entity it plays from, a
 * guid and a packed block of parameters. This interface builds and sends that
 * message itself instead of going through an entity on the server, which is
 * what makes the rest possible:
 *
 * * a sound goes to exactly the players you pick -- nobody else receives it,
 *   so there is nothing for them to hear;
 * * Emit() hands back the guid, and the guid stops the sound at once or changes
 *   its parameters while it plays;
 * * volume channels: a player's own volume for "music", "effects" or whatever a
 *   plugin names is applied per recipient, so a player turns the server's
 *   music down in a menu instead of fighting snd_musicvolume;
 * * every sound the game itself starts passes through HookSound(), where it
 *   can be blocked, renamed, re-parametrised or re-addressed.
 *
 * @note Everything here belongs on the main thread.
 */

#ifndef _INCLUDE_ITOOLKIT_SOUNDS_H
#define _INCLUDE_ITOOLKIT_SOUNDS_H

#pragma once
#include "IToolkitPlugin.h"
#include "IToolkitTypes.h"

#include <mathlib/vector.h>
#include <playerslot.h>

#include <cstdint>
#include <functional>

/* =========================
Forward declarations
========================= */

class CEntityInstance;

/* =========================
Types
========================= */

/**
 * @brief Identifies one playing sound on the clients it was sent to.
 *
 * Taken from the engine's own counter, so it never collides with a sound the
 * game started. 0 means "nothing was sent".
 */
using SoundGuid = uint32_t;

/**
 * @brief Recipient set, one bit per player slot (bit N = slot N).
 */
using SoundRecipients = uint64_t;

inline constexpr SoundRecipients SOUND_RECIPIENTS_NONE = 0;

/**
 * @brief Where a sound plays from, for IToolkitSound::SetSourceEntityIndex().
 */
enum : int
{
    /// At each recipient: no position, no falloff, same for everybody who gets it.
    SOUND_SOURCE_LISTENER = -1,
    /// In the world, at the "public.position" parameter (see SetPosition()).
    SOUND_SOURCE_WORLD = 0,
};

/**
 * @brief Wire type of a packed sound parameter.
 *
 * The engine knows more than these; they are the ones whose layout is known,
 * and every public parameter of the stock sound events is one of them.
 * Parameters of any other type on a hooked sound are carried through untouched.
 */
enum class SoundParamType : uint8_t
{
    Invalid = 0,
    Bool    = 1,
    Int32   = 2,
    UInt32  = 3,
    UInt64  = 4,
    Float   = 8,
    Float3  = 10,
};

/* =========================
Sound
========================= */

/**
 * @brief One sound event: what to play, from where, how, and for whom.
 *
 * Made with IToolkitSound::New(), released with `delete`, and reusable in
 * between -- Emit() may be called any number of times, each call is a new
 * sound with a new guid. The
 * same type is what a HookSound() handler receives for a sound the game is
 * about to send.
 *
 * Parameter names are the sound event's public fields ("public.volume",
 * "public.pitch", "public.position", or whatever a custom .vsndevts declares).
 * They travel as hashes, so a name cannot be listed back, only asked for.
 */
class IToolkitSound
{
public:
    virtual ~IToolkitSound() = default;

    /* =========================
    Identity
    ========================= */

    /**
     * @brief Sets the sound event to play, e.g. "Weapon_AK47.Single".
     */
    virtual void SetName(const char* name) = 0;

    /**
     * @brief Sound event name, or "" when only its hash is known.
     *
     * A hooked game sound arrives as a hash, and a hash cannot be turned back
     * into a name. The toolkit resolves it against the names it has been given
     * so far -- everything emitted through this interface and everything
     * passed to IToolkitSounds::HashSoundName() -- so the sounds a plugin
     * looks for come out named, the rest as "".
     */
    virtual const char* GetName() const = 0;

    virtual void SetNameHash(uint32_t hash) = 0;
    virtual uint32_t GetNameHash() const = 0;

    /**
     * @brief Guid of the sound. Assigned by the engine for a hooked sound, by
     *        the last Emit() otherwise (0 before the first one).
     */
    virtual SoundGuid GetGuid() const = 0;

    /* =========================
    Source
    ========================= */

    /**
     * @brief Entity index the sound plays from and follows, or
     *        SOUND_SOURCE_LISTENER (default) / SOUND_SOURCE_WORLD.
     */
    virtual void SetSourceEntityIndex(int index) = 0;
    virtual int GetSourceEntityIndex() const = 0;

    /**
     * @brief Plays the sound from an entity. nullptr is SOUND_SOURCE_LISTENER.
     */
    virtual void SetSourceEntity(CEntityInstance* entity) = 0;

    /**
     * @brief Plays the sound at a point in the world: SOUND_SOURCE_WORLD plus
     *        "public.position".
     */
    virtual void SetPosition(const Vector& position) = 0;

    /* =========================
    Common parameters
    ========================= */

    /**
     * @brief "public.volume". 1.0 is the sound event's own volume.
     *
     * With a channel set, each recipient gets this multiplied by their own
     * volume for that channel.
     */
    virtual void SetVolume(float volume) = 0;
    virtual float GetVolume() const = 0;

    /**
     * @brief "public.pitch". 1.0 is unchanged.
     */
    virtual void SetPitch(float pitch) = 0;
    virtual float GetPitch() const = 0;

    /**
     * @brief Volume channel this sound belongs to, or nullptr/"" for none.
     *
     * See IToolkitSounds::SetPlayerVolume(). A recipient whose volume for the
     * channel is 0 is not sent the sound at all.
     */
    virtual void SetChannel(const char* channel) = 0;
    virtual const char* GetChannel() const = 0;

    /* =========================
    Parameters
    ========================= */

    virtual bool HasParam(const char* name) const = 0;
    virtual SoundParamType GetParamType(const char* name) const = 0;
    virtual void RemoveParam(const char* name) = 0;
    virtual void ClearParams() = 0;

    virtual void SetBool(const char* name, bool value) = 0;
    virtual void SetInt32(const char* name, int32_t value) = 0;
    virtual void SetUInt32(const char* name, uint32_t value) = 0;
    virtual void SetUInt64(const char* name, uint64_t value) = 0;
    virtual void SetFloat(const char* name, float value) = 0;
    virtual void SetFloat3(const char* name, const Vector& value) = 0;

    /**
     * @brief Typed getters. @p def comes back when the parameter is missing or
     *        of another type.
     */
    virtual bool GetBool(const char* name, bool def = false) const = 0;
    virtual int32_t GetInt32(const char* name, int32_t def = 0) const = 0;
    virtual uint32_t GetUInt32(const char* name, uint32_t def = 0) const = 0;
    virtual uint64_t GetUInt64(const char* name, uint64_t def = 0) const = 0;
    virtual float GetFloat(const char* name, float def = 0.0f) const = 0;
    virtual Vector GetFloat3(const char* name, const Vector& def = Vector(0, 0, 0)) const = 0;

    /* =========================
    Recipients
    ========================= */

    virtual void AddRecipient(CPlayerSlot slot) = 0;
    virtual void RemoveRecipient(CPlayerSlot slot) = 0;
    virtual bool HasRecipient(CPlayerSlot slot) const = 0;

    /**
     * @brief Every player connected at the moment of the call.
     */
    virtual void AddAllRecipients() = 0;
    virtual void ClearRecipients() = 0;

    virtual void SetRecipients(SoundRecipients mask) = 0;
    virtual SoundRecipients GetRecipients() const = 0;

    /* =========================
    Emit
    ========================= */

    /**
     * @brief Sends the sound to its recipients.
     *
     * @return Guid of the new sound, 0 when there was nobody to send it to.
     *
     * @note Not available inside a HookSound() handler -- the sound there is
     *       already on its way; change it and return Action::Override.
     */
    virtual SoundGuid Emit() = 0;

    /* =========================
    Virtual constructor (New)
    ========================= */

    /**
     * @brief Creates a sound: listener-sourced, volume and pitch 1, no recipients.
     *
     * Equivalent to `g_pToolkitSounds->CreateSound(g_PluginID, name)`. Release
     * with `delete` or `g_pToolkitSounds->DestroySound()`.
     *
     *     IToolkitSound* music = IToolkitSound::New("jb.duel");
     *     music->SetChannel("music");
     *     music->AddAllRecipients();
     *     SoundGuid guid = music->Emit();
     *     delete music;
     *
     * @param name Sound event name, may be set later with SetName().
     */
    static IToolkitSound* New(const char* name = nullptr);
};

/* =========================
Callback types
========================= */

/**
 * @brief Hook called for every sound the game is about to send to clients.
 *
 * @param sound The sound, decoded. Name (or hash), source entity, parameters
 *              and recipients may all be changed.
 *
 * @return Action::Ignore to leave it alone, Action::Override to send it with
 *         the changes made to @p sound, Action::Supersede to drop it.
 *
 * Sounds sent through this interface do not come back through the hook.
 *
 * @note The server only knows about the sounds it sends. What a client
 *       predicts for itself -- its own weapon fire, its own knife swing, its
 *       own footsteps -- is played locally and never passes through here; the
 *       copy of those sounds that goes to everybody else does.
 */
using SoundHook = ToolkitCallback<Action(IToolkitSound* sound)>;

/* =========================
Core Toolkit Sounds
========================= */

#define TOOLKIT_SOUNDS_INTERFACE "IToolkitSounds002"

class IToolkitSounds
{
public:
    virtual ~IToolkitSounds() = default;

    /* =========================
    Sound objects
    ========================= */

    /**
     * @brief Creates a sound: listener-sourced, volume and pitch 1, no recipients.
     *
     * IToolkitSound::New() is the usual way in. Release with `delete` or
     * DestroySound(); whatever a plugin leaves behind goes when it unloads.
     *
     * @param owner Plugin ID that owns the sound
     * @param name  Sound event name, may be set later
     */
    virtual IToolkitSound* CreateSound(PluginId owner, const char* name = nullptr) = 0;
    virtual void DestroySound(IToolkitSound* sound) = 0;

    /* =========================
    One-call emit
    ========================= */

    /**
     * @brief Plays a sound for one player only, at the listener.
     */
    virtual SoundGuid EmitSoundToPlayer(CPlayerSlot slot, const char* name, float volume = 1.0f, float pitch = 1.0f, const char* channel = nullptr) = 0;

    /**
     * @brief Plays a sound for a set of players, at each listener.
     */
    virtual SoundGuid EmitSoundToPlayers(SoundRecipients recipients, const char* name, float volume = 1.0f, float pitch = 1.0f, const char* channel = nullptr) = 0;

    /**
     * @brief Plays a sound for everybody, at each listener.
     */
    virtual SoundGuid EmitSoundToAll(const char* name, float volume = 1.0f, float pitch = 1.0f, const char* channel = nullptr) = 0;

    /**
     * @brief Plays a sound from an entity, for @p recipients (everybody by default).
     */
    virtual SoundGuid EmitSoundFromEntity(CEntityInstance* entity, const char* name, float volume = 1.0f, float pitch = 1.0f, SoundRecipients recipients = ~SoundRecipients(0), const char* channel = nullptr) = 0;

    /* =========================
    Stopping and changing a playing sound
    ========================= */

    /**
     * @brief Stops a sound at once, for everybody it was sent to.
     */
    virtual void StopSound(SoundGuid guid) = 0;

    /**
     * @brief Stops a sound for some of its recipients; it keeps playing for the rest.
     */
    virtual void StopSoundForPlayers(SoundGuid guid, SoundRecipients recipients) = 0;

    /**
     * @brief Stops a sound by name, including one the game started.
     *
     * @param name              Sound event name
     * @param sourceEntityIndex Entity the sound plays from, SOUND_SOURCE_LISTENER
     *                          for sounds without one
     * @param recipients        Who to stop it for, everybody by default
     */
    virtual void StopSoundByName(const char* name, int sourceEntityIndex = SOUND_SOURCE_LISTENER, SoundRecipients recipients = ~SoundRecipients(0)) = 0;

    /**
     * @brief Changes the volume of a playing sound. Channel volumes still apply.
     */
    virtual void SetSoundVolume(SoundGuid guid, float volume) = 0;

    /**
     * @brief Sends the parameters of @p params to a playing sound.
     *
     * Only the parameters set on @p params are sent; its name, source and
     * recipients are ignored.
     *
     * @param recipients Who to send to, 0 for everybody the sound went to
     */
    virtual void UpdateSoundParams(SoundGuid guid, const IToolkitSound* params, SoundRecipients recipients = SOUND_RECIPIENTS_NONE) = 0;

    /* =========================
    Volume channels
    ========================= */

    /**
     * @brief Sets one player's volume for a channel (0 mutes, 1 is unchanged).
     *
     * A channel is just a name sounds are emitted under ("music", "effects",
     * ...). Applies to every later sound on the channel, and straight away to
     * the ones this player is still hearing: they get louder, quieter, or
     * stop. Forgotten when the player disconnects -- the toolkit keeps nothing
     * on disk, load it from wherever the plugin stores preferences.
     */
    virtual void SetPlayerVolume(CPlayerSlot slot, const char* channel, float volume) = 0;

    /**
     * @brief A player's volume for a channel, 1.0 when never set.
     */
    virtual float GetPlayerVolume(CPlayerSlot slot, const char* channel) = 0;

    /**
     * @brief Puts all of a player's channels back to 1.0.
     */
    virtual void ResetPlayerVolumes(CPlayerSlot slot) = 0;

    /* =========================
    Names and hashes
    ========================= */

    /**
     * @brief Hash a sound event name travels as, for matching a hooked sound:
     *
     *     static const uint32_t knifeHit = g_pToolkitSounds->HashSoundName("Weapon_Knife.Hit");
     *     if (sound->GetNameHash() == knifeHit) ...
     *
     * The name is remembered, so IToolkitSound::GetName() and
     * LookupSoundName() resolve it from then on.
     */
    virtual uint32_t HashSoundName(const char* name) = 0;

    /**
     * @brief Name for a hash, nullptr when the toolkit has not seen it.
     */
    virtual const char* LookupSoundName(uint32_t hash) = 0;

    /* =========================
    Hooks
    ========================= */

    /**
     * @brief Hooks the sounds the game sends to clients.
     *
     * A plugin may hook as often as it likes; each handler runs.
     *
     * Whose it is, the core reads off the handler (see ToolkitCallback); what
     * a plugin still holds at unload the core drops for it.
     *
     * @param handler A function, an object and a method, or a lambda
     * @return The id UnhookSound(id) takes
     */
    virtual ToolkitHookId HookSound(SoundHook handler) = 0;

    /**
     * @brief Drops the hook with this handler -- a function or an object and
     * a method; a lambda goes by its id.
     *
     * @return true when one was found
     */
    virtual bool UnhookSound(const SoundHook& handler) = 0;

    /**
     * @brief Drops the hook HookSound() returned the id for.
     *
     * @return true when it was still there
     */
    virtual bool UnhookSound(ToolkitHookId id) = 0;
};

/* =========================
Helpers
========================= */

/**
 * @brief Recipient mask of one slot.
 */
inline SoundRecipients SoundRecipientOf(CPlayerSlot slot)
{
    const int i = slot.Get();
    return (i >= 0 && i < 64) ? (SoundRecipients(1) << i) : SOUND_RECIPIENTS_NONE;
}

/**
 * @brief Shorthand accessors via g_pToolkitSounds.
 */
#define SOUND_EMIT_TO_PLAYER(slot, ...)     g_pToolkitSounds->EmitSoundToPlayer(slot, __VA_ARGS__)
#define SOUND_EMIT_TO_PLAYERS(mask, ...)    g_pToolkitSounds->EmitSoundToPlayers(mask, __VA_ARGS__)
#define SOUND_EMIT_TO_ALL(...)              g_pToolkitSounds->EmitSoundToAll(__VA_ARGS__)
#define SOUND_EMIT_FROM_ENTITY(entity, ...) g_pToolkitSounds->EmitSoundFromEntity(entity, __VA_ARGS__)
#define SOUND_STOP(guid)                    g_pToolkitSounds->StopSound(guid)

#define HOOK_SOUND(handler)   g_pToolkitSounds->HookSound(handler)
#define UNHOOK_SOUND(handler) g_pToolkitSounds->UnhookSound(handler)

#endif //_INCLUDE_ITOOLKIT_SOUNDS_H
