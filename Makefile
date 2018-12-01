all:
	g++ -g -std=c++11 layer.cpp split.cpp dense.cpp test.cpp -I.  -o test
