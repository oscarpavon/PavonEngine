
include $(WORKDIR)/include.make

vulkan.o: $(OBJS)

$(OBJS): %.o : %.c 
	$(CC) $(CFLAGS) $(GLOBAL_DEFINE) $(CINCLUDES) -c $<

clean:
	rm -f *.o
