CC=g++
CFLAGS=-std=c++11 -I -Wall -lpthread
OBJ = client_sample.cpp

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

client_20172066: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)
