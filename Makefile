
WORKDIR := $(shell pwd)

all: engine editor

engine:
	make -C ./src/engine WORKDIR=$(WORKDIR)

editor:
	make -C ./src/editor WORKDIR=$(WORKDIR)

clean:
	make -C ./src/editor clean
	make -C ./src/engine clean
	rm ./bin/peditor
