include ../../include.make

.PHONY: all clean install

all: peditor

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) $(GLOBAL_DEFINE) $(CINCLUDES) -c $<

windows := $(wildcard ./windows/*.c)
textures := $(wildcard ./Textures/*.c)

LIBC := /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/libc.so /usr/lib/crtn.o -dynamic-linker /lib/ld-linux.so.2
LIBRARIES := -lvulkan -lm -lglfw3 -lGLESv2 -lpthread -lfreetype -lEGL -ldl  

peditor: $(OBJS)
	$(COMPILE) -c $(textures)
	$(COMPILE) -c $(windows)
	ld $(LIBC) *.o -L../engine -lengine $(LIBRARIES) -o $(WORKDIR)/bin/peditor
	


clean:
	rm -f *.o
