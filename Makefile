.POSIX:
.SUFFIXES:
CXX=g++
CXXFLAGS=
LDFLAGS=-lSDL2main -lSDL2 
DEBUGARGS=-g3 -Wall -Wextra -Wconversion -Wdouble-promotion -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion 
BUILD=$(CXX) $(CXXFLAGS) -o build/$@ $? $(LDFLAGS) 

all: server client

server: server.cpp linux_socket.cpp
	$(BUILD) $(DEBUGARGS)
client: client.cpp linux_socket.cpp linux_platform.cpp
	$(BUILD) $(DEBUGARGS)
