
cmake_minimum_required(VERSION 3.4.1)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")

add_subdirectory(glm)
add_library(game SHARED
        android_main.cpp
        native_engine.cpp
        scene/anim.cpp
        scene/dialog_scene.cpp
        scene/play_scene.cpp
        scene/scene.cpp
        scene/scene_manager.cpp
        scene/ui_scene.cpp
        scene/welcome_scene.cpp
        shader/shader.cpp
        shader/our_shader.cpp
        utils/ascii_to_geom.cpp
        utils/indexbuf.cpp
        utils/obstacle.cpp
        utils/obstacle_generator.cpp
        utils/sfxman.cpp
        utils/util.cpp
        utils/vertexbuf.cpp
        view/input_util.cpp
        view/texture.cpp
        view/tex_quad.cpp
        render/shape_render.cpp
        render/text_render.cpp)

target_include_directories(game PRIVATE
        .
        data
        ${ANDROID_NDK}/sources/android/native_app_glue)
# add lib dependencies
target_link_libraries(game
        android
        native_app_glue
        atomic
        EGL
        GLESv2
        glm
        log
        OpenSLES)