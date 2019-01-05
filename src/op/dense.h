

#ifndef _DENSE_H_
#define _DENSE_H_


#include "armednn/operator.h"
#include "armednn/op_regist.h"
#include "armednn/op_sub.h"
#include "armednn/node.h"
#include <vector>
#include <string>

namespace Armednn
{

class Dense: public Operator{
    public:


        void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm);

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

