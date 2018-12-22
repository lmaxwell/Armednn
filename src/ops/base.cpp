
#include "tsnn/op.h"

namespace Tsnn{


bool _add_matrix_matrix(Matrix& a,Matrix &b)
{
    if(a.rows()==b.rows() && a.cols()==b.cols())
    {
        a+=b;
        return true;
    }
    else
    {
        return false;
    }
}

Eigen::Map<Vector> _matrix_to_vector_in_place(Matrix& a)
{
    CHECK(a.rows()==1);
    return Eigen::Map<Vector>(a.data(),1,a.cols());
}

bool _add_matrix_vector(Matrix& a,Matrix &b)
{
    CHECK(b.rows()==1);
    if(a.cols()==b.cols())
    {
        a.rowwise()+=_matrix_to_vector_in_place(b);
        return true;
    }
    else
    {
        return false;
    }
}

/*
add in place
operate a+b , and store result in a
*/
bool add(Matrix& a,Matrix& b)
{
           
    if(b.rows()==1)
    {
        return _add_matrix_vector(a,b);
    }
    else 
        return _add_matrix_matrix(a,b);
}




/*
multiply
operate a*b , and store result in c  
*/
bool  multiply(Matrix& a, Matrix& b, Matrix& c)
{
    if(a.cols()==b.rows())
    {
        c.noalias() = a*b;
        return true;
    }
    else
    {
        DEBUG<<"a.cols() " <<a.cols()<<" != b.rows() "<<b.rows();
        return false;
    }

}

/*
multiply elementwise
operate a*b  , and store result in c  
*/
bool multiply_elemwise(Matrix &a, Matrix&b, Matrix& c)
{
    if(a.rows()==b.rows() && a.cols()==b.cols())
    {
        c.array()=a.array()*b.array();
        return true;
    }
    else
    {
        return false;
    }

}

}
