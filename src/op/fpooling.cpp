#include "./fpooling.h"

namespace Armednn
{

void Fpooling::inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
{   

    outputs[0]->allocate(inputs[0]->get().rows(),inputs[0]->get().cols());
    auto output=outputs[0]->get();

    auto state=inputs[2]->get();
    INFO<<"state:"<<std::endl<<state;

    for(uint32_t i=0;i<inputs[0]->get().rows();i++)
    {
        Matrix f=inputs[0]->get().row(i); // memory allocate, should avoid
        activation("sigmoid")(f);
        Matrix z=inputs[1]->get().row(i); // ..
        activation("tanh")(z);
        if(i==0)
            output.row(i).array()=f.array() * state.array()+
                (1-f.array()) * z.array();
        else
            output.row(i).array()=f.array() * output.row(i-1).array()+
                (1-f.array()) * z.array();
    }
    state=output.row(output.rows()-1);
}

}// namespace Armednn
