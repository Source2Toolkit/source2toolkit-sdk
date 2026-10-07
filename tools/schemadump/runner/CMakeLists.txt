# Builds schemadump_runner.so (Linux only), see runner.cpp.
# Needs MMSOURCE_DEV in the environment; s2sdk is found like the SDK itself
# does it (S2SDK_CS2 / S2SDKCS2 / S2SDK / HL2SDK_CS2 / HL2SDKCS2, else the
# SDK's vendor/s2sdk submodule).
cmake_minimum_required(VERSION 3.18)
project(schemadump_runner CXX)

if(NOT DEFINED ENV{MMSOURCE_DEV})
    message(FATAL_ERROR "MMSOURCE_DEV has to be set")
endif()
set(HL2SDK "${CMAKE_CURRENT_SOURCE_DIR}/../../../vendor/s2sdk")
foreach(_var S2SDK_CS2 S2SDKCS2 S2SDK HL2SDK_CS2 HL2SDKCS2)
    if(DEFINED ENV{${_var}} AND NOT "$ENV{${_var}}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{${_var}}" HL2SDK)
        break()
    endif()
endforeach()
if(NOT EXISTS "${HL2SDK}/public/eiface.h")
    message(FATAL_ERROR "s2sdk not found at ${HL2SDK}")
endif()
set(MMSOURCE $ENV{MMSOURCE_DEV})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

# eiface.h (through Metamod's headers) includes network_connection.pb.h.
set(PROTOBUF_SRC ${HL2SDK}/thirdparty/protobuf-3.21.8/src)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/network_connection.pb.h
    COMMAND ${HL2SDK}/devtools/bin/linux/protoc --cpp_out=${CMAKE_BINARY_DIR}
            -I${HL2SDK}/common -I${PROTOBUF_SRC} ${HL2SDK}/common/network_connection.proto
    DEPENDS ${HL2SDK}/common/network_connection.proto)

add_library(schemadump_runner SHARED runner.cpp ${HL2SDK}/tier1/convar.cpp ${CMAKE_BINARY_DIR}/network_connection.pb.h)
set_target_properties(schemadump_runner PROPERTIES PREFIX "")

target_include_directories(schemadump_runner PRIVATE
    ${CMAKE_BINARY_DIR} ${PROTOBUF_SRC}
    ${HL2SDK} ${HL2SDK}/common ${HL2SDK}/public ${HL2SDK}/public/engine ${HL2SDK}/public/entity2
    ${HL2SDK}/public/mathlib ${HL2SDK}/public/tier0 ${HL2SDK}/public/tier1
    ${HL2SDK}/game/shared ${HL2SDK}/game/server
    ${MMSOURCE}/core ${MMSOURCE}/third_party/khook/include)

target_compile_definitions(schemadump_runner PRIVATE
    META_IS_SOURCE2 PLATFORM_64BITS X64BITS COMPILER_GCC GNUC POSIX LINUX _LINUX
    _FILE_OFFSET_BITS=64 _GLIBCXX_USE_CXX11_ABI=0
    stricmp=strcasecmp _stricmp=strcasecmp strcmpi=strcasecmp
    strnicmp=strncasecmp _strnicmp=strncasecmp
    _snprintf=snprintf _vsnprintf=vsnprintf _alloca=alloca)

target_compile_options(schemadump_runner PRIVATE
    -fPIC -fno-strict-aliasing -fno-threadsafe-statics -fvisibility=hidden -msse -mfpmath=sse -w)

target_link_libraries(schemadump_runner PRIVATE
    ${HL2SDK}/lib/linux64/libtier0.so ${HL2SDK}/lib/linux64/interfaces.a ${HL2SDK}/lib/linux64/mathlib.a
    -static-libstdc++ -lgcc_eh)
