.POSIX:
.SUFFIXES:
CXX=g++
CXXFLAGS=
LDFLAGS=-lmingw32 -lws2_32 -lSDL2main -lSDL2 
DEBUGARGS=-g3 -Wall -Wextra -Wconversion -Wdouble-promotion -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion 
BUILD=$(CXX) $(CXXFLAGS) -o build/$@ $? $(LDFLAGS) 

all: server client

server: server.cpp win32_socket.cpp
	$(BUILD) $(DEBUGARGS)
client: client.cpp win32_socket.cpp win32_platform.cpp
	$(BUILD) $(DEBUGARGS)
