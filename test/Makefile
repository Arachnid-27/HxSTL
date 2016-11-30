CC=g++
HDR=../include
CPPFLAGS=-std=c++11 -g -I $(HDR)


SRC=$(wildcard *.cpp)
EXE=$(patsubst %.cpp,%,$(SRC))


all: $(EXE)


.PHONY: clean
clean:
	rm -f $(EXE)


echo:
	@echo SRC=$(SRC)  
	@echo EXE=$(EXE)  
	@echo CPPFLAGS=$(CPPFLAGS) 
