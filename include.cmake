
#set(OpenGL_GL_PREFERENCE GLVND)

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})


#find_package(OpenGL REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

if(CMAKE_SYSTEM_NAME MATCHES "OpenBSD|FreeBSD")

	
endif()


if(CMAKE_SYSTEM_NAME MATCHES "Linux")

find_package(glfw3 REQUIRED)

set(linux_libraries 
	
  pulse
    pulse-simple
    asound
)


set(pc_link 
	  glfw
    pe_vk
    vulkan
    
    #BulletDynamics
    #BulletCollision
    #BulletSoftBody
    #LinearMath
	  ${OPENGL_LIBRARIES}
	  dl
	  m
	  pthread
    EGL 
    GLESv2
    peditor
)

set(lib ${lib} ${linux_libraries})

endif()

set(engine_link 
	${FREETYPE_LIBRARIES}
  ${pc_link}
    )

set(android_link 
    android
    native_app_glue
    EGL
    log
    GLESv2
    pavon_engine
    m
    peditor
    #${FREETYPE_LIBRARIES}
    freetype
    
    )

