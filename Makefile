CC=g++
EXE=main.out
HDR=include
CPPFLAGS=-I $(HDR)


SRC=$(wildcard ./source/*.cpp) $(wildcard ./case/*.cpp) $(wildcard *.cpp)
OBJ=$(patsubst %.cpp,%.o,$(SRC))
DEP=$(patsubst %.o,%.d,$(OBJ))


$(EXE): $(OBJ)
	$(CC) $^ -o $@


%.o: %.cpp
	$(CC) $< -c $(CPPFLAGS) -o $@


include $(DEP)


%.d: %.cpp
	set -e; \
	rm -f $@; \
	$(CC) $< -MM $(CPPFLAGS) > $@.tmp; \
	sed 's,.*:,$*.o $@:,g' < $@.tmp > $@; \
	rm -f $@.tmp


.PHONY: clean
clean:
	rm -f $(EXE) $(OBJ) $(DEP)


echo:
	@echo SRC=$(SRC)  
	@echo OBJ=$(OBJ)  
	@echo dep=$(DEP)  
	@echo CPPFLAGS=$(CPPFLAGS) 
