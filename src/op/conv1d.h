


#ifndef _CONV1D_H_
#define _CONV1D_H_


#include <vector>
#include <string>
#include "armednn/operator.h"
#include "armednn/op_regist.h"
#include "armednn/activation.h"
#include "armednn/node.h"
#include "./im2col.h"


namespace Armednn
{
    



struct  ToColHelper1D
{
       uint32_t length=0;
       uint32_t channels=0;
       uint32_t filter_width=0;
       uint32_t stride=0;
       uint32_t dilation=0;
       std::string padding="same";
       uint32_t left=0;
       uint32_t right=0;
       uint32_t col_length=0;
       uint32_t col_channels=0;


       /* dilation and stride are not supported now 
        */ 
       explicit ToColHelper1D()=default;
       explicit ToColHelper1D(uint32_t _length, uint32_t _channels, uint32_t _filter_width, uint32_t _stride, uint32_t _dilation, std::string _padding):
           length(_length),
           channels(_channels),
           filter_width(_filter_width),
           stride(_stride),
           dilation(_dilation),
           padding(_padding)
        {
         /* calculate: 
          * left and right padding 
          * length and channels of "col" 
          * */
           if (padding=="same")
           {

               left=(filter_width-1)/2;
               right=filter_width-1-left;
               col_length=length;
           }
           else if(padding=="valid"){
               col_length=length-(filter_width-1);
           }
           else
           {
               INFO<<"padding "<<padding<<" not implemented!";
           }

           col_channels=filter_width*channels;
        }


       void process(float *in, float* out)
       {
           Im2Col3dNHWC(
                   channels,//C
                   1,//T
                   1,//H
                   length,//W
                   1,//kernel t
                   1,//  h
                   filter_width,//  w
                   1,//dilation t
                   1,// h
                   dilation,// w
                   0,// pad_prev
                   0,// pad_top
                   left,// pad_left
                   0,// pad_next
                   0,// pad_bottom
                   right,// pad_right
                   1,// stride t
                   1,// h
                   stride,// w
                   in,
                   out,
                   1);
       }


};

class Conv1d: public Operator{
    public:

        void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm);

    private:
       Data col;
};



std::vector<uint32_t> conv1d_shape_func(ConfigMap& config)
{
    uint32_t filter_width=0;
    uint32_t input_channels=0;
    uint32_t output_channels=0;
    config["filter_width"].get<uint32_t>(filter_width);
    config["input_channels"].get<uint32_t>(input_channels);
    config["output_channels"].get<uint32_t>(output_channels);
    return {filter_width*input_channels, output_channels};
}



REGISTER_OP(Conv1d).add_config<uint32_t>("filter_width","number of filters")
                .add_config<uint32_t>("input_channels","input channels")
                .add_config<uint32_t>("output_channels","output channels")
                .add_config<std::string>("padding","padding: same | valid")
                .add_config<std::string>("activation","activation function type")
                .add_param("weight",conv1d_shape_func) // store as [filter_width  input_channels , output_channels]
                .add_param("bias",{"1","output_channels"})
                .set_num_input("1")
                .set_num_output("1");


}// namespace Armednn

#endif

