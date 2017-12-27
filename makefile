CC =gcc
CFLAGS =-Wall
OBJECTS =  main.c indexFunctions.c browseFunctions.c

indexer: $(OBJECTS)
        $(CC) $(CFLAGS) $(OBJECTS) -o indexer
main.o: main.c
        $(CC) $(CFLAGS) -c main.c
%.o: %.c
        $(CC) $(CFLAGS) -c $<
clean:
        rm indexer $(OBJECTS)

