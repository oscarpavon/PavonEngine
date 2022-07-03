include(../../make_include.txt)

set(windows 
		windows/tabs.c
		windows/content_browser.c
		windows/static_mesh_editor.c
		windows/windows.c
		)

set(editor_sources 
input.c
editor.c
text.c
content_manager.c
menus.c
skeletal_editor.c
data.c
gizmos.c
editor_mode.c
export_model.c
geometry.c
commands.c
Textures/texture_factory.c
HLOD/HLOD_factory.c
editor_command_queue.c
level.c
path.c
network/network.c
directories.c
../ThirdParty/parson.c
${windows}
file_explorer.c
ProjectManager/project_manager.c
)

if(DESKTOP)
		set(editor_sources ${editor_sources}
 
		EditServer/edit_server.c
		main.c

		)
endif()

set (game_source
        ../engine/game.c
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)


if(DESKTOP)
add_executable(peditor ${editor_sources} ${game_source})
FIND_LIBRARY(bullet libBulletDynamics.so )
FIND_LIBRARY(bullet1 libBulletCollision.so)
FIND_LIBRARY(bullet2 libBullet3Common.so)
set(lib 
	engine        
    ${engine_link}
    )
target_link_libraries(peditor ${lib})
endif()

if(ANDROID)

add_library(peditor
STATIC
${editor_sources} 
)

endif()

