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

* @file schema.h
* @brief Schema system utilities for accessing and modifying entity fields.
*
* Provides:
* * Runtime field offset resolution (via SchemaSystem)
* * Safe access to entity properties
* * Automatic network state updates
* * Compile-time hashing for fast lookup
*
* @note This system is similar to Source2 schema/netvar access.
  */

#pragma once

#ifndef SCHEMA_H
#define SCHEMA_H

#include "igameevents.h"
#include "igameeventsystem.h"
#include "schemasystem.h"
#include "schemasystem/schematypes.h"
#include "entity2/entityclass.h"
#include "entity2/entitysystem.h"
#include "entity2/entityidentity.h"
#include "entity2/entityinstance.h"
#include "const.h"
#include "utlmap.h"
#include "tier0/dbg.h"

#include <type_traits>
#include <cstdint>
#include <string>
#include <string_view>

/* =========================
Engine interface getters
========================= */

/**

* @brief Global accessors for engine interfaces.
  */
IGameEventManager2* GetGameEventManager();
CGlobalVars* GetGlobalVars();
ICvar* GetCVar();
ISource2Server* GetSource2Server();
IVEngineServer2* GetEngineServer();
IGameEventSystem* GetGameEventSystem();
INetworkMessages* GetNetworkMessages();
INetworkServerService* GetNetworkServerService();
CGameEntitySystem* GetEntitySystem();
CSchemaSystem* GetSchemaSystem();

/* =========================
Schema core types
========================= */

class GameSessionConfiguration_t
{
};


typedef void (*CEntityNameString)(void);


struct CGlobalSymbol
{
private:
    const char* m_pszString;

    static constexpr std::size_t BLOCK_SIZE = 8192;

    static std::unordered_map<std::string, const char*>& Cache()
    {
        static std::unordered_map<std::string, const char*> s_cache;
        return s_cache;
    }

    static std::shared_mutex& PoolMutex()
    {
        static std::shared_mutex s_mtx;
        return s_mtx;
    }

    static char*& CurrentBlock()
    {
        static char* s_block = nullptr;
        return s_block;
    }

    static std::size_t& RemainingBytes()
    {
        static std::size_t s_remaining = 0;
        return s_remaining;
    }

    static const char* Allocate(const char* str)
    {
        if (str == nullptr) return nullptr;

        const std::size_t byteCount = std::strlen(str);
        const std::size_t neededSize = byteCount + 1; // + '\0'

        {
            std::shared_lock<std::shared_mutex> rlock(PoolMutex());
            auto& cache = Cache();
            auto it = cache.find(std::string(str, byteCount));
            if (it != cache.end())
                return it->second;
        }

        std::unique_lock<std::shared_mutex> wlock(PoolMutex());

        std::string key(str, byteCount);
        auto& cache = Cache();

        if (auto it = cache.find(key); it != cache.end())
            return it->second;

        char* addr;

        if (neededSize > BLOCK_SIZE / 2)
        {
            addr = static_cast<char*>(std::malloc(neededSize));
        }
        else
        {
            if (RemainingBytes() < neededSize)
            {
                CurrentBlock() = static_cast<char*>(std::malloc(BLOCK_SIZE));
                RemainingBytes() = BLOCK_SIZE;
            }
            addr = CurrentBlock();
            CurrentBlock() += neededSize;
            RemainingBytes() -= neededSize;
        }

        std::memcpy(addr, str, byteCount);
        addr[byteCount] = '\0';

        cache.emplace(std::move(key), addr);
        return addr;
    }

public:
    const char* Get() const
    {
        return (m_pszString == nullptr) ? "" : m_pszString;
    }

    void Set(const char* value)
    {
        m_pszString = Allocate(value);
    }
};

// Referenced as a field type by locksound_t, CFuncMoveLinear, CFuncRotating and
// CMessage, but absent from the schema dump, so the generator never emits it and
// s2sdk does not declare it either -- those four classes did not compile at all.
//
// It is a pooled sound-event name, i.e. the same shape as the symbol type above,
// and SCHEMA_FIELD needs a complete type (it takes sizeof of the field), so a
// forward declaration will not do. Aliasing CGlobalSymbol keeps the size right
// at 8 bytes; if a caller ever needs to read one of these fields for real, verify
// the layout against the engine first.
using CGameSoundEventName = CGlobalSymbol;

static_assert(sizeof(CGlobalSymbol) == 8);

using RotationVector = void*;

/**

* @brief Represents schema offset information.
  */
struct SchemaKey
{
    /// Offset of the field
    int32 offset;

    /// True if field is networked
    bool networked;

    /// For an atomic collection field, the engine's own element accessor.
    ///
    /// Some networked collections are not laid out like a plain CUtlVector --
    /// CUtlVectorEmbeddedNetworkVar, for one -- so indexing them directly reads
    /// the wrong memory. The schema system publishes a manipulator per such
    /// field; going through it is the only correct way to reach an element.
    /// Null for every other field.
    SchemaCollectionManipulatorFn_t manipulator = nullptr;

    /// False when the field is not in the running game's schema (renamed,
    /// removed). Reads then come from zeroed scratch memory and writes go
    /// there, so the plugin keeps running with that one field doing nothing
    /// -- an error line says which, once.
    bool valid = true;
};

/**

* @brief Helper for networked variable propagation.
  */
class CNetworkVarChainer2
{
public:
    CEntityInstance* m_pEntity;

private:
    uint8 pad_0000[24];

public:
    ChangeAccessorFieldPathIndex_t m_PathIndex;

private:
    uint8 pad_0024[4];
};

/* =========================
Strong handle wrapper
========================= */

/**

* @brief Lightweight wrapper for entity pointers.
  */
template <typename T>
class CStrongHandle
{
private:
    T* m_pValue;

public:
    T* Get() const { return m_pValue; }
    void Set(void* pPtr) { m_pValue = pPtr; }

    bool IsValid() const { return m_pValue != nullptr; }

    T* operator->() const { return m_pValue; }
    operator T*() const { return m_pValue; }
};

template <typename T>
class CStrongHandleCopyable
{
private:
    T* m_pValue;

public:
    T* Get() const { return m_pValue; }
    void Set(void* pPtr) { m_pValue = pPtr; }

    bool IsValid() const { return m_pValue != nullptr; }

    T* operator->() const { return m_pValue; }
    operator T*() const { return m_pValue; }
};

template <typename T>
class CWeakHandle
{
private:
    T* m_pValue;

public:
    T* Get() const { return m_pValue; }
    void Set(void* pPtr) { m_pValue = pPtr; }

    bool IsValid() const { return m_pValue != nullptr; }

    T* operator->() const { return m_pValue; }
    operator T*() const { return m_pValue; }
};

/* =========================
Network state helpers
========================= */

void EntityNetworkStateChanged(uintptr_t pEntity, uint nOffset);
void ChainNetworkStateChanged(uintptr_t pNetworkVarChainer, uint nOffset);
void NetworkVarStateChanged(uintptr_t pNetworkVar, uint32_t nOffset, uint32 nNetworkStateChangedOffset);

/* =========================
Schema API
========================= */

namespace schema
{
    /**
    * @brief Finds chain offset for a class.
    */
    int16_t FindChainOffset(const char* className, uint32_t classNameHash);

    int16_t FindChainOffset(const char* className);

    /**
     * @brief Gets schema offset for a field.
     */
    SchemaKey GetOffset(const char* className, uint32_t classKey,
                        const char* memberName, uint32_t memberKey);

    /**
     * @brief Scratch memory a missing field reads from and writes to; zeroed,
     * one block for every such field in the plugin.
     */
    void* MissingFieldStorage(size_t size);

    template <typename T>
    T& MissingField()
    {
        return *static_cast<T*>(MissingFieldStorage(sizeof(T)));
    }

    /**
     * @brief Gets server offset (fallback).
     */
    int32_t GetServerOffset(const char* pszClassName, const char* pszPropName);

    /**
     * @brief Gets class size.
     */
    int32_t GetClassSize(const char* className);

    /**
     * @brief Marks field as changed (network update).
     */
    void SetStateChanged(CEntityInstance* entity, const char* className, const char* propName);
}

/* =========================
Compile-time hashing
========================= */

/**

* @brief FNV-1a 32-bit hash (constexpr).
  */
inline constexpr uint32_t hash_32_fnv1a_const(const char* const str,
                                              const uint32_t value = 0x811c9dc5) noexcept
{
    return (str[0] == '\0')
               ? value
               : hash_32_fnv1a_const(&str[1], (value ^ uint32_t(str[0])) * 0x1000193);
}

/**

* @brief FNV-1a 64-bit hash (constexpr).
  */
inline constexpr uint64_t hash_64_fnv1a_const(const char* const str,
                                              const uint64_t value = 0xcbf29ce484222325) noexcept
{
    return (str[0] == '\0')
               ? value
               : hash_64_fnv1a_const(&str[1], (value ^ uint64_t(str[0])) * 0x100000001b3);
}

/* =========================
Schema API by type
========================= */

namespace schema
{
    /**
     * @brief The C++ name of T at compile time ("CChicken"); schema class names are the C++ ones.
     */
    template <typename T>
    constexpr std::string_view ClassName()
    {
#ifdef _MSC_VER
        // "... schema::ClassName<class CChicken>(void)"
        std::string_view name = __FUNCSIG__;
        name.remove_prefix(name.find("ClassName<") + sizeof("ClassName<") - 1);
        name = name.substr(0, name.rfind(">(void)"));
        for (std::string_view prefix : {"class ", "struct "})
        {
            if (name.substr(0, prefix.size()) == prefix)
                name.remove_prefix(prefix.size());
        }
#else
        // Clang: "... [T = CChicken]", GCC: "... [with T = CChicken; ...]"
        std::string_view name = __PRETTY_FUNCTION__;
        name.remove_prefix(name.find("T = ") + sizeof("T = ") - 1);
        name = name.substr(0, name.find_first_of(";]"));
#endif
        return name;
    }

    /**
     * @brief ClassName<T>() as a null-terminated string.
     */
    template <typename T>
    const char* ClassNameCStr()
    {
        static const std::string s_className(ClassName<T>());
        return s_className.c_str();
    }

    /*
     * Each function taking a class name also takes the class as a type or as a
     * pointer (its static type), e.g. schema::GetServerOffset<CChicken>("m_leader")
     * or schema::GetServerOffset(this, "m_leader"). Fields of base classes are
     * found too, so the pointer's type need not be the declaring class.
     */

    template <typename T>
    int16_t FindChainOffset() { return FindChainOffset(ClassNameCStr<T>()); }
    template <typename T>
    int16_t FindChainOffset(const T*) { return FindChainOffset<T>(); }

    template <typename T>
    SchemaKey GetOffset(const char* memberName)
    {
        static const uint32_t s_classKey = hash_32_fnv1a_const(ClassNameCStr<T>());
        return GetOffset(ClassNameCStr<T>(), s_classKey, memberName, hash_32_fnv1a_const(memberName));
    }
    template <typename T>
    SchemaKey GetOffset(const T*, const char* memberName) { return GetOffset<T>(memberName); }

    template <typename T>
    int32_t GetServerOffset(const char* pszPropName) { return GetServerOffset(ClassNameCStr<T>(), pszPropName); }
    template <typename T>
    int32_t GetServerOffset(const T*, const char* pszPropName) { return GetServerOffset<T>(pszPropName); }

    template <typename T>
    int32_t GetClassSize() { return GetClassSize(ClassNameCStr<T>()); }
    template <typename T>
    int32_t GetClassSize(const T*) { return GetClassSize<T>(); }

    /**
     * @brief SetStateChanged(entity, "m_iHealth"), the class taken from the entity's type.
     */
    template <typename T>
    void SetStateChanged(T* entity, const char* propName)
    {
        static_assert(std::is_base_of_v<CEntityInstance, T>, "SetStateChanged needs an entity");
        SetStateChanged(entity, ClassNameCStr<T>(), propName);
    }
}

/* =========================
Writable trait
========================= */

/**

* @brief Determines if schema field can be written directly.
  */
template <typename T>
inline constexpr bool schema_writable_v =
    std::is_pointer_v<T> ||
    std::is_trivially_copyable_v<T> ||
    std::is_same_v<T, Vector> ||
    std::is_same_v<T, QAngle> ||
    std::is_same_v<T, Color>;

/**

* @brief Puts a parameter in a non-deduced context.
* * The field setters are templates only so enable_if can pick an overload; T
* * must stay the field's own type. Deduced from the argument, `m_nByte = 1`
* * made T an int and wrote four bytes over a one-byte field.
  */
template <typename T>
struct schema_identity { using type = T; };
template <typename T>
using schema_identity_t = typename schema_identity<T>::type;

/**

* @brief Types the read-modify-write operators (++, +=, ...) are generated for.
  */
template <typename T>
inline constexpr bool schema_arithmetic_v = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

/**

* @brief Types the bitwise operators (|=, &=, ...) are generated for.
  */
template <typename T>
inline constexpr bool schema_bitwise_v = (std::is_integral_v<T> && !std::is_same_v<T, bool>) || std::is_enum_v<T>;

/**

* @brief The integer type a bitwise operator computes in: the underlying type of an enum, otherwise T.
  */
template <typename T, bool = std::is_enum_v<T>>
struct schema_bits { using type = T; };
template <typename T>
struct schema_bits<T, true> { using type = std::underlying_type_t<T>; };
template <typename T>
using schema_bits_t = typename schema_bits<T>::type;

/* =========================
Schema field macros
========================= */

#define SCHEMA_FIELD_OFFSET(type, varName, extra_offset)                                                                     \
	class varName##_prop                                                                                                     \
	{                                                                                                                     \
	public:                                                                                                                  \
		/*The deleted copy ctor below is user-declared, which suppresses the*/                                               \
		/*implicit default one -- without this a schema class that has a real*/                                              \
		/*constructor cannot initialise its own fields.*/                                                                    \
		varName##_prop() = default;                                                                                          \
		std::add_lvalue_reference_t<type> Get()                                                                              \
		{                                                                                                                    \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);              \
			if (!m_key.valid) return schema::MissingField<type>();                                             \
			static const auto m_offset = offsetof(ThisClass, varName);														 \
																															 \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                             \
                                                                                                                             \
			return *reinterpret_cast<std::add_pointer_t<type>>(pThisClass + m_key.offset + extra_offset);                    \
		}                                                                                                                    \
		/*Const overload, so a const method of the owning class can read the*/                                               \
		/*field. Reading only needs the wrapper's address, which const does*/                                                \
		/*not get in the way of.*/                                                                                           \
		std::add_lvalue_reference_t<const type> Get() const                                                                  \
		{                                                                                                                    \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);                 \
			if (!m_key.valid) return schema::MissingField<const type>();                                             \
			static const auto m_offset = offsetof(ThisClass, varName);                                                          \
                                                                                                                       \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                                \
                                                                                                                       \
			return *reinterpret_cast<std::add_pointer_t<const type>>(pThisClass + m_key.offset + extra_offset);                 \
		}                                                                                                                    \
		template <typename T = type>                                                                                         \
		std::enable_if_t<std::is_pointer_v<T>, void>                                                                         \
		Set(schema_identity_t<T> val)                                                                                       \
		{                                                                                                                    \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);              \
			if (!m_key.valid) return;                                                                          \
			static const auto m_offset = offsetof(ThisClass, varName);                                                       \
																															 \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                             \
																															 \
			NetworkStateChanged();                                                                                           \
			*reinterpret_cast<T*>(pThisClass + m_key.offset + extra_offset) = val;                                           \
		}																													 \
		template <typename T = type>                                                                                         \
		std::enable_if_t<!std::is_pointer_v<T> && std::is_trivially_copyable_v<T>, void>                                     \
		Set(schema_identity_t<T> val)                                                                                       \
	    {                                                                                                                 \
    		static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);              \
    		if (!m_key.valid) return;                                                                          \
			static const auto m_offset = offsetof(ThisClass, varName);                                                       \
																															 \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                             \
																															 \
			NetworkStateChanged();                                                                                           \
			*reinterpret_cast<T*>(pThisClass + m_key.offset + extra_offset) = val;                                           \
		}																													 \
		template <typename T = type>                                                                                         \
		std::enable_if_t<!std::is_pointer_v<T> && !std::is_trivially_copyable_v<T>, void>                                    \
		Set(const schema_identity_t<T>& val)                                                                                \
		{                                                                                                                    \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);              \
			if (!m_key.valid) return;                                                                          \
			static const auto m_offset = offsetof(ThisClass, varName);                                                       \
																															 \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                             \
																															 \
			NetworkStateChanged();                                                                                           \
			memcpy(                                                                                                          \
			reinterpret_cast<void*>(pThisClass + m_key.offset + extra_offset),												 \
			&val,																											 \
			sizeof(type)																									 \
			);                                                                                                               \
		}																													 \
		void NetworkStateChanged()                                                                                           \
		{                                                                                                                    \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);				 \
			if (!m_key.valid) return;                                                                          \
			static const auto m_chain = schema::FindChainOffset(m_className, m_classNameHash);								 \
			static const auto m_offset = offsetof(ThisClass, varName);														 \
																															 \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                             \
                                                                                                                             \
			if (m_chain != 0 && m_key.networked)                                                                             \
			{                                                                                                                \
				ChainNetworkStateChanged(pThisClass + m_chain, m_key.offset + extra_offset);								 \
			}                                                                                                                \
			else if (m_key.networked)                                                                                        \
			{                                                                                                                \
				if (!m_networkStateChangedOffset)                                                                            \
					EntityNetworkStateChanged(pThisClass, m_key.offset + extra_offset);									     \
				else                                                                                                         \
					NetworkVarStateChanged(pThisClass, m_key.offset + extra_offset, m_networkStateChangedOffset);			 \
			}                                                                                                                \
		}                                                                                                                    \
				/*The engine's element accessor for this field, when it has one. See*/                                               \
		/*SchemaKey::manipulator -- needed for collections whose layout is not*/                                             \
		/*a plain CUtlVector.*/                                                                                             \
		SchemaCollectionManipulatorFn_t GetManipulator()                                                                    \
		{                                                                                                                   \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);              \
			return m_key.manipulator;                                                                                       \
		}                                                                                                                   \
		operator std::add_lvalue_reference_t<type>()                                                                         \
		{                                                                                                                    \
			return Get();                                                                                                    \
		}                                                                                                                    \
		std::add_lvalue_reference_t<type> operator()()                                                                       \
		{                                                                                                                    \
			return Get();                                                                                                    \
		}                                                                                                                    \
		std::add_lvalue_reference_t<type> operator->()                                                                       \
		{                                                                                                                    \
			return Get();                                                                                                    \
		}                                                                                                                    \
		operator std::add_lvalue_reference_t<const type>() const                                                             \
		{                                                                                                                    \
			return Get();                                                                                                          \
		}                                                                                                                    \
		std::add_lvalue_reference_t<const type> operator()() const                                                           \
		{                                                                                                                    \
			return Get();                                                                                                          \
		}                                                                                                                    \
		std::add_lvalue_reference_t<const type> operator->() const                                                           \
		{                                                                                                                    \
			return Get();                                                                                                          \
		}                                                                                                                    \
		template <typename T = type>                                                                                         \
		std::enable_if_t<schema_writable_v<T>, void>                                                                         \
		operator()(schema_identity_t<T> val)                                                                                \
		{                                                                                                                    \
			Set(val);                                                                                                        \
		}																													 \
		template <typename T = type>                                                                                         \
		std::enable_if_t<schema_writable_v<T>, varName##_prop&>                                                              \
		operator=(schema_identity_t<T> val)                                                                                 \
		{                                                                                                                    \
			Set(val);                                                                                                        \
			return *this;                                                                                                    \
		}																													 \
		/*Read-modify-write operators. Without them `m_x++` and `m_x |= F` still*/                                           \
		/*compile -- through the implicit conversion to type& above -- but they*/                                            \
		/*write past Set(), so NetworkStateChanged() never runs. These go*/                                                  \
		/*through Set(). `m_x()++` stays the raw, unannounced write.*/                                                       \
		template <typename T = type>                                                                                         \
		std::enable_if_t<schema_arithmetic_v<T>, varName##_prop&>                                                            \
		operator++()                                                                                                         \
		{                                                                                                                    \
			Set(static_cast<T>(static_cast<T>(Get()) + 1));                                                                         \
			return *this;                                                                                                    \
		}                                                                                                                    \
		template <typename T = type>                                                                                         \
		std::enable_if_t<schema_arithmetic_v<T>, T>                                                                          \
		operator++(int)                                                                                                      \
		{                                                                                                                    \
			T old = Get();                                                                                                   \
			Set(static_cast<T>(old + 1));                                                                                    \
			return old;                                                                                                      \
		}                                                                                                                    \
		template <typename T = type>                                                                                         \
		std::enable_if_t<schema_arithmetic_v<T>, varName##_prop&>                                                            \
		operator--()                                                                                                         \
		{                                                                                                                    \
			Set(static_cast<T>(static_cast<T>(Get()) - 1));                                                                         \
			return *this;                                                                                                    \
		}                                                                                                                    \
		template <typename T = type>                                                                                         \
		std::enable_if_t<schema_arithmetic_v<T>, T>                                                                          \
		operator--(int)                                                                                                      \
		{                                                                                                                    \
			T old = Get();                                                                                                   \
			Set(static_cast<T>(old - 1));                                                                                    \
			return old;                                                                                                      \
		}                                                                                                                    \
		template <typename U, typename T = type>                                                                             \
		std::enable_if_t<schema_arithmetic_v<T>, varName##_prop&>                                                            \
		operator+=(U val)                                                                                                    \
		{                                                                                                                    \
			Set(static_cast<T>(Get() + val));                                                                                \
			return *this;                                                                                                    \
		}                                                                                                                    \
		template <typename U, typename T = type>                                                                             \
		std::enable_if_t<schema_arithmetic_v<T>, varName##_prop&>                                                            \
		operator-=(U val)                                                                                                    \
		{                                                                                                                    \
			Set(static_cast<T>(Get() - val));                                                                                \
			return *this;                                                                                                    \
		}                                                                                                                    \
		template <typename U, typename T = type>                                                                             \
		std::enable_if_t<schema_arithmetic_v<T>, varName##_prop&>                                                            \
		operator*=(U val)                                                                                                    \
		{                                                                                                                    \
			Set(static_cast<T>(Get() * val));                                                                                \
			return *this;                                                                                                    \
		}                                                                                                                    \
		template <typename U, typename T = type>                                                                             \
		std::enable_if_t<schema_arithmetic_v<T>, varName##_prop&>                                                            \
		operator/=(U val)                                                                                                    \
		{                                                                                                                    \
			Set(static_cast<T>(Get() / val));                                                                                \
			return *this;                                                                                                    \
		}                                                                                                                    \
		/*Bitwise ones also cover enum fields: computed in the underlying type,*/                                            \
		/*so a scoped enum of flags needs no operators of its own.*/                                                         \
		template <typename U, typename T = type>                                                                             \
		std::enable_if_t<schema_bitwise_v<T>, varName##_prop&>                                                               \
		operator|=(U val)                                                                                                    \
		{                                                                                                                    \
			Set(static_cast<T>(static_cast<schema_bits_t<T>>(Get()) | static_cast<schema_bits_t<T>>(val)));                  \
			return *this;                                                                                                    \
		}                                                                                                                    \
		template <typename U, typename T = type>                                                                             \
		std::enable_if_t<schema_bitwise_v<T>, varName##_prop&>                                                               \
		operator&=(U val)                                                                                                    \
		{                                                                                                                    \
			Set(static_cast<T>(static_cast<schema_bits_t<T>>(Get()) & static_cast<schema_bits_t<T>>(val)));                  \
			return *this;                                                                                                    \
		}                                                                                                                    \
		template <typename U, typename T = type>                                                                             \
		std::enable_if_t<schema_bitwise_v<T>, varName##_prop&>                                                               \
		operator^=(U val)                                                                                                    \
		{                                                                                                                    \
			Set(static_cast<T>(static_cast<schema_bits_t<T>>(Get()) ^ static_cast<schema_bits_t<T>>(val)));                  \
			return *this;                                                                                                    \
		}                                                                                                                    \
	private:                                                                                                                 \
		/*Prevent accidentally copying this wrapper class instead of the underlying field*/                                  \
		varName##_prop(const varName##_prop&) = delete;                                                                      \
		static constexpr auto m_varNameHash = hash_32_fnv1a_const(#varName);                                                 \
	} varName{};

#define SCHEMA_FIELD_POINTER_OFFSET(type, varName, extra_offset)                                                             \
	class varName##_prop                                                                                                     \
	{                                                                                                                     \
	public:                                                                                                                  \
		/*The deleted copy ctor below is user-declared, which suppresses the*/                                               \
		/*implicit default one -- without this a schema class that has a real*/                                              \
		/*constructor cannot initialise its own fields.*/                                                                    \
		varName##_prop() = default;                                                                                          \
		type* Get()                                                                                                          \
		{                                                                                                                    \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);				 \
			if (!m_key.valid) return &schema::MissingField<type>();                                            \
			static const auto m_offset = offsetof(ThisClass, varName);														 \
																															 \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                             \
                                                                                                                             \
			return reinterpret_cast<std::add_pointer_t<type>>(pThisClass + m_key.offset + extra_offset);                     \
		}                                                                                                                    \
		/*Const overload, so a const method of the owning class can read the*/                                               \
		/*field. Reading only needs the wrapper's address, which const does*/                                                \
		/*not get in the way of.*/                                                                                           \
		const type* Get() const                                                                                              \
		{                                                                                                                    \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);                 \
			if (!m_key.valid) return &schema::MissingField<const type>();                                            \
			static const auto m_offset = offsetof(ThisClass, varName);                                                          \
                                                                                                                       \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                                \
                                                                                                                       \
			return reinterpret_cast<std::add_pointer_t<const type>>(pThisClass + m_key.offset + extra_offset);                  \
		}                                                                                                                    \
		void NetworkStateChanged() /*Call this after editing the field*/                                                     \
		{                                                                                                                    \
			static const auto m_key = schema::GetOffset(m_className, m_classNameHash, #varName, m_varNameHash);				 \
			if (!m_key.valid) return;                                                                          \
			static const auto m_chain = schema::FindChainOffset(m_className, m_classNameHash);								 \
			static const auto m_offset = offsetof(ThisClass, varName);														 \
																															 \
			uintptr_t pThisClass = ((uintptr_t)this - m_offset);                                                             \
                                                                                                                             \
			if (m_chain != 0 && m_key.networked)                                                                             \
			{                                                                                                                \
				ChainNetworkStateChanged(pThisClass + m_chain, m_key.offset + extra_offset);								 \
			}                                                                                                                \
			else if (m_key.networked)                                                                                        \
			{                                                                                                                \
				if (!m_networkStateChangedOffset)                                                                            \
					EntityNetworkStateChanged(pThisClass, m_key.offset + extra_offset);									     \
				else                                                                                                         \
					NetworkVarStateChanged(pThisClass, m_key.offset + extra_offset, m_networkStateChangedOffset);			 \
			}                                                                                                                \
		}                                                                                                                    \
		operator type*()                                                                                                     \
		{                                                                                                                    \
			return Get();                                                                                                    \
		}                                                                                                                    \
		type* operator()()                                                                                                   \
		{                                                                                                                    \
			return Get();                                                                                                    \
		}                                                                                                                    \
		type* operator->()                                                                                                   \
		{                                                                                                                    \
			return Get();                                                                                                    \
		}                                                                                                                    \
		operator const type*() const                                                                                         \
		{                                                                                                                    \
			return Get();                                                                                                          \
		}                                                                                                                    \
		const type* operator()() const                                                                                       \
		{                                                                                                                    \
			return Get();                                                                                                          \
		}                                                                                                                    \
		const type* operator->() const                                                                                       \
		{                                                                                                                    \
			return Get();                                                                                                          \
		}                                                                                                                    \
	private:                                                                                                                 \
		/*Prevent accidentally copying this wrapper class instead of the underlying field*/                                  \
		varName##_prop(const varName##_prop&) = delete;                                                                      \
		static constexpr auto m_varNameHash = hash_32_fnv1a_const(#varName);                                                 \
	} varName{};

/**

* @brief Declares schema field (auto offset + network update).
*
* @note This macro generates a wrapper class that:
* * Resolves offset via schema
* * Provides Get/Set
* * Automatically updates network state
* * Use this when you want the member's value itself
    */
#define SCHEMA_FIELD(type, varName) \
	SCHEMA_FIELD_OFFSET(type, varName, 0)

/**

* @brief Declares pointer schema field.
* * Use this when you want a pointer to a member
  */
#define SCHEMA_FIELD_POINTER(type, varName) \
	SCHEMA_FIELD_POINTER_OFFSET(type, varName, 0)

#define SCHEMA_FIELD_OLD(type, className, propName)                                                    \
    std::add_lvalue_reference_t<type> propName()                                                       \
    {                                                                                                  \
        static const int32_t offset = [] {                                                             \
            const int32_t nOffset = schema::GetServerOffset(#className, #propName);                    \
            if (nOffset == -1)                                                                         \
                Warning("SCHEMA_FIELD_OLD: '" #className "::" #propName "' was not found!\n");         \
            return nOffset;                                                                            \
        }();                                                                                           \
        return *reinterpret_cast<std::add_pointer_t<type>>(reinterpret_cast<intptr_t>(this) + offset); \
    }

/* =========================
Schema class macros
========================= */

/**

* @brief Declares schema-enabled class.
* * offset != 0 marks a non-entity (embedded) class; the value itself is not used any more
  */
#define DECLARE_SCHEMA_CLASS_BASE(ClassName, offset)								\
	private:																		\
		typedef ClassName ThisClass;												\
		static constexpr const char* m_className = #ClassName;						\
		static constexpr uint32_t m_classNameHash = hash_32_fnv1a_const(#ClassName);\
		static constexpr int m_networkStateChangedOffset = offset;					\
	public:
#define DECLARE_SCHEMA_CLASS(className) DECLARE_SCHEMA_CLASS_BASE(className, 0)
/**

* @brief Declares inline schema class (non-entity).
* * Use this for non-entity classes such as CCollisionProperty or CGlowProperty
* * Their NetworkStateChanged is a virtual of the NetworkVar_<field> wrapper the engine embeds them in, not CEntityInstance's;
* * its vtable slot is found at runtime (NetworkVarStateChanged), so no index is kept per class
* * Though some classes like CGameRules will instead use their CNetworkVarChainer as a link back to the parent entity
  */
#define DECLARE_SCHEMA_CLASS_INLINE(className) DECLARE_SCHEMA_CLASS_BASE(className, 1)

#endif // SCHEMA_H
