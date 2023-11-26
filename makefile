# makefile ServerMng.out

TARGET= serverMain.out
CC= gcc
CFLAGS= -ansi -g -pedantic 
OBJS= ServerMng.o serverDRV.o userMng.o user.o serverMain.o protocol.o
PATH_GENDS= libgends
PATH_SERVER= ServerNet
PATH_USER= user
PATH_PROTOCOL= protocol

ALL_PATHS = $(PATH_GENDS) $(PATH_SERVER) $(PATH_USER) $(PATH_PROTOCOL)
ALL_INCLUDE_PATHS = $(addprefix -I,$(ALL_PATHS))


$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -L$(PATH_GENDS) -lgends -lm -Wl,-rpath $(PATH_GENDS)
	
serverMain.o: $(PATH_SERVER)/serverMain.c $(PATH_SERVER)/ServerMng.h 
	$(CC) $(CFLAGS) $(ALL_INCLUDE_PATHS) -c $(PATH_SERVER)/serverMain.c

ServerMng.o: $(PATH_SERVER)/ServerMng.c $(PATH_SERVER)/ServerMng.h $(PATH_SERVER)/serverDRV.h $(PATH_USER)/userMng.h
	$(CC) $(CFLAGS) $(ALL_INCLUDE_PATHS) -c $(PATH_SERVER)/ServerMng.c
		
serverDRV.o: $(PATH_SERVER)/serverDRV.c $(PATH_SERVER)/serverDRV.h $(PATH_GENDS)/list.h $(PATH_GENDS)/list_itr.h $(PATH_GENDS)/list_functions.h
	$(CC) $(CFLAGS) $(ALL_INCLUDE_PATHS) -c $(PATH_SERVER)/serverDRV.c 

protocol.o: $(PATH_PROTOCOL)/protocol.c $(PATH_PROTOCOL)/protocol.h
	$(CC) $(CFLAGS) $(ALL_INCLUDE_PATHS) -c $(PATH_PROTOCOL)/protocol.c

userMng.o: $(PATH_USER)/userMng.c $(PATH_USER)/userMng.h $(PATH_USER)/user.h $(PATH_GENDS)/hashmap.h
	$(CC) $(CFLAGS) $(ALL_INCLUDE_PATHS) -c $(PATH_USER)/userMng.c

user.o: $(PATH_USER)/user.c $(PATH_USER)/user.h 
	$(CC) $(CFLAGS) $(ALL_INCLUDE_PATHS) -c $(PATH_USER)/user.c

vlg:
	valgrind -s --leak-check=yes --track-origins=yes ./$(TARGET)

lib: #this command doesnt work, need to write those 2 lines in the terminal after make
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(PATH_GENDS)
	export LD_LIBRARY_PATH

clean:
	rm -f $(TARGET) $(OBJS)



# -lm is to link math.h
