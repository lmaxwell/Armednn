
#ifndef _CONCAT_H_
#define _CONCAT_H_


#include <iostream>
#include "operator.h"
#include "op_regist.h"
#include "op_sub.h"
#include "node.h"
#include <vector>
#include <string>

namespace Tsnn
{
class Concat: public Operator{
    public:


            void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
            {   

                Matrix output;
                uint32_t rows=inputs[0]->get().rows();
                uint32_t cols=0;
                for(uint32_t i=0;i<arm.config<uint32_t>("num_input");i++)
                {
                    cols+=inputs[i]->get().cols();
                }
                output.resize(rows,cols);

                cols=0;
                for(uint32_t i=0;i<arm.config<uint32_t>("num_input");i++)
                {
                    output.block(0,cols,rows,inputs[i]->get().cols())=inputs[i]->get();
                    cols+=inputs[i]->get().cols();
                }

                outputs[0]->set(std::move(output));

            }

};

REGISTER_OP(Concat).doc("Concat: split channels along the last axis")
                      .add_config<uint32_t>("num_input","number of inputs")
                      .set_num_input("num_input")
                      .set_num_output("1");

}
#endif
