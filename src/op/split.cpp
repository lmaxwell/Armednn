#include "./split.h"
namespace Armednn{


void Split::inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
{   

    auto input=inputs[0]->get();
    CHECK(input.cols()%arm.config<uint32_t>("num_split")==0);
    uint32_t out_channels=uint32_t(input.cols()/arm.config<uint32_t>("num_split"));
    for(uint32_t i=0;i<arm.config<uint32_t>("num_split");i++)
    {
        outputs[i]->allocate(input.rows(),out_channels);
        outputs[i]->get()=input.block(0,out_channels*i,input.rows(),out_channels);
    }
}


}//namespace Armednn
