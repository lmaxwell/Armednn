
cxx:=g++


-include common.mk

#includes
cxxflags+= -Iinclude -Ithird_party/eigen

ldflags:=

srcs:=$(wildcard src/core/*.cpp)
srcs+=$(wildcard src/op/*.cpp)
ifneq ($(OTHEROP),)
	srcs+=$(wildcard $(OTHEROP)/*.cpp)
endif

objs:=$(addprefix .temp/, $(srcs:.cpp=.o))

# mkl 
ifneq ($(MKLROOT),)
	MKLROOT_PATH:=$(shell cd $(MKLROOT);pwd)
	cxxflags+= -I$(MKLROOT)/include -DEIGEN_USE_MKL_ALL -DMKL_LP64 -m64
endif

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
	+$(MAKE) -C example MKLROOT=$(MKLROOT_PATH) 
tests: $(shared)
	+$(MAKE) -C tests MKLROOT=$(MKLROOT_PATH)

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
