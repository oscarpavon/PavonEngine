set(windows 
		windows/tabs.c
		windows/content_browser.c
		windows/static_mesh_editor.c
		windows/windows.c
		)


set(editor_sources 
main.c
input.c
editor.c
text.c
skeletal_editor.c
data.c
gizmos.c
editor_mode.c
export_model.c
geometry.c
commands.c
Textures/texture_factory.c
HLOD/HLOD_factory.c
menus.c
content_manager.c
EditServer/edit_server.c
editor_command_queue.c
file_explorer.c
level.c
path.c
${windows}
ProjectManager/project_manager.c
network/network.c
directories.c
)

set (game_source
        ../engine/game.c
)

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_executable(peditor ${editor_sources} ${game_source})

set(lib 
	engine        
	m
	glfw
	pthread
	${FREETYPE_LIBRARIES}
	${OPENGL_LIBRARIES}
	)
if(CMAKE_SYSTEM_NAME MATCHES "OpenBSD|FreeBSD")

	
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
set(linux_libraries 
	dl
	asound
)
set(lib ${lib} ${linux_libraries})
endif()

target_link_libraries(peditor ${lib})
