common_headers = data_types.h

myclient: myclient.c $(common_headers)
	gcc myclient.c --std=c99 -Wno-implicit-function-declaration -g -o myclient

myserver: myserver.c $(common_headers)
	gcc myserver.c --std=c99 -Wno-implicit-function-declaration -g -o myserver

.PHONY: clean all

clean:
	rm myclient myserver
	rm -rf ./*.dSYM

all: myclient myserver
