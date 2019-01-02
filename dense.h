

#ifndef _DENSE_H_
#define _DENSE_H_


#include "operator.h"
#include "op_regist.h"
#include "op_sub.h"
#include "node.h"
#include <vector>
#include <string>

namespace Tsnn
{

class Dense: public Operator{
    public:


            void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
            {   
                Matrix output;
                OpSub::multiply(inputs[0]->get(),arm.param("weight"),output);
                OpSub::add(output,arm.param("bias"));
                OpSub::activation(arm.config<std::string>("activation"))(output);
                outputs[0]->set(std::move(output));
            }
            
};



REGISTER_OP(Dense).add_config<uint32_t>("dim0","dimension 0")
                     .add_config<uint32_t>("dim1","dimension 1")
                     .add_config<std::string>("activation","activation function type")
                     .add_param("weight",{"dim0","dim1"})
                     .add_param("bias",{"1","dim1"})
                     .set_num_input("1")
                     .set_num_output("1");

}// namespace Tsnn

#endif
