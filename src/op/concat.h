
#ifndef _CONCAT_H_
#define _CONCAT_H_


#include <iostream>
#include "armednn/operator.h"
#include "armednn/op_regist.h"
#include "armednn/activation.h"
#include "armednn/node.h"
#include <vector>
#include <string>

namespace Armednn
{

class Concat: public Operator{
    public:
        void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm);

};

REGISTER_OP(Concat).doc("Concat: split channels along the last axis")
                      .add_config<uint32_t>("num_input","number of inputs")
                      .set_num_input("num_input")
                      .set_num_output("1");

}
#endif
