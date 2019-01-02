
#include <map>
#include "op_sub.h"



namespace Armednn{
namespace OpSub{


/* internal 
 * 
 * */
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

Eigen::Map<Vector,Eigen::Aligned> _matrix_to_vector_in_place(Matrix& a)
{
    CHECK(a.rows()==1);
    return Eigen::Map<Vector,Eigen::Aligned>(a.data(),1,a.cols());
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
if b.rows==1, b is broadcast
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


namespace {
//internal

// activation "tanh"
bool _tanh(Matrix& a)
{
    a.array()=a.array().tanh();
    return true;
}

// "sigmoid"
//  1/(1+exp(-x))
bool _sigmoid(Matrix& a)
{
    a.array()=1.0/(1.0 + (-a.array()).exp());
    return true;
}

//  "identity"
bool _identity(Matrix& a)
{
    return true;
}


//  not implemented
bool _no_implement(Matrix& a)
{
    return false;
}

}// internal

/*
//get activation function
Matrix a=Matrix::Random(2,2);
activaition("tanh")(a)
*/
ActFun activation(std::string name)
{
    if(name == "tanh")
        return &_tanh;
    if(name == "identity")
        return &_identity;
    if(name == "sigmoid")
        return &_sigmoid;

    DEBUG<<"activation "<<name<<" not implemented!";
    return &_no_implement;
}






}// Op
}// Armednn
