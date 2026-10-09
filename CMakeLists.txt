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

# s2sdk (alliedmodders/s2sdk, the CS2 SDK that used to be hl2sdk's cs2 branch).
# In order:
#  - SOURCESDK from deps.cmake, which tools/deps.py writes for an explicit
#    choice (the submodule, or a typed path);
#  - the first of these variables that is set -- the newer names first, so a
#    stale HL2SDKCS2 left over from the hl2sdk days does not shadow them;
#  - the vendor/s2sdk submodule.
include("${CMAKE_CURRENT_SOURCE_DIR}/deps.cmake" OPTIONAL)

if (DEFINED SOURCESDK AND NOT "${SOURCESDK}" STREQUAL "")
    message(STATUS "s2sdk -> ${SOURCESDK} (deps.cmake)")
else()
    set(SOURCESDK "${CMAKE_CURRENT_SOURCE_DIR}/vendor/s2sdk")
    foreach (_env S2SDK_CS2 S2SDKCS2 S2SDK HL2SDK_CS2 HL2SDKCS2)
        if (DEFINED ENV{${_env}} AND NOT "$ENV{${_env}}" STREQUAL "")
            file(TO_CMAKE_PATH "$ENV{${_env}}" SOURCESDK)
            break()
        endif()
    endforeach()
    message(STATUS "s2sdk -> ${SOURCESDK}")
endif()

if (NOT EXISTS "${SOURCESDK}/CMakeLists.txt")
    message(FATAL_ERROR
            "s2sdk not found at ${SOURCESDK}, or too old to have its CMake support. Set S2SDK "
            "(or HL2SDKCS2) to your checkout, or fetch the submodule: "
            "git submodule update --init --recursive vendor/s2sdk")
endif()

# A plugin that adds this SDK with add_subdirectory() gets the same path.
get_directory_property(_sdk_has_parent PARENT_DIRECTORY)
if (_sdk_has_parent)
    set(SOURCESDK "${SOURCESDK}" PARENT_SCOPE)
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

# =========================
# GLOBAL CONFIG
# =========================
#
# On top of what s2sdk::headers already defines for its headers.

add_library(source2toolkit-config INTERFACE)

target_compile_definitions(source2toolkit-config INTERFACE
        META_IS_SOURCE2
        _ITERATOR_DEBUG_LEVEL=0

        $<$<PLATFORM_ID:Linux>:
        _FILE_OFFSET_BITS=64
        _snprintf=snprintf
        >

        $<$<PLATFORM_ID:Windows>:
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
# S2SDK
# =========================
#
# Its include paths, defines and libraries as s2sdk::s2sdk, and
# s2sdk_protobufs() for the game protobufs.

if (NOT TARGET s2sdk)
    add_subdirectory(${SOURCESDK} ${CMAKE_CURRENT_BINARY_DIR}/s2sdk)
endif()
if (NOT COMMAND s2sdk_protobufs)
    message(FATAL_ERROR
            "s2sdk at ${SOURCESDK} has no s2sdk_protobufs(): update it")
endif()

# The game protobufs, generated from s2sdk's own protos into the Protobufs
# library the toolkit and everything linking it share.
add_library(Protobufs STATIC)
target_link_libraries(Protobufs PUBLIC s2sdk::s2sdk)
s2sdk_protobufs(Protobufs
        common/network_connection.proto
        common/networkbasetypes.proto
        game/shared/cs/cs_gameevents.proto
        game/shared/cs/cs_usercmd.proto
        game/shared/base_gcmessages.proto
        game/shared/econ/econ_gcmessages.proto
        common/engine_gcmessages.proto
        gcsdk/gcsdk_gcmessages.proto
        gcsdk/gcsystemmsgs.proto
        game/shared/cstrike15/cstrike15_gcmessages.proto
        game/shared/cstrike15/cstrike15_usermessages.proto
        common/source2_steam_stats.proto
        common/netmessages.proto
        networksystem/networksystem_protomessages.proto
        gcsdk/steammessages.proto
        game/shared/usercmd.proto
        game/shared/usermessages.proto
        game/shared/gameevents.proto
        game/shared/clientmessages.proto
        game/shared/te.proto
        common/valveextensions.proto
)
set_target_properties(Protobufs PROPERTIES FOLDER SDK POSITION_INDEPENDENT_CODE ON)

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

# What the toolkit headers reach into beyond s2sdk's own include paths.
target_include_directories(source2toolkit-sdk PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        ${SOURCESDK}
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

target_link_libraries(source2toolkit-sdk PUBLIC
        s2sdk::s2sdk
        s2sdk::steam_api
        Protobufs
)

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
