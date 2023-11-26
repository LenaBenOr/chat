LIB_PATH=../../libgends
INCLUDE_PATH=../../libgends
CC=gcc
CFLAGS=-g -ansi -pedantic -I$(INCLUDE_PATH)
SERVER_OBJS=serverDRV.o ServerMng.o
SERVER_TARGET=server.out


all: $(SERVER_TARGET) 

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -L$(LIB_PATH) -Wl,-rpath $(LIB_PATH) $^ -o $@ -lgends -lrt

serverDRV.o: serverDRV.c serverDRV.h
	$(CC) $(CFLAGS) -c $< -o $@

ServerMng.o: ServerMng.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) 
