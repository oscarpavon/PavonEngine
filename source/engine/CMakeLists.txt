
if(ANDROID)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(/home/pavon/sources/freetype2/include)

endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(audio 
		${CMAKE_CURRENT_LIST_DIR}/platforms/linux/audio_alsa.c
		)
endif()

set(network 
		${CMAKE_CURRENT_LIST_DIR}/network/network.c
		${CMAKE_CURRENT_LIST_DIR}/platforms/linux/network.c
)

set(components
        ${CMAKE_CURRENT_LIST_DIR}/components/components.c
        ${CMAKE_CURRENT_LIST_DIR}/components/static_mesh_component.c
        ${CMAKE_CURRENT_LIST_DIR}/components/skinned_mesh_component.c
        )
set(third_party
	../ThirdParty/parson.c
	)	

set(renderer
	${CMAKE_CURRENT_LIST_DIR}/renderer/render_thread.c
	${CMAKE_CURRENT_LIST_DIR}/renderer/opengl/opengl_es2.c
	${CMAKE_CURRENT_LIST_DIR}/text_renderer.c
  ${CMAKE_CURRENT_LIST_DIR}/renderer/opengl/shader.c        
	)
set(engine_source
        ${CMAKE_CURRENT_LIST_DIR}/array.c
        ${CMAKE_CURRENT_LIST_DIR}/threads.c
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
        ${CMAKE_CURRENT_LIST_DIR}/level.c
        ${CMAKE_CURRENT_LIST_DIR}/files_parser.c        
        ${CMAKE_CURRENT_LIST_DIR}/time.c
        ${CMAKE_CURRENT_LIST_DIR}/animation/animation.c
        ${CMAKE_CURRENT_LIST_DIR}/Collision/collision.c
        ${CMAKE_CURRENT_LIST_DIR}/content_manager.c
				${CMAKE_CURRENT_LIST_DIR}/audio/audio_engine.c
				${CMAKE_CURRENT_LIST_DIR}/windows_manager.c
				${CMAKE_CURRENT_LIST_DIR}/game.c
				${CMAKE_CURRENT_LIST_DIR}/input.c
        ${components}
				${audio}
				${network}
				${third_party}
				${renderer}
)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

add_library(engine
STATIC
${engine_source}
)

#add_library(engine_dynamic
#	SHARED
#	${engine_source}
#)

