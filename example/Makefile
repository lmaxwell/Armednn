
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

$(objs): bin/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(cxx) -c $(cxxflags)  $< -o $@


$(exes): % : %.o  $(libpath)/libarmednn.so
	@mkdir -p $(dir $@)
	$(cxx)  $(ldflags) -Wl,-rpath $(libpath)  $*.o -o $@

clean:
	@rm -f bin/*
