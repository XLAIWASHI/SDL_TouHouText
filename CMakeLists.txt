#标题
cmake_minimum_required(VERSION 3.10.0)
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)

#设置c++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# 屏蔽 CMake 弃用警告
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

#/wd4100：就是关掉 “未引用的参数” 警告
#/wd4244：就是关掉 “从 int 转 float 可能丢数据” 的警告
add_compile_options(/wd4100 /wd4244)

#设置编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

#查找并载入Cmake预设
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

#修复
include_directories(${SDL2_INCLUDE_DIRS})

#添加可执行文件
add_executable(${TARGET}
                "src/main.cpp"
                "src/Game.cpp"
                "src/Scene.cpp"
                "src/SceneMain.cpp"
                "src/SceneTitle.cpp"
                "src/SceneOption.cpp"
                "src/Utils.cpp"
                "src/Boss.cpp"
                "src/Pattern.cpp"
                "src/BulletPattern.cpp"
                "src/EnemyBullet.cpp"
                "src/BulletManager.cpp"
                "src/Boss1Fight.cpp"
                "src/BossFight.cpp"
                "src/BossfightController.cpp"
                "src/BulletSkill.cpp"
                "src/Effect.cpp"
                "src/EffectManager.cpp"
                "src/Bomb.cpp"
                "src/Pseudo3DBackground.cpp"
                "src/SceneEnd.cpp")

#链接库
target_link_libraries(${TARGET}
                        ${SDL2_LIBRARIES}
                        SDL2_image::SDL2_image
                        SDL2_mixer::SDL2_mixer
                        SDL2_ttf::SDL2_ttf
                        nlohmann_json::nlohmann_json
                        spdlog::spdlog
                        )
#关闭终端
if(MSVC)
    target_link_options(${TARGET} PRIVATE "/SUBSYSTEM:WINDOWS")
endif()