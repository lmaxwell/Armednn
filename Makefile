
SRC:=$(wildcard src/examples/*.cpp)
OBJS:=$(SRC:.cpp=.o) 

all: $(OBJS)
	mkdir -p bin
	g++  -std=c++11 $(OBJS) -Lbuild -ltsnn  -o bin/test

%.o:%.cpp
	#g++ -c  -g -std=c++11   -march=native -fabi-version=6 -O2 -Iinclude -Ithird_party/eigen  $< -o $@
	#g++ -c  -std=c++11 -DNDEBUG  -msse4.2  -fabi-version=6 -mavx2 -mfma -O3 -Iinclude -Ithird_party/eigen    $< -o $@
	g++ -c  -std=c++11   -msse4.2  -fabi-version=6 -mavx2 -mfma -O2 -Iinclude -Ithird_party/eigen    $< -o $@
	#g++ -c  -std=c++11 -DNDEBUG  -O3 -Iinclude -Ithird_party/eigen    $< -o $@
clean:
	rm -f src/*/*.o

