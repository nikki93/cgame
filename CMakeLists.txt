cmake_minimum_required(VERSION 2.8)

project(cgame)

add_executable(cgame main.c vec2.c mat3.c game.c entity.c system.c entitymap.c
    pool.c saveload.c script.c transform.c camera.c sprite.c
    test/keyboard_controlled.c test/test.c
    
    ext/glew/src/glew.c ext/stb/stb_image.c)

if(MSVC)
    add_definitions(/D _USE_MATH_DEFINES)
else()
    add_definitions(-std=c99 -O3)
endif()

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(ext/glfw)
add_subdirectory(ext/luajit)
include_directories(${PROJECT_SOURCE_DIR}/ext/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/stb)
include_directories(${PROJECT_SOURCE_DIR}/ext/luajit/src)

if(APPLE)
    set_target_properties(cgame PROPERTIES LINK_FLAGS
        "-pagezero_size 10000 -image_base 100000000")
    target_link_libraries(cgame glfw ${GLFW_LIBRARIES} libluajit)
elseif(UNIX)
    target_link_libraries(cgame dl glfw ${GLFW_LIBRARIES} libluajit)
else()
    target_link_libraries(cgame ws2_32.lib glfw ${GLFW_LIBRARIES} libluajit)
endif()

#add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data/")
#add_definitions(-DUSR_DIR="${PROJECT_SOURCE_DIR}/usr/")

