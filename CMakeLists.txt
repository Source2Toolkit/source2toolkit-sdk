cmake_minimum_required(VERSION 3.18)
project(source2toolkit-sdk CXX)

option(SOURCE2TOOLKIT_CORE "Build core toolkit" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =========================
# PLATFORM
# =========================

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if (WIN32 AND NOT MSVC)
    message(FATAL_ERROR "MSVC restricted.")
endif()

if (LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs=libprotobuf.a")
    add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif ()

# =========================
# DEPENDENCY PATHS
# =========================

# ENV_NAMES is a list: the first variable that is set wins.
function(resolve_dep OUT_VAR ENV_NAMES)
    # Already set -- by deps.cmake, which tools/deps.py writes when a
    # dependency was chosen explicitly rather than read from the environment.
    if (DEFINED ${OUT_VAR} AND NOT "${${OUT_VAR}}" STREQUAL "")
        message(STATUS "${OUT_VAR} -> ${${OUT_VAR}} (deps.cmake)")
        return()
    endif()
    foreach (_env ${ENV_NAMES})
        if (DEFINED ENV{${_env}} AND NOT "$ENV{${_env}}" STREQUAL "")
            file(TO_CMAKE_PATH "$ENV{${_env}}" _resolved)
            set(${OUT_VAR} "${_resolved}" PARENT_SCOPE)
            message(STATUS "${_env} -> ${_resolved}")
            return()
        endif()
    endforeach()
    if (ARGC GREATER 2)
        set(_fallback "${ARGV2}")
        set(${OUT_VAR} "${_fallback}" PARENT_SCOPE)
        message(STATUS "${ENV_NAMES} not set, using fallback: ${_fallback}")
    else()
        message(FATAL_ERROR "${ENV_NAMES} is not set")
    endif()
endfunction()

# s2sdk (alliedmodders/s2sdk, the CS2 SDK that used to be hl2sdk's cs2 branch)
# can be named by any of these; the newer names come first so a stale
# HL2SDKCS2 left over from the hl2sdk days does not shadow them.
set(S2SDK_ENV_VARS S2SDK_CS2 S2SDKCS2 S2SDK HL2SDK_CS2 HL2SDKCS2)

# tools/deps.py: `init` picks, once, whether s2sdk and the Protobufs come from
# a checkout you already have (the variables above / CSGO_PROTO) or from this
# repo -- s2sdk is the vendor/s2sdk submodule, the Protobufs a clone under
# vendor/ -- and `update` keeps whichever it is current, together with
# vendor/khook, moved to the commit metamod-source pins. An explicit choice
# lands in deps.cmake and wins over the environment.
include("${CMAKE_CURRENT_SOURCE_DIR}/deps.cmake" OPTIONAL)

# Without deps.py: the environment if set, else the submodule.
resolve_dep(SOURCESDK "${S2SDK_ENV_VARS}" "${CMAKE_CURRENT_SOURCE_DIR}/vendor/s2sdk")
resolve_dep(PROTOBUFS CSGO_PROTO "${CMAKE_CURRENT_SOURCE_DIR}/vendor/Protobufs/csgo")

if (NOT EXISTS "${SOURCESDK}/public/eiface.h")
    message(FATAL_ERROR
            "s2sdk not found at ${SOURCESDK}. Set S2SDK (or HL2SDKCS2) to your checkout, "
            "or fetch the submodule: git submodule update --init vendor/s2sdk")
endif()

# A plugin that adds this SDK with add_subdirectory() gets the same path.
get_directory_property(_sdk_has_parent PARENT_DIRECTORY)
if (_sdk_has_parent)
    set(SOURCESDK "${SOURCESDK}" PARENT_SCOPE)
endif()
if (NOT EXISTS "${PROTOBUFS}/netmessages.proto")
    message(FATAL_ERROR
            "Protobufs (csgo/) not found at ${PROTOBUFS}. Set CSGO_PROTO to the csgo/ folder "
            "of a SteamDatabase/Protobufs checkout, or run `python tools/deps.py init`.")
endif()

# =========================
# KHOOK
# =========================
#
# KHook, metamod's detour library, is what the toolkit and its plugins hook
# with. Header-only on this side: the engine lives in metamod.so and reaches a
# plugin through TOOLKIT_SAVEVARS() (see IToolkitPlugin.h), so nothing here is
# built -- the headers are all the SDK needs.
#
# They come from vendor/khook, a submodule of this repo, so a toolkit plugin
# depends on the SDK alone and not on a metamod-source checkout. The one
# exception is the toolkit core itself (SOURCE2TOOLKIT_CORE): it is a metamod
# plugin, its engine is the KHook compiled into metamod, and it has to use
# that copy's headers so what it compiles against is what it runs on.
#
# TOOLKIT_KHOOK_COMMIT is the commit of whichever copy was used. The core
# answers TOOLKIT_KHOOK_VERSION_INTERFACE with its own, and TOOLKIT_SAVEVARS()
# compares the two at plugin load, so a plugin built against one KHook can
# never silently hook through another (see IToolkitPlugin.h).

if (SOURCE2TOOLKIT_CORE AND DEFINED ENV{MMSOURCE_DEV} AND NOT "$ENV{MMSOURCE_DEV}" STREQUAL "")
    file(TO_CMAKE_PATH "$ENV{MMSOURCE_DEV}" _metamod)
    set(KHOOK_DIR "${_metamod}/third_party/khook")
    message(STATUS "KHook (core build) -> ${KHOOK_DIR}")
else ()
    set(KHOOK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/khook")
endif ()

if (NOT EXISTS "${KHOOK_DIR}/include/khook.hpp")
    message(FATAL_ERROR
            "KHook headers not found at ${KHOOK_DIR}/include. "
            "Initialise the submodule: git submodule update --init --recursive")
endif ()

find_package(Git QUIET)
set(TOOLKIT_KHOOK_COMMIT "unknown")
if (GIT_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
            WORKING_DIRECTORY ${KHOOK_DIR}
            OUTPUT_VARIABLE _khook_commit
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            RESULT_VARIABLE _khook_rc
    )
    if (_khook_rc EQUAL 0 AND NOT "${_khook_commit}" STREQUAL "")
        set(TOOLKIT_KHOOK_COMMIT "${_khook_commit}")
    endif ()
endif ()
message(STATUS "KHook commit: ${TOOLKIT_KHOOK_COMMIT}")

if (SOURCE2TOOLKIT_CORE AND GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/khook/include/khook.hpp")
    # The core compiles against metamod's copy, plugins against the submodule.
    # They have to be the same commit, or every plugin fails its load check.
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/khook
            OUTPUT_VARIABLE _sdk_khook_commit
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
    if (NOT "${_sdk_khook_commit}" STREQUAL "${TOOLKIT_KHOOK_COMMIT}")
        message(WARNING
                "KHook pin drift: metamod-source has ${TOOLKIT_KHOOK_COMMIT}, "
                "the SDK submodule vendor/khook has ${_sdk_khook_commit}. "
                "Plugins built from this SDK will refuse to load on this core "
                "until both point at the same commit.")
    endif ()
endif ()

set(SOURCESDK_LIB ${SOURCESDK}/lib)

# =========================
# PROTOBUF
# =========================

include(protobuf.cmake)

# =========================
# GLOBAL CONFIG
# =========================

add_library(source2toolkit-config INTERFACE)

target_compile_definitions(source2toolkit-config INTERFACE
        META_IS_SOURCE2
        _ITERATOR_DEBUG_LEVEL=0
        X64BITS

        $<$<PLATFORM_ID:Linux>:
        _LINUX POSIX LINUX GNUC COMPILER_GCC PLATFORM_64BITS
        _FILE_OFFSET_BITS=64
        _GLIBCXX_USE_CXX11_ABI=0

        stricmp=strcasecmp
        _stricmp=strcasecmp
        strnicmp=strncasecmp
        _strnicmp=strncasecmp
        _snprintf=snprintf
        _vsnprintf=vsnprintf
        _alloca=alloca
        strcmpi=strcasecmp
        >

        $<$<PLATFORM_ID:Windows>:
        COMPILER_MSVC COMPILER_MSVC64
        _WIN32 _WINDOWS
        _ALLOW_KEYWORD_MACROS
        __STDC_LIMIT_MACROS

        _CRT_SECURE_NO_WARNINGS=1
        _CRT_SECURE_NO_DEPRECATE=1
        _CRT_NONSTDC_NO_DEPRECATE=1
        >
)

target_compile_options(source2toolkit-config INTERFACE
        $<$<PLATFORM_ID:Linux>:
        -fPIC
        -fno-strict-aliasing
        -fno-threadsafe-statics

        -Wall
        -Wno-uninitialized
        -Wno-switch
        -Wno-unused
        -Wno-non-virtual-dtor
        -Wno-overloaded-virtual
        -Wno-conversion-null
        -Wno-write-strings
        -Wno-invalid-offsetof
        -Wno-reorder

        -mfpmath=sse
        -msse
        -fvisibility=default
        >

        $<$<PLATFORM_ID:Windows>:
        /wd4819
        /wd4828
        /wd5033
        /wd4005
        /permissive-
        /utf-8
        /MP
        >
)

target_link_options(source2toolkit-config INTERFACE
        $<$<PLATFORM_ID:Linux>:
        -Wl,--exclude-libs=libprotobuf.a
        >

        $<$<PLATFORM_ID:Windows>:
        $<$<CONFIG:RELEASE>:/OPT:REF>
        $<$<CONFIG:RELEASE>:/OPT:ICF>
        $<$<CONFIG:DEBUG>:/NODEFAULTLIB:libcmt>
        >
)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

# =========================
# SDK LIB
# =========================

file(GLOB_RECURSE SOURCE_FILES
        public/*.cpp
        public/*.h
        public/*.hpp
        src/*.cpp
        src/*.h
        src/*.hpp
)

list(APPEND SOURCE_FILES
        ${SOURCESDK}/public/tier0/memoverride.cpp
        ${SOURCESDK}/tier1/bitbuf.cpp
        ${SOURCESDK}/tier1/convar.cpp
        ${SOURCESDK}/tier1/keyvalues3.cpp
        ${SOURCESDK}/entity2/entityidentity.cpp
        ${SOURCESDK}/entity2/entitysystem.cpp
        ${SOURCESDK}/entity2/entitykeyvalues.cpp
)

add_library(source2toolkit-sdk STATIC ${SOURCE_FILES})

if (SOURCE2TOOLKIT_CORE)
    target_include_directories(source2toolkit-sdk PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/../../src
    )
endif()

target_link_libraries(source2toolkit-sdk PUBLIC
        source2toolkit-config
)

target_include_directories(source2toolkit-sdk PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        ${SOURCESDK}
        ${SOURCESDK}/thirdparty/protobuf-3.21.8/src
        ${SOURCESDK}/common
        ${SOURCESDK}/game/shared
        ${SOURCESDK}/game/server
        ${SOURCESDK}/public
        ${SOURCESDK}/public/engine
        ${SOURCESDK}/public/mathlib
        ${SOURCESDK}/public/tier0
        ${SOURCESDK}/public/tier1
        ${SOURCESDK}/public/entity2
        ${SOURCESDK}/public/game/server
        ${SOURCESDK}/public/schemasystem
        ${KHOOK_DIR}/include
)

# Which KHook this binary -- and everything linking it -- was compiled
# against. Read back by TOOLKIT_SAVEVARS() and by the core's ToolkitFactory.
target_compile_definitions(source2toolkit-sdk PUBLIC
        TOOLKIT_KHOOK_COMMIT="${TOOLKIT_KHOOK_COMMIT}"
)

# =========================
# LINK SDK DEPENDENCIES
# =========================

if (LINUX)
    target_link_libraries(source2toolkit-sdk PUBLIC
            ${SOURCESDK_LIB}/linux64/libtier0.so
            ${SOURCESDK_LIB}/linux64/interfaces.a
            ${SOURCESDK_LIB}/linux64/mathlib.a
            ${SOURCESDK_LIB}/linux64/release/libprotobuf.a
            ${SOURCESDK_LIB}/linux64/libsteam_api.so
            Protobufs
    )
endif()

if (WIN32)
    target_link_libraries(source2toolkit-sdk PUBLIC
            ${SOURCESDK_LIB}/public/win64/tier0.lib
            ${SOURCESDK_LIB}/public/win64/interfaces.lib
            ${SOURCESDK_LIB}/public/win64/mathlib.lib
            ${SOURCESDK_LIB}/public/win64/2015/libprotobuf.lib
            ${SOURCESDK_LIB}/public/win64/steam_api64.lib
            Protobufs
    )
endif()

# =========================
# HELPER
# =========================

function(add_s2toolkit_plugin name)
    add_library(${name} SHARED ${ARGN})

    target_link_libraries(${name}
            source2toolkit-sdk
    )

    set_target_properties(${name} PROPERTIES
            PREFIX ""
            SUFFIX ".stx"
    )
endfunction()
