#include "./conv1d.h"
namespace Armednn
{




void Conv1d::inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)
{
    auto input=inputs[0]->get();
    ToColHelper helper(input.rows(),
                input.cols(),
                arm.config<uint32_t>("filter_width"),
                1,
                1,
                arm.config<std::string>("padding"));

    Timer timer;

    timer.reset();
    col.allocate(helper.col_length,helper.col_channels);
    INFO<<"allocate cost:"<<timer.elapsed();


    timer.reset();
    helper.process(input.data(),col.get().data());
    INFO<<"to col cost:"<<timer.elapsed();


    outputs[0]->allocate(input.rows(),arm.config<uint32_t>("output_channels"));
    auto output=outputs[0]->get();
    
    timer.reset();
    output.noalias()=col.get()*arm.param("weight");
    INFO<<"gemm cost"<<timer.elapsed();
    output+=arm.param("bias").replicate(output.rows(),1);
    activation(arm.config<std::string>("activation"))(output);

}
}//Armednn
