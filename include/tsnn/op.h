#include "tsnn/core.h"
#include <iostream>

#ifndef _OP_H
#define _OP_H
namespace Tsnn{


/*
add in place
operate a+b , and store result in a
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


}
#endif

