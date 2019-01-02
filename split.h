
#ifndef _SPLIT_H_
#define _SPLIT_H_


#include <iostream>
#include "operator.h"
#include "op_regist.h"
#include "op_sub.h"
#include "node.h"
#include <vector>
#include <string>

namespace Armednn{

class Split: public Operator{

    public:
            Split():Operator()
            {
            }


            void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
            {   

                Matrix output;
                Matrix& input=inputs[0]->get();
                uint32_t out_channels=uint32_t(input.cols()/arm.config<uint32_t>("num_split"));
                for(uint32_t i=0;i<arm.config<uint32_t>("num_split");i++)
                {
                    output=input.block(0,out_channels*i,input.rows(),out_channels);
                    outputs[i]->set(std::move(output));
                }

            }
};

REGISTER_OP(Split).add_config<uint32_t>("num_split","number of split outputs, along last dim")
                     .set_num_input("1")
                     .set_num_output("num_split");


}
#endif
