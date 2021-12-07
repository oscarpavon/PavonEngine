include(/home/pavon/PavonEngine/make_include.txt)

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
../ThirdParty/parson.c
)

set (game_source
        ../engine/game.c
)


add_executable(peditor ${editor_sources} ${game_source})

set(lib 
	engine        
    ${engine_link}
    )

target_link_libraries(peditor ${lib})
