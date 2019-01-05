

#ifndef _OP_SPCECIAL_H_
#define _OP_SPCECIAL_H_


#include "armednn/operator.h"
#include "armednn/op_regist.h"
#include "armednn/op_sub.h"
#include "armednn/node.h"
#include <vector>
#include <string>

namespace Armednn
{


class _Null_: public Operator{
    public:
        void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
        {   
        }
};
REGISTER_OP(_Null_).set_num_input("0")
                  .set_num_output("1");


class _Copy_: public Operator{
    public:
        void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
        {   
            auto input=inputs[0]->get();
            outputs[0]->allocate(input.rows(),input.cols());
            auto output=outputs[0]->get();
            output=input;
        }
};
REGISTER_OP(_Copy_).set_num_input("1")
                  .set_num_output("1");



class _Zero_: public Operator{
    public:
        void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
        {   
        }
};
REGISTER_OP(_Zero_).set_num_input("1")
                   .set_num_output("1");




}// namespace Armednn

#endif

