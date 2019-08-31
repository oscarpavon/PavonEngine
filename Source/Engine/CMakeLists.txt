

if(ANDROID)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(/home/pavon/sources/freetype2/include)

endif()

set(components
        ${CMAKE_CURRENT_LIST_DIR}/components/components.c
        )
         
set(engine_source
        ${CMAKE_CURRENT_LIST_DIR}/array.c
        ${CMAKE_CURRENT_LIST_DIR}/threads.c
        ${CMAKE_CURRENT_LIST_DIR}/Renderer/OpenGL/opengl_es2.c
        ${CMAKE_CURRENT_LIST_DIR}/LOD_system.c
        ${CMAKE_CURRENT_LIST_DIR}/memory.c
        ${CMAKE_CURRENT_LIST_DIR}/model.c
        ${CMAKE_CURRENT_LIST_DIR}/file_loader.c
        ${CMAKE_CURRENT_LIST_DIR}/camera.c
        ${CMAKE_CURRENT_LIST_DIR}/images.c       
        ${CMAKE_CURRENT_LIST_DIR}/gui.c
        ${CMAKE_CURRENT_LIST_DIR}/object_select.c
        ${CMAKE_CURRENT_LIST_DIR}/engine.c
        ${CMAKE_CURRENT_LIST_DIR}/skeletal.c
        ${CMAKE_CURRENT_LIST_DIR}/shader.c        
        ${CMAKE_CURRENT_LIST_DIR}/level.c
        ${CMAKE_CURRENT_LIST_DIR}/files_parser.c        
        ${CMAKE_CURRENT_LIST_DIR}/time.c
        ${CMAKE_CURRENT_LIST_DIR}/animation/animation.c
        ${CMAKE_CURRENT_LIST_DIR}/Collision/collision.c
        ${CMAKE_CURRENT_LIST_DIR}/content_manager.c
        ${CMAKE_CURRENT_LIST_DIR}/text_renderer.c
        ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/jsmn.c
        ${components}
)



add_library(engine
STATIC
${engine_source}
)


