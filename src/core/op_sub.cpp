
#include <map>
#include "armednn/op_sub.h"



namespace Armednn{
namespace OpSub{



namespace {
//internal

// activation "tanh"
bool _tanh(Eigen::Ref<Matrix> a)
{
    a.array()=a.array().tanh();
    return true;
}

// "sigmoid"
//  1/(1+exp(-x))
bool _sigmoid(Eigen::Ref<Matrix> a)
{
    for(uint32_t i=0;i<a.rows();i++)
    {
        for(uint32_t j=0;j<a.cols();j++)
        {
            if(a(i,j)>0)
                a(i,j)=1.0/(1.0+exp(-a(i,j)));
            else
                a(i,j)=exp(a(i,j))/(1.0+exp(a(i,j)));
        }
    }
    return true;
}

//  "identity"
bool _identity(Eigen::Ref<Matrix> a)
{
    return true;
}


//  not implemented
bool _no_implement(Eigen::Ref<Matrix> a)
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
