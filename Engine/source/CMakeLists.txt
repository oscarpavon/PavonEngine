set(components
        Engine/components/components.c
        )

set(engine_source 
        model.c
        file_loader.c
        camera.c
        images.c
        input.c        
        gui.c
        shader.c        
        object_select.c
        engine.c
        skeletal.c
        Engine/level.c
        third_party/jsmn.c
        Engine/files_parser.c
        Engine/memory.c
        Engine/array.c
        Engine/animation/animation.c
        ${components}
)

add_library(engine
        STATIC
        ${engine_source}
)
