set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.10)
project(sdl_opengl32_simple_demo)

# SDL库的路径，请根据实际情况修改
set(SDL2_PATH "D:/project/c/SDL2-2.28.0")

# 找到 SDL2
list(APPEND CMAKE_PREFIX_PATH ${SDL2_PATH})
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(sos main.c)

# Build DLL
add_library(steam_wrapper SHARED steam_wrapper.c)
set_target_properties(steam_wrapper PROPERTIES PREFIX "")
add_custom_command(TARGET steam_wrapper POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:steam_wrapper>
        D:/program/Steam/steamapps/common/DeathRoadToCanada)

# 链接 SDL 和 OpenGL
target_link_libraries(sos ${SDL2_LIBRARIES} opengl32)
target_link_libraries(steam_wrapper ${SDL2_LIBRARIES} opengl32)
