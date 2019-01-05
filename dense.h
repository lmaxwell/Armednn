

#ifndef _DENSE_H_
#define _DENSE_H_


#include "operator.h"
#include "op_regist.h"
#include "op_sub.h"
#include "node.h"
#include <vector>
#include <string>

namespace Armednn
{

class Dense: public Operator{
    public:


        void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
        {   
            auto input=inputs[0]->get();
            outputs[0]->allocate(input.rows(),arm.config<uint32_t>("dim1"));
            auto output=outputs[0]->get();
            output.noalias()=input*arm.param("weight");
            output+=arm.param("bias").replicate(output.rows(),1);
            OpSub::activation(arm.config<std::string>("activation"))(output);
        }

};



REGISTER_OP(Dense).add_config<uint32_t>("dim0","dimension 0")
                .add_config<uint32_t>("dim1","dimension 1")
                .add_config<std::string>("activation","activation function type")
                .add_param("weight",{"dim0","dim1"})
                .add_param("bias",{"1","dim1"})
                .set_num_input("1")
                .set_num_output("1");


}// namespace Armednn

#endif

