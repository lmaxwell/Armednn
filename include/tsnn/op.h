#include "tsnn/core.h"
#include <iostream>

namespace Tsnn{


/*
add in place
operate a+b , and store result in a
*/
void add(Matrix& a,Matrix& b)
{
    a+=b;
}

/*
multiply
operate a*b , and store result in c  
*/
TSNN_STATUS  multiply(Matrix& a, Matrix& b, Matrix& c)
{
    if(a.rows==b.cols and a.cols==b.rows)
    {
        c.noalias() = a*b;
    }
    else
    {
        std::cout<<"
    }

}

/*
multiply elementwise
operate a*b  , and store result in c  
*/
void multiply(




}

