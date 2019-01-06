
cxx:=g++


-include common.mk

#includes
cxxflags+= -Iinclude -Ithird_party/eigen

# mkl 
#eigen_use_mkl_all
ldflags:=



srcs:=$(wildcard src/core/*.cpp)
srcs+=$(wildcard src/op/*.cpp)


objs:=$(addprefix .temp/, $(srcs:.cpp=.o))


build:=lib

static:=$(build)/libarmednn.a

shared:=$(build)/libarmednn.so

debug : cxxflags+=-g 
debug: all

release: cxxflags+=-DNDEBUG
release: all

all: $(static) $(shared) tests example

$(static): $(objs)
	@mkdir -p $(dir $(static))
	ar rcs $(ldflags) $@ $^
	# -cvq 

$(shared): $(objs)
	@mkdir -p $(dir $(shared))
	$(cxx) -shared $(ldflags) -o $@ $^

example: $(shared)
	+$(MAKE) -C example 
tests: $(shared)
	+$(MAKE) -C tests 

$(objs): .temp/%.o: %.cpp
	@echo "$< -> $@"
	@mkdir -p $(dir $@)
	$(cxx) -c $(cxxflags)  $< -o $@
	$(cxx) -MM -MQ $@ $(cxxflags) $< > .temp/$*.d

-include $(objs:.o=.d)


clean:
	@rm -f $(static) $(shared)  $(objs) $(objs:.o=.d)
	$(MAKE) -C tests clean
	$(MAKE) -C example clean

.PHONY: release debug all tests example clean 
