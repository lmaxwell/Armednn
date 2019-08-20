


#ifndef _LSTM_H_
#define _LSTM_H_


#include <vector>
#include <string>
#include "armednn/operator.h"
#include "armednn/op_regist.h"
#include "armednn/activation.h"
#include "armednn/node.h"


namespace Armednn
{
    

class LSTM: public Operator{
    public:

        void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm);
    private:
        Data XW;
        Data HR;
};



std::vector<uint32_t> lstm_weightW_func(ConfigMap& config)
{
    uint32_t input_channels=0;
    uint32_t output_channels=0;
    bool bidirection=false;
    config["input_channels"].get<uint32_t>(input_channels);
    config["output_channels"].get<uint32_t>(output_channels);
    config["bidirection"].get<bool>(bidirection);

    auto rows=input_channels;
    auto cols=output_channels*4;
    if(bidirection)
    {
        output_channels /= 2;
        cols=output_channels*4*2;
    }
    return {rows, cols};
}

std::vector<uint32_t> lstm_weightR_func(ConfigMap& config)
{
    uint32_t input_channels=0;
    uint32_t output_channels=0;
    bool bidirection=false;
    config["input_channels"].get<uint32_t>(input_channels);
    config["output_channels"].get<uint32_t>(output_channels);
    config["bidirection"].get<bool>(bidirection);

    uint32_t rows=output_channels;
    uint32_t cols=output_channels*4;
    if(bidirection)
    {
        output_channels /= 2;
        rows=output_channels*2;
        cols=output_channels*4;
    }
    return {rows, cols};
}


std::vector<uint32_t> lstm_bias_func(ConfigMap& config)
{
    uint32_t output_channels=0;
    config["output_channels"].get<uint32_t>(output_channels);

    uint32_t rows=1;
    uint32_t cols=output_channels*4;
    return {rows, cols};
}

/*
    if unidirection:
        weight_W stored as [Wi,Wc,Wf,Wo]
        weight_R stored as [Ri,Rc,Rf,Ro]
        bias stored as [b]
    if unidirection:
        weight_W stored as [Wi_fw,Wc_fw,Wf_fw,Wo_fw,Wi_bw,Wc_bw,Wf_bw,Wo_bw]
        weight_R stored as [[Ri_fw,Rc_fw,Rf_fw,Ro_fw],
                            [Ri_bw,Rc_bw,Rf_bw,Ro_bw]]
        bias stored as [b_fw,b_bw]

*/

REGISTER_OP(LSTM).add_config<uint32_t>("input_channels","input channels")
                .add_config<uint32_t>("output_channels","output channels")
                .add_config<bool>("bidirection","bidirection: true | false")
                .add_param("weightW",lstm_weightW_func) 
                .add_param("weightR",lstm_weightR_func) 
                .add_param("bias",lstm_bias_func)
                .set_num_input("1")
                .set_num_output("1")
                .set_num_state("2");


}// namespace Armednn

#endif

