set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required(VERSION 3.10)
project(sdl_opengl32_simple_demo)

# SDL2库的路径，请根据实际情况修改
set(SDL2_PATH "D:/project/c/SDL2-2.28.0")
# SDL2_ttf库的路径，请根据实际情况修改
set(SDL2_TTF_PATH "D:/project/c/SDL2_ttf-2.20.2")
# SDL2_image库的路径，请根据实际情况修改
set(SDL2_IMAGE_PATH "D:/project/c/SDL2_image-2.6.3")

# 找到 SDL2
list(APPEND CMAKE_PREFIX_PATH ${SDL2_PATH})
find_package(SDL2 REQUIRED)

# 找到 SDL2_TTF
list(APPEND CMAKE_PREFIX_PATH ${SDL2_TTF_PATH})
find_package(SDL2_ttf REQUIRED)

# 找到 SDL2_image
list(APPEND CMAKE_PREFIX_PATH ${SDL2_IMAGE_PATH})
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
# 在这里不引入SDL2_ttf, 它会自动引入
include_directories("D:/project/c/SDL2_mixer-2.6.3/i686-w64-mingw32/include")

# Build test exe
add_executable(test test.c)
set_target_properties(test PROPERTIES PREFIX "")

# Build steam_wrapper DLL
add_library(steam_wrapper SHARED steam_wrapper.c)
set_target_properties(steam_wrapper PROPERTIES PREFIX "")
add_custom_command(TARGET steam_wrapper POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:steam_wrapper>
        D:/program/Steam/steamapps/common/DeathRoadToCanada)

# Build SDL2_mixer DLL
add_library(SDL2_mixer SHARED SDL2_mixer.c)
set_target_properties(SDL2_mixer PROPERTIES PREFIX "")
add_custom_command(TARGET SDL2_mixer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL2_mixer>
        D:/program/Steam/steamapps/common/DeathRoadToCanada)

# 链接 SDL 和 OpenGL
target_link_libraries(test ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image opengl32)
target_link_libraries(steam_wrapper ${SDL2_LIBRARIES} opengl32)
target_link_libraries(SDL2_mixer ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image opengl32)
