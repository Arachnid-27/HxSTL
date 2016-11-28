CC=g++
HDR=../include
CPPFLAGS=-std=c++11 -g -I $(HDR)


SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))


all: $(OBJ)


.PHONY: clean
clean:
	rm -f $(OBJ)
