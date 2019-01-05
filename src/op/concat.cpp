#include "./concat.h"

namespace Armednn
{

void Concat::inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
{   


    uint32_t rows=inputs[0]->get().rows();
    uint32_t cols=0;
    for(uint32_t i=0;i<arm.config<uint32_t>("num_input");i++)
    {
        cols+=inputs[i]->get().cols();
    }

    outputs[0]->allocate(rows,cols);
    auto output=outputs[0]->get();

    cols=0;
    for(uint32_t i=0;i<arm.config<uint32_t>("num_input");i++)
    {
        output.block(0,cols,rows,inputs[i]->get().cols())=inputs[i]->get();
        cols+=inputs[i]->get().cols();
    }

}

}//namespace Armednn
