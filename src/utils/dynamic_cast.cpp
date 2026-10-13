/**
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

// dynamic_cast that works on engine objects.
//
// The compiler turns dynamic_cast into a call to the C++ runtime --
// __dynamic_cast (Itanium, Linux) or __RTDynamicCast (MSVC). Both runtimes
// look the target up in the RTTI of the object's own binary and compare types
// by name, so in principle a cast on a server.dll / libserver.so object works.
// What breaks it is that they also trust the *plugin's* view of the class: the
// source type and the static offset hint come from the SDK hierarchy, which
// is a schema-only stand-in (no CAI_ExpresserHost<>, CAI_ExpresserSink, ...),
// and MSVC tells class from struct in the name (.?AV vs .?AU).
//
// This file replaces those runtime entry points for every binary built with
// the SDK. The cast reads the real hierarchy from the object's RTTI, finds the
// target by name (ignoring class/struct and the SDK's offset hint), and caches
// the result per (vtable, source type, target type) -- after the first cast of
// a given class it is one hash lookup. Plain dynamic_cast in plugin code, no
// helper to call. Before a cast is computed, its vtable and RTTI records must
// lie in the read-only data of a loaded binary (sections from IToolkitModule),
// so a dangling or foreign pointer fails instead of being walked.
//
// The symbols are hidden (Linux) / per-binary (Windows), so every plugin gets
// its own copy and nothing outside it is affected. Define
// SOURCE2TOOLKIT_NATIVE_DYNAMIC_CAST to build with the compiler's runtime
// instead.

#ifndef SOURCE2TOOLKIT_NATIVE_DYNAMIC_CAST

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <typeinfo>
#include <vector>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitMemory.h"
#include "source2toolkit/IToolkitModule.h"
#include "source2toolkit/IToolkitPlugin.h"
TOOLKIT_GLOBALVARS();

namespace
{
    constexpr ptrdiff_t kCastFails = PTRDIFF_MIN;

    constexpr size_t kNoParent = SIZE_MAX;

    // One base subobject of the object being cast, in pre-order: the whole
    // object first, then each base followed by its own bases. [index, end) is
    // the subtree of a node.
    struct RttiNode
    {
        const char* name;
        ptrdiff_t offset;   // from the start of the whole object
        size_t parent;      // the class this one is a direct base of, kNoParent for the whole object
        bool edgePublic;    // a public base of its parent
        bool isPublic;      // reachable from the whole object through public bases only
        size_t end;
    };

    // Type names as the two ABIs spell them. Itanium marks types that must be
    // compared by address with a leading '*', MSVC prefixes .?AV (class) or
    // .?AU (struct) -- neither may make an engine type differ from the SDK's.
    const char* PlainName(const char* name)
    {
#ifdef _WIN32
        if (name[0] == '.' && name[1] == '?' && name[2] == 'A' && (name[3] == 'V' || name[3] == 'U'))
            return name + 4;
#else
        if (name[0] == '*')
            return name + 1;
#endif
        return name;
    }

    bool SameType(const char* a, const char* b)
    {
        return a == b || std::strcmp(PlainName(a), PlainName(b)) == 0;
    }

    // Whether node `base` is a public base of node `derived` (or the same).
    bool IsPublicBaseOf(const std::vector<RttiNode>& nodes, size_t base, size_t derived)
    {
        for (size_t k = base; k != derived; k = nodes[k].parent)
        {
            if (!nodes[k].edgePublic)
                return false;
        }
        return true;
    }

    // The language rules, on the real hierarchy: first a downcast -- the one
    // target the source subobject is a public base of -- then a cross cast --
    // the source and the target both public bases of the whole object, the
    // target a unique one. Returns the target's offset in the whole object,
    // or kCastFails.
    ptrdiff_t ResolveCast(const std::vector<RttiNode>& nodes, const char* dstName, const char* srcName, ptrdiff_t srcOffset)
    {
        ptrdiff_t found = kCastFails;
        bool ambiguous = false;

        for (size_t i = 0; i < nodes.size(); i++)
        {
            if (!SameType(nodes[i].name, dstName))
                continue;

            for (size_t j = i; j < nodes[i].end; j++)
            {
                if (nodes[j].offset != srcOffset || !SameType(nodes[j].name, srcName) || !IsPublicBaseOf(nodes, j, i))
                    continue;

                if (found == kCastFails)
                    found = nodes[i].offset;
                else if (found != nodes[i].offset)
                    ambiguous = true;
                break;
            }
        }

        if (found != kCastFails && !ambiguous)
            return found;

        // A source type the binary does not know by that name (an SDK-only
        // stand-in) does not block the cross cast; a known but non-public one
        // does.
        bool srcKnown = false, srcPublic = false;
        for (const RttiNode& node : nodes)
        {
            if (node.offset == srcOffset && SameType(node.name, srcName))
            {
                srcKnown = true;
                srcPublic |= node.isPublic;
            }
        }
        if (srcKnown && !srcPublic)
            return kCastFails;

        // A virtual base shows up once per path to it, at the same offset --
        // only different offsets make the target ambiguous.
        found = kCastFails;
        for (const RttiNode& node : nodes)
        {
            if (!node.isPublic || !SameType(node.name, dstName))
                continue;

            if (found == kCastFails)
                found = node.offset;
            else if (found != node.offset)
                return kCastFails;
        }

        return found;
    }

    // Results per (vtable, source type, target type), as the delta from the
    // source pointer to the result. The layout behind a vtable is fixed, so a
    // result never goes stale while the binary owning the vtable is loaded.
    //
    // Open addressing over a static array: zero-initialised, so it is usable
    // before any constructor runs and needs no destructor at unload, and
    // readers take no lock. A slot is claimed with a CAS on its state, filled,
    // then published; a slot still being filled reads as a miss. A full table
    // only means the cast is computed every time.
    struct CastCacheEntry
    {
        std::atomic<uint32_t> state; // 0 empty, 1 being filled, 2 ready
        const void* vtable;
        const void* srcType;
        const void* dstType;
        ptrdiff_t delta;
    };

    constexpr size_t kCastCacheSize = 4096;
    constexpr size_t kCastCacheProbes = 32;
    CastCacheEntry g_CastCache[kCastCacheSize];

    size_t CastCacheSlot(const void* vtable, const void* srcType, const void* dstType)
    {
        uint64_t h = reinterpret_cast<uintptr_t>(vtable);
        h = (h ^ (reinterpret_cast<uintptr_t>(srcType) * 0x9E3779B97F4A7C15ull)) * 0xBF58476D1CE4E5B9ull;
        h = (h ^ (reinterpret_cast<uintptr_t>(dstType) * 0x94D049BB133111EBull)) * 0x9E3779B97F4A7C15ull;
        return static_cast<size_t>(h >> 32) & (kCastCacheSize - 1);
    }

    bool CastCacheFind(const void* vtable, const void* srcType, const void* dstType, ptrdiff_t& delta)
    {
        size_t slot = CastCacheSlot(vtable, srcType, dstType);
        for (size_t probe = 0; probe < kCastCacheProbes; probe++, slot = (slot + 1) & (kCastCacheSize - 1))
        {
            const CastCacheEntry& entry = g_CastCache[slot];
            const uint32_t state = entry.state.load(std::memory_order_acquire);
            if (state == 0)
                return false;
            if (state == 2 && entry.vtable == vtable && entry.srcType == srcType && entry.dstType == dstType)
            {
                delta = entry.delta;
                return true;
            }
        }
        return false;
    }

    void CastCacheStore(const void* vtable, const void* srcType, const void* dstType, ptrdiff_t delta)
    {
        size_t slot = CastCacheSlot(vtable, srcType, dstType);
        for (size_t probe = 0; probe < kCastCacheProbes; probe++, slot = (slot + 1) & (kCastCacheSize - 1))
        {
            CastCacheEntry& entry = g_CastCache[slot];
            uint32_t expected = 0;
            if (entry.state.compare_exchange_strong(expected, 1, std::memory_order_acq_rel))
            {
                entry.vtable = vtable;
                entry.srcType = srcType;
                entry.dstType = dstType;
                entry.delta = delta;
                entry.state.store(2, std::memory_order_release);
                return;
            }
            // Another thread got there first with the same cast -- done.
            if (expected == 2 && entry.vtable == vtable && entry.srcType == srcType && entry.dstType == dstType)
                return;
        }
    }

    // Whether the address lies in some loaded binary at all.
    bool InLoadedModule(IToolkitMemory address)
    {
#ifdef _WIN32
        HMODULE module = nullptr;
        return GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                  address.RCast<LPCSTR>(), &module) && module;
#else
        Dl_info info;
        return dladdr(address.GetPtr(), &info) && info.dli_fbase;
#endif
    }

    // The read-only data sections of the binaries casts have met -- where
    // every vtable, object locator and type_info lives. Each binary is looked
    // up once through IToolkitModule (DynLibUtils' section table, in the core)
    // and only the ranges are kept, the wrapper is freed right away.
    // Append-only and read without a lock, like the cast cache. A binary
    // unloaded later leaves its ranges behind; only a dangling object could
    // point there.
#ifdef _WIN32
    constexpr const char* kRttiSectionNames[] = { ".rdata" };
#else
    constexpr const char* kRttiSectionNames[] = { ".data.rel.ro", ".data.rel.ro.local", ".rodata" };
#endif

    struct RttiSection
    {
        std::atomic<uintptr_t> begin; // published last, 0 until then
        uintptr_t end;
    };

    constexpr size_t kRttiSectionsMax = 256;
    RttiSection g_RttiSections[kRttiSectionsMax];
    std::atomic<size_t> g_RttiSectionCount;
    std::atomic_flag g_RttiSectionsLock = ATOMIC_FLAG_INIT;

    // Set while this thread is inside the module lookup, which runs core code
    // that may cast as well -- such a cast takes the plain loaded-binary check.
    thread_local bool t_InModuleLookup;

    bool InKnownRttiSection(IToolkitMemory address)
    {
        const size_t count = g_RttiSectionCount.load(std::memory_order_acquire);
        for (size_t i = 0; i < count; i++)
        {
            const uintptr_t begin = g_RttiSections[i].begin.load(std::memory_order_acquire);
            if (begin && address.GetAddr() >= begin && address.GetAddr() < g_RttiSections[i].end)
                return true;
        }
        return false;
    }

    // Records the RTTI sections of the binary holding the address. False when
    // the core cannot describe that binary.
    bool AddRttiSectionsOf(IToolkitMemory address)
    {
        t_InModuleLookup = true;

        bool described = false;
        if (IToolkitModule* module = IToolkitModule::New(address))
        {
            while (g_RttiSectionsLock.test_and_set(std::memory_order_acquire))
                ;

            for (const char* name : kRttiSectionNames)
            {
                const IToolkitModule::SectionInfo section = module->GetSectionByName(name);
                if (!section.IsValid())
                    continue;

                described = true;
                const size_t count = g_RttiSectionCount.load(std::memory_order_relaxed);
                if (count == kRttiSectionsMax || InKnownRttiSection(section.base))
                    continue;

                g_RttiSections[count].end = section.base + section.size;
                g_RttiSections[count].begin.store(section.base, std::memory_order_release);
                g_RttiSectionCount.store(count + 1, std::memory_order_release);
            }

            g_RttiSectionsLock.clear(std::memory_order_release);
            g_ToolkitAPI->FreeModule(module);
        }

        t_InModuleLookup = false;
        return described;
    }

    // Whether the address is RTTI data of a loaded binary. Checked only when a
    // cast is computed, never on cache hits. Before the plugin is loaded
    // (static initialisers run before Load) there is no core to ask, and only
    // being inside some binary is checked.
    bool IsRttiAddress(IToolkitMemory address)
    {
        if (!address)
            return false;
        if (InKnownRttiSection(address))
            return true;
        if (!InLoadedModule(address))
            return false;
        if (!g_ToolkitAPI || t_InModuleLookup)
            return true;
        if (!AddRttiSectionsOf(address))
            return true; // in a binary the core cannot read -- trust it, as before
        return InKnownRttiSection(address);
    }
}

#ifdef _WIN32

// MSVC x64 RTTI, every reference an RVA from the image base.
namespace
{
    struct RttiTypeDescriptor
    {
        const void* vftable;
        void* spare;
        char name[1];
    };

    struct RttiPMD
    {
        int32_t mdisp;
        int32_t pdisp;
        int32_t vdisp;
    };

    struct RttiBaseClassDescriptor
    {
        uint32_t typeDescriptor;
        uint32_t numContainedBases;
        RttiPMD where;
        uint32_t attributes;
        uint32_t classDescriptor;
    };

    struct RttiClassHierarchyDescriptor
    {
        uint32_t signature;
        uint32_t attributes;
        uint32_t numBaseClasses;
        uint32_t baseClassArray;
    };

    struct RttiCompleteObjectLocator
    {
        uint32_t signature; // 1 on x64
        uint32_t offset;    // of this vfptr in the whole object
        uint32_t cdOffset;  // of the vtordisp before the vfptr, 0 when none
        uint32_t typeDescriptor;
        uint32_t classDescriptor;
        uint32_t self;
    };

    constexpr uint32_t BCD_NOTVISIBLE = 0x01;     // not a public base of the whole object
    constexpr uint32_t BCD_PRIVORPROTBASE = 0x04; // a private or protected base of its parent

    // The locator sits in the slot before the first virtual function.
    const RttiCompleteObjectLocator* GetLocator(IToolkitMemory object)
    {
        return object.Deref().Offset(-0x8).Deref().RCast<const RttiCompleteObjectLocator*>();
    }

    char* FindCompleteObject(IToolkitMemory object)
    {
        const RttiCompleteObjectLocator* locator = GetLocator(object);
        IToolkitMemory complete = object.Offset(-static_cast<ptrdiff_t>(locator->offset));
        if (locator->cdOffset)
            complete.OffsetSelf(-object.Offset(-static_cast<ptrdiff_t>(locator->cdOffset)).GetValue<int32_t>());
        return complete.RCast<char*>();
    }

    template<typename T>
    const T* FromRva(uintptr_t imageBase, uint32_t rva)
    {
        return IToolkitMemory(imageBase).Offset(rva).RCast<const T*>();
    }

    ptrdiff_t PMDtoOffset(const char* complete, const RttiPMD& where)
    {
        ptrdiff_t offset = 0;
        if (where.pdisp >= 0)
        {
            // A virtual base: its offset is in the vbtable of the subobject at pdisp.
            offset = where.pdisp;
            offset += IToolkitMemory(complete).Offset(offset).Deref().Offset(where.vdisp).GetValue<int32_t>();
        }
        return offset + where.mdisp;
    }

    // kCastFails, or the delta from inptr to the target.
    ptrdiff_t ComputeCast(void* inptr, long vfDelta, const RttiTypeDescriptor* srcType, const RttiTypeDescriptor* dstType)
    {
        const IToolkitMemory vtable = IToolkitMemory(inptr).Deref();
        if (!IsRttiAddress(vtable))
            return kCastFails;

        const RttiCompleteObjectLocator* locator = GetLocator(inptr);
        if (!IsRttiAddress(locator) || locator->signature != 1)
            return kCastFails;

        const uintptr_t imageBase = IToolkitMemory(locator).Offset(-static_cast<ptrdiff_t>(locator->self)).GetAddr();
        const auto* hierarchy = FromRva<RttiClassHierarchyDescriptor>(imageBase, locator->classDescriptor);
        const auto* baseArray = FromRva<uint32_t>(imageBase, hierarchy->baseClassArray);

        char* complete = FindCompleteObject(inptr);
        const ptrdiff_t srcOffset = (static_cast<char*>(inptr) - vfDelta) - complete;

        // The base array already is the pre-order walk, each entry followed by
        // the numContainedBases entries of its own bases.
        std::vector<RttiNode> nodes;
        nodes.reserve(hierarchy->numBaseClasses);
        for (uint32_t i = 0; i < hierarchy->numBaseClasses; i++)
        {
            const auto* base = FromRva<RttiBaseClassDescriptor>(imageBase, baseArray[i]);

            // The parent is the innermost earlier entry whose subtree still
            // covers this one.
            size_t parent = i ? i - 1 : kNoParent;
            while (parent != kNoParent && nodes[parent].end <= i)
                parent = nodes[parent].parent;

            nodes.push_back({
                FromRva<RttiTypeDescriptor>(imageBase, base->typeDescriptor)->name,
                PMDtoOffset(complete, base->where),
                parent,
                (base->attributes & BCD_PRIVORPROTBASE) == 0,
                (base->attributes & BCD_NOTVISIBLE) == 0,
                i + 1 + static_cast<size_t>(base->numContainedBases),
            });
        }

        const ptrdiff_t dstOffset = ResolveCast(nodes, dstType->name, srcType->name, srcOffset);
        if (dstOffset == kCastFails)
            return kCastFails;

        return (complete + dstOffset) - static_cast<char*>(inptr);
    }
}

// Every entry point of the runtime's rtti.obj is defined here, so the linker
// never pulls that object in and the definitions cannot clash.

extern "C" void* __cdecl __RTDynamicCast(void* inptr, long vfDelta, void* srcType, void* targetType, int isReference) noexcept(false)
{
    if (!inptr)
        return nullptr;

    const void* vtable = *static_cast<const void* const*>(inptr);

    ptrdiff_t delta;
    if (!CastCacheFind(vtable, srcType, targetType, delta))
    {
        delta = ComputeCast(inptr, vfDelta, static_cast<const RttiTypeDescriptor*>(srcType), static_cast<const RttiTypeDescriptor*>(targetType));
        if (delta != kCastFails || IsRttiAddress(vtable))
            CastCacheStore(vtable, srcType, targetType, delta);
    }

    if (delta == kCastFails)
    {
        if (isReference)
            throw std::bad_cast();
        return nullptr;
    }

    return static_cast<char*>(inptr) + delta;
}

extern "C" void* __cdecl __RTCastToVoid(void* inptr) noexcept(false)
{
    return inptr ? FindCompleteObject(inptr) : nullptr;
}

extern "C" void* __cdecl __RTtypeid(void* inptr) noexcept(false)
{
    if (!inptr)
        throw std::bad_typeid();

    const RttiCompleteObjectLocator* locator = GetLocator(inptr);
    const uintptr_t imageBase = IToolkitMemory(locator).Offset(-static_cast<ptrdiff_t>(locator->self)).GetAddr();
    return const_cast<RttiTypeDescriptor*>(FromRva<RttiTypeDescriptor>(imageBase, locator->typeDescriptor));
}

#else

// Itanium C++ ABI type_info, read as raw records: the layout is the ABI's, the
// vtables of the records belong to whichever runtime built the binary.
namespace
{
    struct ItaniumTypeInfo
    {
        const void* vtable;
        const char* name;
    };

    struct ItaniumSiTypeInfo : ItaniumTypeInfo
    {
        const ItaniumTypeInfo* base;
    };

    struct ItaniumBaseInfo
    {
        const ItaniumTypeInfo* type;
        long offsetFlags; // offset << 8 | public 2 | virtual 1
    };

    struct ItaniumVmiTypeInfo : ItaniumTypeInfo
    {
        uint32_t flags;
        uint32_t baseCount;
        ItaniumBaseInfo bases[1];
    };

    enum class TypeInfoKind
    {
        Class, // no bases
        Si,    // one public non-virtual base at offset 0
        Vmi,   // anything else
    };

    // Which record a type_info is, from the name of the type_info's own type
    // -- no symbol or signature of the owning runtime needed.
    TypeInfoKind GetKind(const ItaniumTypeInfo* type)
    {
        const auto* meta = IToolkitMemory(type->vtable).Offset(-0x8).Deref().RCast<const ItaniumTypeInfo*>();
        const char* name = meta->name;
        if (std::strcmp(name, "N10__cxxabiv120__si_class_type_infoE") == 0)
            return TypeInfoKind::Si;
        if (std::strcmp(name, "N10__cxxabiv121__vmi_class_type_infoE") == 0)
            return TypeInfoKind::Vmi;
        return TypeInfoKind::Class;
    }

    void WalkBases(const ItaniumTypeInfo* type, const char* whole, ptrdiff_t offset, size_t parent, bool edgePublic, std::vector<RttiNode>& nodes)
    {
        const size_t index = nodes.size();
        const bool isPublic = edgePublic && (parent == kNoParent || nodes[parent].isPublic);
        nodes.push_back({type->name, offset, parent, edgePublic, isPublic, 0});

        switch (GetKind(type))
        {
        case TypeInfoKind::Si:
            WalkBases(static_cast<const ItaniumSiTypeInfo*>(type)->base, whole, offset, index, true, nodes);
            break;

        case TypeInfoKind::Vmi:
        {
            const auto* vmi = static_cast<const ItaniumVmiTypeInfo*>(type);
            for (uint32_t i = 0; i < vmi->baseCount; i++)
            {
                const ItaniumBaseInfo& base = vmi->bases[i];
                ptrdiff_t baseOffset = base.offsetFlags >> 8;
                if (base.offsetFlags & 1)
                {
                    // A virtual base: the offset is a slot in this
                    // subobject's vtable that holds the real offset.
                    baseOffset = IToolkitMemory(whole).Offset(offset).Deref().Offset(baseOffset).GetValue<ptrdiff_t>();
                }
                WalkBases(base.type, whole, offset + baseOffset, index, (base.offsetFlags & 2) != 0, nodes);
            }
            break;
        }

        case TypeInfoKind::Class:
            break;
        }

        nodes[index].end = nodes.size();
    }

    // kCastFails, or the delta from src to the target.
    ptrdiff_t ComputeCast(const void* src, const ItaniumTypeInfo* srcType, const ItaniumTypeInfo* dstType)
    {
        // The vtable prefix: offset to the whole object at -0x10, its type_info at -0x8.
        const IToolkitMemory vtable = IToolkitMemory(src).Deref();
        if (!IsRttiAddress(vtable))
            return kCastFails;

        const ptrdiff_t offsetToTop = vtable.Offset(-0x10).GetValue<ptrdiff_t>();
        const auto* wholeType = vtable.Offset(-0x8).Deref().RCast<const ItaniumTypeInfo*>();
        if (!IsRttiAddress(wholeType))
            return kCastFails;

        const char* whole = static_cast<const char*>(src) + offsetToTop;

        std::vector<RttiNode> nodes;
        nodes.reserve(16);
        WalkBases(wholeType, whole, 0, kNoParent, true, nodes);

        const ptrdiff_t dstOffset = ResolveCast(nodes, dstType->name, srcType->name, -offsetToTop);
        if (dstOffset == kCastFails)
            return kCastFails;

        return dstOffset + offsetToTop;
    }
}

// Hidden: calls from this binary bind to it at link time, and nothing outside
// it -- the server, other plugins -- ever sees it. The compiler checks the
// null pointer before it calls.
extern "C" __attribute__((visibility("hidden"))) void* __dynamic_cast(const void* src, const void* srcType, const void* dstType, ptrdiff_t /*src2dst*/)
{
    const void* vtable = *static_cast<const void* const*>(src);

    ptrdiff_t delta;
    if (!CastCacheFind(vtable, srcType, dstType, delta))
    {
        delta = ComputeCast(src, static_cast<const ItaniumTypeInfo*>(srcType), static_cast<const ItaniumTypeInfo*>(dstType));
        if (delta != kCastFails || IsRttiAddress(vtable))
            CastCacheStore(vtable, srcType, dstType, delta);
    }

    if (delta == kCastFails)
        return nullptr;

    return const_cast<char*>(static_cast<const char*>(src)) + delta;
}

#endif

#endif // SOURCE2TOOLKIT_NATIVE_DYNAMIC_CAST
