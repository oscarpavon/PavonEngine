
WORKDIR := $(shell pwd)

all: engine editor

engine:
	make -C ./source/engine WORKDIR=$(WORKDIR)

editor:
	make -C ./source/editor WORKDIR=$(WORKDIR)

clean:
	make -C ./source/editor clean
	make -C ./source/engine clean
