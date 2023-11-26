# makefile clientManager

TARGET= clientManagerMain.out
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS= clientManager.o clientNet.o protocol.o ui.o clientManagerMain.o
PATH_LIB = ../../../gends/
PATH_PROTOCOL = ../protocol/
PATH_LIST = ../../genericLinkedList/

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -L$(PATH_LIB) -lgends -lm -Wl,-rpath $(PATH_LIB)

clientManagerMain.o: clientManagerMain.c clientManager.h
	$(CC) $(CFLAGS) -I ./ -I $(PATH_LIB)-c clientManagerMain.c

clientNet.o: clientNet.c clientNet.h
	$(CC) $(CFLAGS) -I ./ -I $(PATH_LIB) -c clientNet.c

clientManager.o: clientManager.c clientManager.h clientNet.h ui.h $(PATH_PROTOCOL)protocol.h $(PATH_LIST)list.h
	$(CC) $(CFLAGS) -I ./ -I $(PATH_PROTOCOL) -I $(PATH_LIST) -c clientManager.c

protocol.o: $(PATH_PROTOCOL)protocol.c $(PATH_PROTOCOL)protocol.h
	$(CC) $(CFLAGS) -I $(PATH_PROTOCOL) -c $(PATH_PROTOCOL)protocol.c

ui.o: ui.c ui.h $(PATH_PROTOCOL)protocol.h
	$(CC) $(CFLAGS) -I ./ -I $(PATH_PROTOCOL) -c ui.c

vlg:
	valgrind -s --leak-check=yes --track-origins=yes ./$(TARGET)

lib:
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(PATH_LIB)
	export LD_LIBRARY_PATH

clean:
	rm -f $(TARGET) $(OBJS)
