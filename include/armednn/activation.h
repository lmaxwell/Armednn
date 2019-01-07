
#ifndef _ACTIVATION_H
#define _ACTIVATION_H

#include "armednn/common.h"
#include <iostream>
namespace Armednn{




/*
 *  ActFun ( returned by act_functor) is functor used for eigen expression .unaryExpr
 *
 *  ActFunMat (returned by activation) take activation on input Matrix 
 *  
 *  Matrix a;
 *
 *  auto b=a.unaryExpr(act_functor("tanh")) // b is a expression
 *  Matrix c=b // evaluate the expression and store value to c
 *  //using auto is not suggested, it leads to misunderstanding 
 *
 *  // if we need a to be unchanged(e.g. a is from input node, we should not chage it, because other nodes may take it as input too) , use act_functor , or else use activation
 *  c=a.unaryExpr(act_functor("tanh")) // if c is from output node, a is from input node
 *
 *  activation("tanh")(a) // do activatoin on a, value in a is changed; mostly used on output node.
 *
 * */
typedef std::function<float (float)> ActFun;

ActFun act_functor(std::string name);


typedef std::function<bool (Eigen::Ref<Matrix>)> ActFunMat;


ActFunMat activation(std::string name);

} // Armednn
#endif

