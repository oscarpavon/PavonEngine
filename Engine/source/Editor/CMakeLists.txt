
set(editor_sources 
main.c
windows.c
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
Textures/render.c
)

set (game_source
        ../game.c
)

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Engine/binaries)

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