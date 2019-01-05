
SRC:=$(wildcard *.cpp)
OBJS:=$(SRC:.cpp=.o) 


FLAGS=-DEIGEN_RUNTIME_NO_MALLOC -std=c++11   -msse4.2  -fabi-version=6 -mavx2 -mfma -O2 -Iinclude -Ithird_party/eigen   -Wall

#FLAGS+=-DEIGEN_USE_BLAS 

#EIGEN_USE_MKL_ALL

all: $(OBJS)
	mkdir -p bin
	g++  -std=c++11 $(OBJS) -lopenblas -o bin/test

%.o:%.cpp
	g++ -c $(FLAGS)  $< -o $@
clean:
	rm -f *.o

