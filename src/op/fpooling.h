


#ifndef _FPOOLING_H_
#define _FPOOLING_H_


#include <vector>
#include <string>
#include "armednn/operator.h"
#include "armednn/op_regist.h"
#include "armednn/op_sub.h"
#include "armednn/node.h"

namespace Armednn
{

class Fpooling: public Operator{
    public:


            void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm);
};

REGISTER_OP(Fpooling).add_config<uint32_t>("output_channels","output channels")
                     .set_num_input("2")
                     .set_num_output("1")
                     .set_num_state("1");

}// namespace Armednn

#endif
