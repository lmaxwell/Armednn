#include "tsnn/core.h"
#include <iostream>

#ifndef _OP_H
#define _OP_H
namespace Tsnn{

namespace Op{


/*
add in place
operate a+b , and store result in a
if b.rows==1, b is broadcast
*/
bool add(Matrix& a,Matrix& b);


/*
multiply
operate a*b , and store result in c  
*/
bool  multiply(Matrix& a, Matrix& b, Matrix& c);

/*
multiply elementwise
operate a*b  , and store result in c  
*/
bool multiply_elemwise(Matrix &a, Matrix&b, Matrix& c);


//Activation function type
typedef std::function<bool (Matrix&)> ActFun;


/*
//get activation function
Matrix a=Matrix::Random(2,2);
activaition("tanh")(a);
*/
ActFun activation(std::string name);

} // Op
} // Tsnn
#endif

