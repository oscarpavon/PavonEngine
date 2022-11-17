

if(CMAKE_SYSTEM_NAME MATCHES "Linux")

set(audio   
${CMAKE_CURRENT_LIST_DIR}/platforms/linux/audio_alsa.c
${CMAKE_CURRENT_LIST_DIR}/audio/audio_engine.c
${CMAKE_CURRENT_LIST_DIR}/audio.c
)

set(network 
	${CMAKE_CURRENT_LIST_DIR}/network/network.c
	${CMAKE_CURRENT_LIST_DIR}/platforms/linux/network.c
)

endif()



set(components
        ${CMAKE_CURRENT_LIST_DIR}/components/components.c
        ${CMAKE_CURRENT_LIST_DIR}/components/static_mesh_component.c
        ${CMAKE_CURRENT_LIST_DIR}/components/skinned_mesh_component.c
        )
set(third_party
	../ThirdParty/parson.c
	)	

set(opengl_es2_renderer
	${CMAKE_CURRENT_LIST_DIR}/renderer/opengl/opengl_es2.c
	${CMAKE_CURRENT_LIST_DIR}/renderer/opengl/shader.c        
	)

set(renderer
	      ${opengl_es2_renderer}
	      ${CMAKE_CURRENT_LIST_DIR}/renderer/render_thread.c
        ${CMAKE_CURRENT_LIST_DIR}/renderer/material.c
        ${CMAKE_CURRENT_LIST_DIR}/renderer/opengl/rasterizer.c
    )


set(physics 
        ${CMAKE_CURRENT_LIST_DIR}/physics.c
    )

set(pe_base
        ${CMAKE_CURRENT_LIST_DIR}/base.c
        ${CMAKE_CURRENT_LIST_DIR}/engine.c
        ${CMAKE_CURRENT_LIST_DIR}/memory.c
        ${CMAKE_CURRENT_LIST_DIR}/array.c
        ${CMAKE_CURRENT_LIST_DIR}/threads.c
        )
set(demos ${CMAKE_CURRENT_LIST_DIR}/../../demos/chess/chess.c)

set(engine_source
        ${pe_base} 
        ${renderer}

        ${components}
	      ${network}
	      ${third_party}
	      #${audio}
	      #${physics}

        ${CMAKE_CURRENT_LIST_DIR}/log.c
        ${CMAKE_CURRENT_LIST_DIR}/LOD_system.c
        ${CMAKE_CURRENT_LIST_DIR}/model.c
        ${CMAKE_CURRENT_LIST_DIR}/file_loader.c
        ${CMAKE_CURRENT_LIST_DIR}/camera.c
        ${CMAKE_CURRENT_LIST_DIR}/images.c       
        ${CMAKE_CURRENT_LIST_DIR}/gui.c
        ${CMAKE_CURRENT_LIST_DIR}/object_select.c
        ${CMAKE_CURRENT_LIST_DIR}/skeletal.c
        ${CMAKE_CURRENT_LIST_DIR}/level.c
        ${CMAKE_CURRENT_LIST_DIR}/files_parser.c        
        ${CMAKE_CURRENT_LIST_DIR}/time.c
        ${CMAKE_CURRENT_LIST_DIR}/animation/animation.c
        ${CMAKE_CURRENT_LIST_DIR}/Collision/collision.c
        ${CMAKE_CURRENT_LIST_DIR}/content_manager.c
	      ${CMAKE_CURRENT_LIST_DIR}/game.c
	      ${CMAKE_CURRENT_LIST_DIR}/elements.c
	      ${CMAKE_CURRENT_LIST_DIR}/window_manager.c
	      ${CMAKE_CURRENT_LIST_DIR}/animation/node.c
        ${CMAKE_CURRENT_LIST_DIR}/text_renderer.c

        ${demos}

)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(platform_pc 	
		${CMAKE_CURRENT_LIST_DIR}/window_manager.c
		${CMAKE_CURRENT_LIST_DIR}/input.c
		)
	set(engine_source ${engine_source} ${window_manager} ${platform_pc})	
endif()

if(ANDROID)
        
	set(engine_source ${engine_source}
        ${CMAKE_CURRENT_LIST_DIR}/platforms/android/android.c
        ${CMAKE_CURRENT_LIST_DIR}/platforms/android/input.c

        ) 

endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

add_library(pavon_engine
STATIC
${engine_source}
)


if(CMAKE_SYSTEM_NAME MATCHES "Android")
include(../../include.cmake)
add_executable(level level.c)
target_link_libraries(level ${android_link})
target_link_directories(level PUBLIC /data/data/com.termux/files/home/PavonEngine/lib)
endif()
#add_library(engine_dynamic
#	SHARED
#	${engine_source}
#)

