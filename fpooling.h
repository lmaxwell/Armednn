


#ifndef _FPOOLING_H_
#define _FPOOLING_H_


#include "operator.h"
#include "op_regist.h"
#include "op_sub.h"
#include "node.h"
#include <vector>
#include <string>

namespace Armednn
{

class Fpooling: public Operator{
    public:


            void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)//, State& state)
            {   

                Matrix output;
                output.resize(inputs[0]->get().rows(),inputs[0]->get().cols());

                Matrix init_state;

                if(arm.state("h").has_value())
                {
                    INFO<<"read init state from Arm";
                    init_state=arm.state("h").get();
                }
                else if(inputs.size()==3) 
                {
                    INFO<<"read init state from input";
                    init_state=inputs[2]->get();
                }
                else
                {
                    INFO<<"zero init state";
                    init_state=Matrix::Zero(arm.state("h").shape()[0],arm.state("h").shape()[1]);
                }
                for(uint32_t i=0;i<inputs[0]->get().rows();i++)
                {
                    Matrix f=inputs[0]->get().row(i);
                    //INFO<<"f "<<f.data();
                    OpSub::activation("sigmoid")(f);
                    Matrix z=inputs[1]->get().row(i);
                    OpSub::activation("tanh")(z);
                    if(i==0)
                        output.row(i).array()=f.array() * init_state.array()+
                                         (1-f.array()) * z.array();
                    else
                        output.row(i).array()=f.array() * output.row(i-1).array()+
                                            (1-f.array()) * z.array();
                    init_state=output.row(i);

                }
                arm.state("h").set(output.row(output.rows()-1));
                outputs[0]->set(std::move(output));
            }
};

REGISTER_OP(Fpooling).add_config<uint32_t>("output_channels","output channels")
                     .add_state("h",{"1","output_channels"})
                     .set_num_input("2")
                     .set_num_output("1");

}// namespace Armednn

#endif
