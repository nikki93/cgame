cmake_minimum_required(VERSION 2.8)

project(cgame)

add_executable(cgame src/main.c src/vec2.c src/mat3.c src/game.c src/entity.c
    src/system.c src/entitymap.c src/array.c src/saveload.c src/script.c
    src/transform.c src/camera.c src/sprite.c

    src/test/keyboard_controlled.c src/test/test.c
    
    ext/glew/src/glew.c ext/stb/stb_image.c)

if(MSVC)
    add_definitions(/D _USE_MATH_DEFINES)
else()
    add_definitions(-std=c99)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src)

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

