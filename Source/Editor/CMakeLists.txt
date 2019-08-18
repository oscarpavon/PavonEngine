
set(editor_sources 
main.c
windows_manager.c
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
Windows/content_browser.c
)

set (game_source
        ../game.c
)

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)

add_executable(Editor ${editor_sources} ${game_source})

target_link_libraries(Editor
engine        
glfw
EGL
GLESv2
m
pthread
freetype
dl
)