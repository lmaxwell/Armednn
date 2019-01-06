
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


all: $(static) $(shared) tests example

$(static): $(objs)
	@mkdir -p $(dir $(static))
	ar -cvq $(ldflags) $@ $^

$(shared): $(objs)
	@mkdir -p $(dir $(shared))
	$(cxx) -shared $(ldflags) -o $@ $^

example: $(shared)
	@make -C example
tests: $(shared)
	@make -C tests

$(objs): .temp/%.o: %.cpp
	@echo "$< -> $@"
	@mkdir -p $(dir $@)
	$(cxx) -c $(cxxflags)  $< -o $@
	$(cxx) -MM -MQ $@ $(cxxflags) $< > .temp/$*.d

-include $(objs:.o=.d)


clean:
	@rm -f $(static) $(shared)  $(objs) $(objs:.o=.d)
	@make -C tests clean
	@make -C example clean

.PHONY: tests example  
