
SRC:=$(wildcard *.cpp)
OBJS:=$(SRC:.cpp=.o) 

all: $(OBJS)
	mkdir -p bin
	g++  -std=c++11 $(OBJS)  -o bin/test

%.o:%.cpp
	g++ -c  -std=c++11   -msse4.2  -fabi-version=6 -mavx2 -mfma -O2 -Iinclude -Ithird_party/eigen    $< -o $@
clean:
	rm -f src/*/*.o

