# Name: Aaron Colin Foote
# Date: 30th May 2013
# User: acf502 / 4258770
# File.: server.cpp for CSCI212 Assignment 3
SVR_OBJECT_FILES = server.o aux.o
CLT_OBJECT_FILES = client.o aux.o 
TARGETS=server.o client.o aux.o
CC=CC
INCLUDES=
LIBS=-lsocket -lnsl -lresolv -mt -lpthread
CFLAGS=-g $(INCLUDES)

all: client server
client: $(CLT_OBJECT_FILES)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
server: $(SVR_OBJECT_FILES)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


client.o server.o aux.o: aux.h

%.o: %.cc
	$(CC) -c $<

clean:
	rm -f *.o $(TARGETS)

