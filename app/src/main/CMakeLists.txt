cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_C_STANDARD_REQUIRED ON)

set(engine_source 
model.c
vector.c
file_loader.c
level.c
camera.c
images.c
input.c        
gui.c
shader.c
game.c
object_select.c
engine.c
)

if(NOT ANDROID)

set(CMAKE_BUILD_TYPE Debug)

set(sources 
level_editor/main.c
level_editor/windows.c
level_editor/input.c
)

add_executable(Editor ${sources} ${engine_source})

target_link_libraries(Editor        
        glfw
        EGL
        GLESv2
        m
        )
endif(NOT ANDROID)

if(ANDROID)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)


find_library(log-lib
log)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(main_file main.c audio.c)


add_library(maincode
    SHARED
        ${main_file}
        ${engine_source}
    )

endif(ANDROID)

include_directories(/usr/include/cglm)






if(ANDROID)
target_link_libraries(maincode
        EGL
        GLESv2
        android
        native_app_glue        
        ${log-lib}
        OpenSLES
        ) 
endif(ANDROID)