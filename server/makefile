CC=g++
CFLAGS=-std=c++11 -I -Wall -lpthread
OBJ = server_sample.cpp

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

server_20172066: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)
