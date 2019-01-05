#include "./fpooling.h"

namespace Armednn
{

void Fpooling::inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)//, State& state)
{   

    outputs[0]->allocate(inputs[0]->get().rows(),inputs[0]->get().cols());
    auto output=outputs[0]->get();

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
        INFO<<"f "<<f;
        OpSub::activation("sigmoid")(f);
        INFO<<"f "<<f;
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
}

}// namespace Armednn
