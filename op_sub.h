
#ifndef _OP_SUB_H
#define _OP_SUB_H

#include "common.h"
#include <iostream>
namespace Armednn{

namespace OpSub{



//Activation function type
typedef std::function<bool (Eigen::Ref<Matrix>)> ActFun;


/*
//get activation function
Matrix a=Matrix::Random(2,2);
activaition("tanh")(a);
*/
ActFun activation(std::string name);

} // OpSub
} // Armednn
#endif

