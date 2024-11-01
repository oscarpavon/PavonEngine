

include $(WORKDIR)/include.make

.PHONY: all clean install

all: engine.a

components_src := $(wildcard ./components/*.c)

opengl_src := $(wildcard ./renderer/opengl/*.c)
renderer_src := $(wildcard ./renderer/*.c)
animation_src := $(wildcard ./animation/*.c)

$(OBJS): %.o : %.c 
	$(CC) $(CFLAGS) $(GLOBAL_DEFINE) $(CINCLUDES) -c $<

engine.a: $(OBJS) 
	$(COMPILE) -c $(components_src)
	$(COMPILE) -c $(renderer_src)
	make -C ./renderer/vulkan WORKDIR=$(WORKDIR)
	$(COMPILE) -c $(opengl_src)
	$(COMPILE) -c $(animation_src)
	$(COMPILE) -c ../ThirdParty/parson.c
	$(COMPILE) -c ./platforms/windows_manager.c 
	$(COMPILE) -c ./Collision/collision.c

	ar rcs libengine.a *.o ./renderer/vulkan/*.o


clean:
	rm -f *.o
	rm -f libengine.a
	make -C ./renderer/vulkan clean
	
	
