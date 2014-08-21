cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE Release)

project(cgame)

file(GLOB_RECURSE CGAME_SOURCES src/*.c src/*.h)
list(APPEND CGAME_SOURCES ext/glew/src/glew.c ext/stb/stb_image.c)

if(APPLE)
  add_definitions(-DCGAME_OSX)
elseif(UNIX)
  add_definitions(-DCGAME_LINUX)
else()
  add_definitions(-DCGAME_WINDOWS)
endif()

if(CGAME_DEBUG_WINDOW)
  add_definitions(-DCGAME_DEBUG_WINDOW)
  list(APPEND CGAME_SOURCES src/debugwin.m)
endif()

add_executable(cgame ${CGAME_SOURCES})

if(MSVC)
  add_definitions(/D _USE_MATH_DEFINES)
else()
  set(CMAKE_C_FLAGS "-std=c99")
  set(CMAKE_C_FLAGS_RELEASE "-O3")
endif()

include_directories(${PROJECT_SOURCE_DIR}/src)

add_definitions(-DLUAJIT_ENABLE_LUA52COMPAT)
add_subdirectory(ext/glfw)
add_subdirectory(ext/luajit)
add_subdirectory(ext/chipmunk)
add_subdirectory(ext/gorilla/build)
include_directories(${PROJECT_SOURCE_DIR}/ext/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/stb)
include_directories(${PROJECT_SOURCE_DIR}/ext/luajit/src)
include_directories(${PROJECT_SOURCE_DIR}/ext/chipmunk/include/chipmunk)
include_directories(${PROJECT_SOURCE_DIR}/ext/gorilla/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/dirent_win)

if(APPLE)
  set_target_properties(cgame PROPERTIES LINK_FLAGS
    "-pagezero_size 10000 -image_base 100000000")
  target_link_libraries(cgame glfw ${GLFW_LIBRARIES} libluajit
    chipmunk_static gorilla)
elseif(UNIX)
  target_link_libraries(cgame dl glfw ${GLFW_LIBRARIES} libluajit
    chipmunk_static gorilla)
else()
  target_link_libraries(cgame ws2_32.lib glfw ${GLFW_LIBRARIES} libluajit
    chipmunk_static gorilla)
endif()

#add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data/")
#add_definitions(-DUSR_DIR="${PROJECT_SOURCE_DIR}/usr/")

