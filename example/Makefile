
-include ../common.mk
cxx:=g++

#includes
cxxflags+= -I../include -I../third_party/eigen -I../src/op


srcs:=$(wildcard *.cpp)
objs:=$(addprefix bin/,$(srcs:.cpp=.o))
exes:=$(objs:.o=)

libpath:=$(shell pwd)/../lib
ldflags:=-L$(libpath) -larmednn

ifneq ($(MKLROOT),)
	MKLLIBS:=-Wl,--start-group $(MKLROOT)/lib/intel64_lin/libmkl_intel_lp64.a \
		$(MKLROOT)/lib/intel64_lin/libmkl_sequential.a \
		$(MKLROOT)/lib/intel64_lin/libmkl_core.a -Wl,--end-group -lpthread -lm -ldl
endif



all: $(exes)

link=


$(objs): bin/%.o: %.cpp $(libpath)/libarmednn.so $(libpath)/libarmednn.a 
	@mkdir -p $(dir $@)
	$(cxx) -c $(cxxflags)  $< -o $@


$(exes): % : %.o 
	@mkdir -p $(dir $@)
ifeq ($(link),dynamic)
	$(cxx)  $(ldflags) -Wl,-rpath $(libpath) $(MKLLIBS) $*.o -o $@
else
	$(cxx)   -Wl,--whole-archive $(libpath)/libarmednn.a -Wl,--no-whole-archive $(MKLLIBS) $*.o -o $@
endif


clean:
	@rm -f bin/*
