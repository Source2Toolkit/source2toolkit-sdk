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
 * @file IToolkitModule.h
 * @brief Toolkit-facing module interface (mirrors DynLibUtils::CModule).
 *
 * Plugins obtain an IToolkitModule via IToolkitAPI::LoadModule() and release
 * it with IToolkitAPI::FreeModule().  The implementation inside the toolkit
 * core delegates every call 1:1 to DynLibUtils::CModule so plugins are
 * fully decoupled from the DynLibUtils headers.
 */

#ifndef _INCLUDE_ITOOLKIT_MODULE_H
#define _INCLUDE_ITOOLKIT_MODULE_H

#pragma once
#include "IToolkitMemory.h"

#include <cstddef>
#include <cstdint>

/* =========================
Windows macro collisions
========================= */

/*
 * windows.h defines GetModuleHandle as GetModuleHandleA/GetModuleHandleW,
 * which silently renames the method below -- see IToolkitNetworkMessages.h for
 * why the quiet rename is the dangerous one.
 */
#ifdef GetModuleHandle
#undef GetModuleHandle
#endif

class IToolkitModule
{
public:
    /**
     * @brief Describes a single section of a loaded module.
     */
    struct SectionInfo
    {
        uintptr_t base;
        size_t size;

        [[nodiscard]] inline bool IsValid() const noexcept { return base != 0; }
    };

    virtual ~IToolkitModule() = default;

    /* =========================
    Initialization
    ========================= */

    /**
     * @brief Initialises the module from its name (without extension).
     *
     * @param name      Module name, e.g. "server" or "engine2".
     * @param extension When true the platform extension is appended automatically.
     * @return True on success.
     */
    virtual bool InitFromName(const char* name, bool extension = false) = 0;

    /**
     * @brief Initialises the module from an address inside it.
     *
     * @param ptr Any address that belongs to the target module.
     * @return True on success.
     */
    virtual bool InitFromMemory(uintptr_t ptr) = 0;

    /* =========================
    Pattern scanning
    ========================= */

    /**
     * @brief Scans the executable section for a byte-string pattern.
     *
     * Pattern format: "AB CD ?? EF" (space-separated hex bytes, ?? = wildcard).
     *
     * @param pattern      IDA-style pattern string.
     * @param startAddress Optional address to start scanning from.
     * @return Address of the first match, or an invalid (null) IToolkitMemory.
     */
    [[nodiscard]] virtual IToolkitMemory FindPattern(const char* pattern,
                                                     IToolkitMemory startAddress = IToolkitMemory()) const = 0;

    /**
     * @brief Scans a named section for a byte-string pattern.
     *
     * @param pattern      IDA-style pattern string.
     * @param section      Section name, e.g. ".text" or ".rdata".
     * @param startAddress Optional address to start scanning from.
     * @return Address of the first match, or an invalid (null) IToolkitMemory.
     */
    [[nodiscard]] virtual IToolkitMemory FindPatternInSection(const char* pattern, const char* section,
                                                              IToolkitMemory startAddress = IToolkitMemory()) const = 0;

    /* =========================
    Symbol / VTable
    ========================= */

    /**
     * @brief Returns the address of a virtual table by mangled class name.
     *
     * @param name      Class name (undecorated unless decorated is true).
     * @param decorated Pass true when name is already a decorated symbol.
     */
    [[nodiscard]] virtual IToolkitMemory GetVirtualTableByName(const char* name, bool decorated = false) const = 0;

    /**
     * @brief Returns the address of an exported symbol by name.
     */
    [[nodiscard]] virtual IToolkitMemory GetFunctionByName(const char* name) const = 0;

    /* =========================
    Module inspection
    ========================= */

    /**
     * @brief Returns information about a named section.
     */
    [[nodiscard]] virtual SectionInfo GetSectionByName(const char* name) const = 0;

    /**
     * @brief Returns the OS-native module handle (HMODULE / void*).
     */
    [[nodiscard]] virtual void* GetModuleHandle() const = 0;

    /**
     * @brief Returns the base load address of the module.
     */
    [[nodiscard]] virtual IToolkitMemory GetModuleBase() const = 0;

    /**
     * @brief Returns the absolute filesystem path of the module.
     *
     * The pointer is valid for the lifetime of this IToolkitModule.
     */
    [[nodiscard]] virtual const char* GetModulePath() const = 0;

    /**
     * @brief Returns the filename component of the module path.
     *
     * The pointer is valid for the lifetime of this IToolkitModule.
     */
    [[nodiscard]] virtual const char* GetModuleName() const = 0;

    /* =========================
    Secondary virtual tables
    ========================= */

    // Appended after everything above, so a plugin built against an older SDK
    // keeps its slots. A plugin calling these needs a core that has them.

    /**
     * @brief Returns the virtual table of the sub-object at an offset inside a class.
     *
     * A class with several bases has one table per base that is not at offset 0;
     * offset 0 is the primary table, the one GetVirtualTableByName returns. The
     * offset moves whenever the class is laid out anew -- prefer GetVirtualTableByBase.
     *
     * @param name      Class name (undecorated unless decorated is true).
     * @param offset    Offset of the sub-object in bytes.
     * @param decorated Pass true when name is already a decorated symbol.
     */
    [[nodiscard]] virtual IToolkitMemory GetVirtualTableByOffset(const char* name, std::ptrdiff_t offset, bool decorated = false) const = 0;

    /**
     * @brief Returns the virtual table of a base class sub-object, located through the RTTI.
     *
     * For an interface that is not a class's first base, whose table
     * GetVirtualTableByName cannot reach. Non-virtual bases only.
     *
     * @param name      Class name (undecorated unless decorated is true).
     * @param baseName  Base class name.
     * @param decorated Pass true when both names are already decorated symbols.
     */
    [[nodiscard]] virtual IToolkitMemory GetVirtualTableByBase(const char* name, const char* baseName, bool decorated = false) const = 0;

    /* =========================
    Virtual constructor (New)
    ========================= */

    /**
     * @brief Creates a module wrapper by name (without extension).
     *
     * Equivalent to `g_ToolkitAPI->LoadModule(name)`. Release with `delete`
     * or `g_ToolkitAPI->FreeModule()`.
     *
     * @param name Module name, e.g. "server" or "tier0".
     */
    static IToolkitModule* New(const char* name);

    /**
     * @brief Creates a module wrapper from an address that belongs to the module.
     *
     * @param ptr Any address inside the target module.
     */
    static IToolkitModule* New(uintptr_t ptr);

    /**
     * @brief Creates a module wrapper from a void pointer inside the module.
     */
    static IToolkitModule* New(void* ptr);

    /**
     * @brief Creates a module wrapper from an IToolkitMemory address inside the module.
     */
    static IToolkitModule* New(IToolkitMemory mem);
};

/**
 * @brief The Windows or the Linux one of two things -- a pattern, an offset,
 * a library name -- picked at compile time.
 *
 * @code
 * pServer->FindPattern(WIN_LINUX("48 89 7C 24 ? 55", "55 48 89 E5 41 57"));
 * @endcode
 *
 * Prefer a gamedata entry for anything the game moves: that is updated
 * without a rebuild. This is for a pattern a plugin keeps to itself.
 */
#ifndef WIN_LINUX
#ifdef _WIN32
#define WIN_LINUX(win, lin) win
#else
#define WIN_LINUX(win, lin) lin
#endif
#endif

#define LOAD_MODULE(name)       g_ToolkitAPI->LoadModule(name)
#define LOAD_MODULE_MEM(ptr)    g_ToolkitAPI->LoadModuleFromMemory(ptr)
#define FREE_MODULE(mod)        g_ToolkitAPI->FreeModule(mod)

#endif // _INCLUDE_ITOOLKIT_MODULE_H
