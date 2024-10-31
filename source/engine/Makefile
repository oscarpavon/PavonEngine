

SRCS := $(wildcard *.c)
OBJS := $(SRCS:c=o)

CFLAGS := -g -fPIC -Wno-switch -Wno-implicit-function-declaration -Wno-int-conversion -Wno-return-type -fcommon -Wno-incompatible-pointer-types

CINCLUDES := -I/root/PavonEngine/source/ -I/usr/include/freetype2 -I/usr/local/include

GLOBAL_DEFINE := -D OPENGL_ES2 -D LINUX -D DESKTOP -D VULKAN -D DEBUG

.PHONY: all clean install

all: engine.a

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) $(GLOBAL_DEFINE) $(CINCLUDES) -c $<

engine.a: $(OBJS)


clean:
	rm *.o
	
	
