
WORKDIR := $(shell pwd)

all:
	make -C ./src/engine WORKDIR=$(WORKDIR)
	make -C ./src/editor WORKDIR=$(WORKDIR)
	make -C ./src/shaders


clean:
	make -C ./src/editor clean
	make -C ./src/engine clean
	make -C ./src/shaders clean
	rm -f ./bin/peditor
