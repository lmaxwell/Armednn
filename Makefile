
SRC:=$(wildcard src/examples/*.cpp)
OBJS:=$(SRC:.cpp=.o) 

all: $(OBJS)
	mkdir -p bin
	g++ -g -std=c++11 $(OBJS) -Lbuild -ltsnn  -o bin/test

%.o:%.cpp
	g++ -c  -std=c++11 -Iinclude -Ithird_party/eigen $< -o $@
clean:
	rm -f src/examples/*.o

