
UNAME_S := $(shell uname -s)
################ cxxflags ########
#common flag
cxxflags:= -std=c++11 -Wall

#eigen debug
cxxflags+=-DEIGEN_RUNTIME_NO_MALLOC  

#eigen vectorization
cxxflags+= -msse4.2 -mavx2 -mfma -O2 


#avx compatible(eigen)
ifeq ($(UNAME_S),Linux)
	cxxflags+=-fabi-version=6
endif
#ifeq ($(UNAME_S),Darwin)
#	CCFLAGS += 
#endif

#eigen licence
cxxflags+=-DEIGEN_MPL2_ONLY

#sharded lib
cxxflags+=-fPIC
############### cxxflags #############


