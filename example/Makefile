
-include ../common.mk
cxx:=g++

#includes
cxxflags+= -I../include -I../third_party/eigen


srcs:=$(wildcard *.cpp)
objs:=$(addprefix bin/,$(srcs:.cpp=.o))
exes:=$(objs:.o=)

libpath:=$(shell pwd)/../lib
ldflags:=-L$(libpath) -larmednn


all: $(exes)

link=


$(objs): bin/%.o: %.cpp $(libpath)/libarmednn.so $(libpath)/libarmednn.a 
	@mkdir -p $(dir $@)
	$(cxx) -c $(cxxflags)  $< -o $@


$(exes): % : %.o 
	@mkdir -p $(dir $@)
ifeq ($(link),dynamic)
	$(cxx)  $(ldflags) -Wl,-rpath $(libpath)  $*.o -o $@
else
	$(cxx)   -Wl,--whole-archive $(libpath)/libarmednn.a -Wl,--no-whole-archive  $*.o -o $@
endif


clean:
	@rm -f bin/*
