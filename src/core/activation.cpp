
#include <map>
#include "armednn/activation.h"



namespace Armednn{


float __tanh(float x)
{
    return std::tanh(x);
}


float __sigmoid(float x)
{
    if(x>=0)
        return 1.0f/(1.0f+std::exp(-x));
    else
    {
        float tmp=std::exp(x);
        return tmp/(tmp+1.0f);
    }
}

float __relu(float x)
{
    if(x>0.0f)
        return x;
    else
        return 0.0f;
}


float __no_impl(float x)
{
    return x;
}


ActFun act_functor(std::string name) 
{
    if(name == "tanh")
    {
        return __tanh; 
    }
    else if(name == "sigmoid")
    {
        return __sigmoid;
    }
    else if(name == "relu")
    {
        return __relu;
    }
    else
    {
        DEBUG<<"act_functor "<<name<<" not implemented!";
        return __no_impl;
    }
}


//  "tanh"
bool _tanh(Eigen::Ref<Matrix> a)
{
    a=a.array().tanh();
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
                a(i,j)=1.0/(1.0+std::exp(-a(i,j)));
            else
            {
                float tmp=std::exp(a(i,j));
                a(i,j)=tmp/(1.0+tmp);
            }
        }
    }
    return true;
}

//softmax on every row
bool _softmax(Eigen::Ref<Matrix> a)
{
    for(uint32_t i=0;i<a.rows();i++)
    {
        float max=-1e+100;
        for(uint32_t j=0;j<a.cols();j++)
        {
            max = max<a(i,j) ? a(i,j) : max;
        }
        for(uint32_t j=0;j<a.cols();j++)
        {
            a(i,j)-=max;
        }
        a.row(i)=a.row(i).array().exp();
        float sum=0.0f;
        for(uint32_t j=0;j<a.cols();j++)
        {
            sum+=a(i,j);
        }
        for(uint32_t j=0;j<a.cols();j++)
        {
            a(i,j)/=sum;
        }
    }
    return true;
}

//relu
bool _relu(Eigen::Ref<Matrix> a)
{
    for(uint32_t i=0;i<a.rows();i++)
    {
        for(uint32_t j=0;j<a.cols();j++)
        {
            if (a(i,j)<0.0f)
                a(i,j)=0.0f;
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
bool _no_impl(Eigen::Ref<Matrix> a)
{
    return false;
}


/*
//get activation function
Matrix a=Matrix::Random(2,2);
activaition("tanh")(a)
*/
ActFunMat activation(std::string name)
{
    if(name == "tanh")
        return _tanh;
    if(name == "identity")
        return _identity;
    if(name == "relu")
        return _relu;
    if(name == "sigmoid")
        return _sigmoid;
    if(name == "softmax")
        return _softmax;

    DEBUG<<"activation "<<name<<" not implemented!";
    return &_no_impl;
}






}// Armednn
