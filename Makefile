
CC:=g++

################ CFLAGS ########

#common flag
CFLAGS:= -std=c++11 -Wall

#Eigen debug
CFLAGS+=-DEIGEN_RUNTIME_NO_MALLOC  

#Eigen vectorization
CFLAGS+= -msse4.2 -mavx2 -mfma -O2 

#includes
CFLAGS+= -Iinclude -Ithird_party/eigen

#avx compatible(eigen)
CFLAGS+=-fabi-version=6

#sharded lib
CFLAGS+=-fPIC

############### CFLAGS #############


# mkl 
#EIGEN_USE_MKL_ALL
LIBS:=



SRCS:=$(wildcard src/core/*.cpp)
SRCS+=$(wildcard src/op/*.cpp)

OBJS:=$(SRCS:.cpp=.o)


BUILD:=lib

STATIC:=$(BUILD)/libarmednn.a

SHARED:=$(BUILD)/libarmednn.so


all: static shared

static: $(OBJS)
	mkdir -p $(BUILD)
	ar -cvq $(STATIC) $^

shared: $(OBJS)
	mkdir -p $(BUILD)
	$(CC) -shared -o $(SHARED) $^

test: shared
	mkdir -p bin
	$(CC) $(CFLAGS) -L $(BUILD) -larmednn -Wl,-rpath `pwd`/lib tests/0_data.cpp -o bin/0_data

example: shared
	mkdir -p bin
	$(CC) $(CFLAGS) -L $(BUILD) -larmednn -Wl,-rpath `pwd`/lib example/ex1.cpp -o bin/ex1

%.o:%.cpp
	$(CC) -c $(CFLAGS)  $< -o $@
	$(CC) -MM -MQ $@ $(CFLAGS) $< > $*.d

-include $(OBJS:.o=.d)


clean:
	rm -f $(STATIC) $(SHARED)  $(OBJS)

.PHONY: example
