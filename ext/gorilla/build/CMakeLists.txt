cmake_minimum_required(VERSION 2.6)

project(Gorilla)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_COMPILER_IS_GNUCC)
  set(LINUX "1")
endif()

if(MSVC)
  SET(PLATFORM_NAME "win32")
endif()
if(APPLE)
  SET(PLATFORM_NAME "osx")
endif()
if(LINUX)
  SET(PLATFORM_NAME "linux")
endif()

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected (defaulting to Release)")
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/../bin/${PLATFORM_NAME}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/../bin/${PLATFORM_NAME}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/../bin/${PLATFORM_NAME}/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/../bin/${PLATFORM_NAME}/Release)

set(CMAKE_DEBUG_POSTFIX D)

if(APPLE)
  add_definitions(-mmacosx-version-min=10.5)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mmacosx-version-min=10.5")
  set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} -mmacosx-version-min=10.5")
  set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -mmacosx-version-min=10.5")
endif()

if(MSVC)
  set_target_properties(PROPERTIES PREFIX "../") # The Prefix Hack!
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
set(DEBUG_CONFIGURATIONS "Debug")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNICODE -D_UNICODE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE -D_UNICODE")
if(MSVC)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Gm")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Gm")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi /Gm")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /Gm")
endif()

if(MSVC)
  set(ENABLE_DIRECTSOUND "0" CACHE STRING "Enable DirectSound device")
  set(ENABLE_XAUDIO2 "0" CACHE STRING "Enable XAudio2 device")
  set(ENABLE_OPENAL "1" CACHE STRING "Enable OpenAL device")
else()
  set(ENABLE_OPENAL "1" CACHE STRING "Enable OpenAL device")
endif()

if(ENABLE_OPENAL)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_OPENAL")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_OPENAL")
  # NOTE: Linux/Mac OpenAL support not yet implemented!
  if(MSVC)
    include_directories(../ext/OpenAL/include)
  else()
    find_package(OpenAL)
    include_directories(${OPENAL_INCLUDE_DIR})
  endif()
endif()

if(ENABLE_XAUDIO2)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_XAUDIO2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_XAUDIO2")
endif()

if(ENABLE_DIRECTSOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_DIRECTSOUND")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_DIRECTSOUND")
endif()

if(ENABLE_DIRECTSOUND OR ENABLE_XAUDIO2)
  set(DIRECTXSDK_PATH "${PROJECT_BINARY_DIR}/../../DirectXSdk" CACHE FILEPATH "DirectX SDK install path")
  if(NOT EXISTS ${DIRECTXSDK_PATH})
    message(FATAL ERROR " Could not locate DIRECTXSDK_PATH - please confirm this value is correct")
  else()
    find_path(DIRECTX_INCLUDE_DIR dsound.h
      HINTS
      ${DIRECTXSDK_PATH}
      PATH_SUFFIXES Include
      PATHS
      ${DIRECTXSDK_PATH}
    )
    find_library(DIRECTX_DSOUND_LIBRARY
      NAMES dsound
      HINTS
      ${DIRECTXSDK_PATH}
      PATH_SUFFIXES Lib/x86
      PATHS
      ${DIRECTXSDK_PATH}
    )
    find_library(DIRECTX_XAUDIO2_LIBRARY
      NAMES XAPOBase
      HINTS
      ${DIRECTXSDK_PATH}
      PATH_SUFFIXES Lib/x86
      PATHS
      ${DIRECTXSDK_PATH}
    )
    include_directories(${DIRECTX_INCLUDE_DIR})
  endif()
endif()

set(INCLUDE_DIR ../include)
set(SRC_DIR ../src)
set(GORILLA_INCLUDE_DIR ${INCLUDE_DIR})

include_directories(${INCLUDE_DIR})

set(GORILLA_INCLUDE
  ${INCLUDE_DIR}/gorilla/ga.h
  ${INCLUDE_DIR}/gorilla/gau.h
  ${INCLUDE_DIR}/gorilla/ga_internal.h
  )
  
set(GORILLA_SRC
  ${SRC_DIR}/ga.c
  ${SRC_DIR}/gau.c
  ${SRC_DIR}/ga_stream.c
  )

set(GORILLA_COMMON_INCLUDE
  ${INCLUDE_DIR}/gorilla/common/gc_common.h
  ${INCLUDE_DIR}/gorilla/common/gc_thread.h
  ${INCLUDE_DIR}/gorilla/common/gc_types.h
  )
  
set(GORILLA_COMMON_SRC
  ${SRC_DIR}/common/gc_common.c
  ${SRC_DIR}/common/gc_thread.c
  )
  
# OGG/Vorbis
set(LIBOGGDIR ../ext/libogg)
set(LIBVORBISDIR ../ext/libvorbis)

include_directories(${LIBOGGDIR}/include)
include_directories(${LIBVORBISDIR}/include)
include_directories(${LIBOGGDIR}/src)
include_directories(${LIBVORBISDIR}/lib)

set(OGGVORBIS_INCLUDE
  ${LIBOGGDIR}/include/ogg/ogg.h
  ${LIBOGGDIR}/include/ogg/os_types.h
  ${LIBVORBISDIR}/include/vorbis/codec.h
  ${LIBVORBISDIR}/include/vorbis/vorbisenc.h
  ${LIBVORBISDIR}/include/vorbis/vorbisfile.h
  )
  
set(OGGVORBIS_SRC
  ${LIBOGGDIR}/src/bitwise.c
  ${LIBOGGDIR}/src/framing.c
  ${LIBVORBISDIR}/lib/analysis.c
  ${LIBVORBISDIR}/lib/bitrate.c
  ${LIBVORBISDIR}/lib/block.c
  ${LIBVORBISDIR}/lib/codebook.c
  ${LIBVORBISDIR}/lib/envelope.c
  ${LIBVORBISDIR}/lib/floor0.c
  ${LIBVORBISDIR}/lib/floor1.c
  ${LIBVORBISDIR}/lib/info.c
  ${LIBVORBISDIR}/lib/lpc.c
  ${LIBVORBISDIR}/lib/lsp.c
  ${LIBVORBISDIR}/lib/mapping0.c
  ${LIBVORBISDIR}/lib/mdct.c
  ${LIBVORBISDIR}/lib/psy.c
  ${LIBVORBISDIR}/lib/registry.c
  ${LIBVORBISDIR}/lib/res0.c
  ${LIBVORBISDIR}/lib/sharedbook.c
  ${LIBVORBISDIR}/lib/smallft.c
  ${LIBVORBISDIR}/lib/synthesis.c
  ${LIBVORBISDIR}/lib/vorbisfile.c
  ${LIBVORBISDIR}/lib/window.c
  )

if(ENABLE_OPENAL)
  list(APPEND GORILLA_INCLUDE ${INCLUDE_DIR}/gorilla/devices/ga_openal.h)
  list(APPEND GORILLA_SRC ${SRC_DIR}/devices/ga_openal.c)
endif()

if(ENABLE_DIRECTSOUND)
  list(APPEND GORILLA_INCLUDE ${INCLUDE_DIR}/gorilla/devices/ga_directsound.h)
  list(APPEND GORILLA_SRC ${SRC_DIR}/devices/ga_directsound.c)
endif()

if(ENABLE_XAUDIO2)
  list(APPEND GORILLA_INCLUDE ${INCLUDE_DIR}/gorilla/devices/ga_xaudio2.h)
  list(APPEND GORILLA_SRC ${SRC_DIR}/devices/ga_xaudio2.c)
endif()

source_group("include" FILES ${GORILLA_INCLUDE})
source_group("src" FILES ${GORILLA_SRC})
source_group("include\\common" FILES ${GORILLA_COMMON_INCLUDE})
source_group("src\\common" FILES ${GORILLA_COMMON_SRC})
source_group("include\\oggvorbis" FILES ${OGGVORBIS_INCLUDE})
source_group("src\\oggvorbis" FILES ${OGGVORBIS_SRC})

add_library(gorilla STATIC
  ${GORILLA_INCLUDE}
  ${GORILLA_SRC}
  ${GORILLA_COMMON_INCLUDE}
  ${GORILLA_COMMON_SRC}
  ${OGGVORBIS_INCLUDE}
  ${OGGVORBIS_SRC}
  )

if(ENABLE_OPENAL)
  # NOTE: Linux/Mac OpenAL support not yet implemented!
  if(MSVC)
    target_link_libraries(gorilla ${CMAKE_CURRENT_SOURCE_DIR}/../ext/OpenAL/libs/Win32/OpenAL32.lib)
  else()
    target_link_libraries(gorilla ${OPENAL_LIBRARY})
  endif()
endif()

if(ENABLE_DIRECTSOUND)
  target_link_libraries(gorilla ${DIRECTX_DSOUND_LIBRARY})
endif()

if(ENABLE_XAUDIO2)
  target_link_libraries(gorilla ${DIRECTX_XAUDIO2_LIBRARY})
endif()
