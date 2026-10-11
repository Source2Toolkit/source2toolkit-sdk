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
 * @file IToolkitMemory.h
 * @brief Toolkit-facing memory address wrapper (mirrors DynLibUtils::CMemory).
 *
 * Plugins use this value type instead of DynLibUtils::CMemory directly so
 * they remain decoupled from the DynLibUtils headers.  The implementation
 * is identical: every method delegates 1:1 to the underlying arithmetic.
 */

#ifndef _INCLUDE_ITOOLKIT_MEMORY_H
#define _INCLUDE_ITOOLKIT_MEMORY_H

#pragma once
#include <cstddef>
#include <cstdint>
#include <utility>

class IToolkitMemory
{
public:
    IToolkitMemory() : m_ptr(0) {}
    IToolkitMemory(const IToolkitMemory&) noexcept = default;
    IToolkitMemory& operator=(const IToolkitMemory&) noexcept = default;
    IToolkitMemory(IToolkitMemory&& other) noexcept : m_ptr(std::exchange(other.m_ptr, 0)) {}
    IToolkitMemory(const uintptr_t ptr) : m_ptr(ptr) {}
    IToolkitMemory(const void* ptr) : m_ptr(reinterpret_cast<uintptr_t>(ptr)) {}

    inline operator uintptr_t() const noexcept
    {
        return m_ptr;
    }

    inline operator void*() const noexcept
    {
        return reinterpret_cast<void*>(m_ptr);
    }

    explicit inline operator bool() const noexcept
    {
        return m_ptr != 0;
    }

    inline bool operator!=(const IToolkitMemory& addr) const noexcept
    {
        return m_ptr != addr.m_ptr;
    }

    inline bool operator==(const IToolkitMemory& addr) const noexcept
    {
        return m_ptr == addr.m_ptr;
    }

    inline bool operator==(const uintptr_t& addr) const noexcept
    {
        return m_ptr == addr;
    }

    /// The address as a pointer.
    [[nodiscard]] inline void* GetPtr() const noexcept
    {
        return reinterpret_cast<void*>(m_ptr);
    }

    /// The same address as an integer.
    [[nodiscard]] inline uintptr_t GetAddr() const noexcept
    {
        return m_ptr;
    }

    template<class T> [[nodiscard]] inline T GetValue() const noexcept
    {
        return *reinterpret_cast<T*>(m_ptr);
    }

    template<typename T> [[nodiscard]] inline T CCast() const noexcept
    {
        return (T)m_ptr;
    }

    template<typename T> [[nodiscard]] inline T RCast() const noexcept
    {
        return reinterpret_cast<T>(m_ptr);
    }

    template<typename T> [[nodiscard]] inline T UCast() const noexcept
    {
        union { uintptr_t m_ptr; T cptr; } cast;
        return cast.m_ptr = m_ptr, cast.cptr;
    }

    [[nodiscard]] inline IToolkitMemory Offset(ptrdiff_t offset) const noexcept
    {
        return m_ptr + offset;
    }

    inline IToolkitMemory& OffsetSelf(ptrdiff_t offset) noexcept
    {
        m_ptr += offset;
        return *this;
    }

    [[nodiscard]] inline IToolkitMemory Deref(int deref = 1) const
    {
        uintptr_t reference = m_ptr;

        while (deref--)
        {
            if (reference)
                reference = *reinterpret_cast<uintptr_t*>(reference);
        }

        return reference;
    }

    inline IToolkitMemory& DerefSelf(int deref = 1)
    {
        while (deref--)
        {
            if (m_ptr)
                m_ptr = *reinterpret_cast<uintptr_t*>(m_ptr);
        }

        return *this;
    }

    [[nodiscard]] inline IToolkitMemory FollowNearCall(const ptrdiff_t opcodeOffset = 0x1, const ptrdiff_t nextInstructionOffset = 0x5) const
    {
        return ResolveRelativeAddress(opcodeOffset, nextInstructionOffset);
    }

    inline IToolkitMemory& FollowNearCallSelf(const ptrdiff_t opcodeOffset = 0x1, const ptrdiff_t nextInstructionOffset = 0x5)
    {
        return ResolveRelativeAddressSelf(opcodeOffset, nextInstructionOffset);
    }

    [[nodiscard]] inline IToolkitMemory ResolveRelativeAddress(const ptrdiff_t registerOffset = 0x0, const ptrdiff_t nextInstructionOffset = 0x4) const
    {
        const uintptr_t skipRegister = m_ptr + registerOffset;
        const int32_t relativeAddress = *reinterpret_cast<int32_t*>(skipRegister);
        const uintptr_t nextInstruction = m_ptr + nextInstructionOffset;
        return nextInstruction + relativeAddress;
    }

    inline IToolkitMemory& ResolveRelativeAddressSelf(const ptrdiff_t registerOffset = 0x0, const ptrdiff_t nextInstructionOffset = 0x4)
    {
        const uintptr_t skipRegister = m_ptr + registerOffset;
        const int32_t relativeAddress = *reinterpret_cast<int32_t*>(skipRegister);
        const uintptr_t nextInstruction = m_ptr + nextInstructionOffset;
        m_ptr = nextInstruction + relativeAddress;
        return *this;
    }

private:
    uintptr_t m_ptr;
};

#endif // _INCLUDE_ITOOLKIT_MEMORY_H
