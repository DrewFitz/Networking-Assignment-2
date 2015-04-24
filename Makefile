common_headers = data_types.h

myclient: myclient.c $(common_headers)
	gcc myclient.c -g -o myclient

myserver: myserver.c $(common_headers)
	gcc myserver.c -g -o myserver

.PHONY: clean all

clean:
	rm myclient myserver
	rm -rf ./*.dSYM

all: myclient myserver