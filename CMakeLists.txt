set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
cmake_minimum_required(VERSION 3.10)
project(drtc_utf8)

# 注释本地路径改用msys2安装
# SDL2库的路径，请根据实际情况修改
#set(SDL2_PATH "D:/project/c/SDL2-2.28.0")

# 找到 SDL2
#list(APPEND CMAKE_PREFIX_PATH ${SDL2_PATH})
find_package(SDL2 REQUIRED)

# 找到 SDL2_TTF
#list(APPEND CMAKE_PREFIX_PATH ${SDL2_TTF_PATH})
find_package(SDL2_ttf REQUIRED)

# 引入Gettext
find_package(Gettext REQUIRED)
find_library(GETTEXT_LIBRARY
        NAMES gettext libgettext libintl libintl-8
        )

# 引入libiconv
find_library(libiconv_LIBRARY
        NAMES libiconv
        )

# 引入sdl和sdl_mixer
include_directories(${SDL2_INCLUDE_DIRS})

# Build SDL2_mixer DLL
add_library(SDL2_mixer SHARED SDL2_mixer.c "NERVE SEP-27-2022.07.43.h" SDL_mixer.h)
set_target_properties(SDL2_mixer PROPERTIES PREFIX "")
add_custom_command(TARGET SDL2_mixer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL2_mixer>
        D:/program/Steam/steamapps/common/DeathRoadToCanada)

# SDL2_mixer链接 SDL 和 OpenGL 和 GETTEXT
target_link_libraries(SDL2_mixer ${SDL2_LIBRARIES} opengl32 ${GETTEXT_LIBRARY})

# test_utf8构建和链接
add_executable(test_utf8 test_utf8.c)
target_link_libraries(test_utf8 ${SDL2_LIBRARIES} opengl32 ${GETTEXT_LIBRARY} ${libiconv_LIBRARY} SDL2_ttf::SDL2_ttf)
