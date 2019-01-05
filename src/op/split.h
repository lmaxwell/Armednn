
#ifndef _SPLIT_H_
#define _SPLIT_H_


#include <iostream>
#include <vector>
#include <string>
#include "armednn/operator.h"
#include "armednn/op_regist.h"
#include "armednn/op_sub.h"
#include "armednn/node.h"

namespace Armednn{

class Split: public Operator{

    public:

            void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm);

};

REGISTER_OP(Split).add_config<uint32_t>("num_split","number of split outputs, along last dim")
                     .set_num_input("1")
                     .set_num_output("num_split");


}//namespace Armednn
#endif
