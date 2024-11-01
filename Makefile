
WORKDIR := $(shell pwd)

all:
	make -C ./src/engine WORKDIR=$(WORKDIR)
	make -C ./src/editor WORKDIR=$(WORKDIR)


clean:
	make -C ./src/editor clean
	make -C ./src/engine clean
	rm -f ./bin/peditor
