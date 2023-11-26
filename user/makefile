# makefile UserMng.out

TARGET= userMng.out
CC= gcc
CFLAGS= -ansi -g -pedantic 
OBJS= user.o userMng.o 
PATH_GENDS= ../../../gends/
PATH_HASH= ../../genericHashMap/

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -L$(PATH_GENDS) -lgends -lm -Wl,-rpath $(PATH_GENDS)
	
user.o: user.c user.h 
	$(CC) $(CFLAGS) -c user.c 

userMng.o: userMng.c userMng.h user.h $(PATH_HASH)hashMap.h
	$(CC) $(CFLAGS) -I $(PATH_HASH) -c userMng.c

vlg:
	valgrind -s --leak-check=yes --track-origins=yes ./$(TARGET)

lib: #this command doesnt work, need to write those 2 lines in the terminal after make
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(PATH_GENDS)
	export LD_LIBRARY_PATH

clean:
	rm -f $(TARGET) $(OBJS)



# -lm is to link math.h
