

include ../../include.make

.PHONY: all clean install

all: engine.a

components_src := $(wildcard ./components/*.c)

vulkan_src := $(wildcard ./renderer/vulkan/*.c)
opengl_src := $(wildcard ./renderer/opengl/*.c)
renderer_src := $(wildcard ./renderer/*.c)
animation_src := $(wildcard ./animation/*.c)

$(OBJS): %.o : %.c 
	$(CC) $(CFLAGS) $(GLOBAL_DEFINE) $(CINCLUDES) -c $<

engine.a: $(OBJS) 
	$(COMPILE) -c $(components_src)
	$(COMPILE) -c $(renderer_src)
	$(COMPILE) -c $(vulkan_src)
	$(COMPILE) -c $(opengl_src)
	$(COMPILE) -c $(animation_src)
	$(COMPILE) -c ../ThirdParty/parson.c
	$(COMPILE) -c ./platforms/windows_manager.c 
	$(COMPILE) -c ./Collision/collision.c

	ar rcs libengine.a *.o


clean:
	rm -f *.o
	rm -f libengine.a
	
	
