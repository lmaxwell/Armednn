
#include "./dense.h"

namespace Armednn{
void Dense::inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
{   
    auto input=inputs[0]->get();
    outputs[0]->allocate(input.rows(),arm.config<uint32_t>("dim1"));
    auto output=outputs[0]->get();
    output.noalias()=input*arm.param("weight");
    output+=arm.param("bias").replicate(output.rows(),1);
    OpSub::activation(arm.config<std::string>("activation"))(output);
}

}//namespace Armednn
