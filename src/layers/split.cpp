#include "split.h"
#include "iostream"

namespace Tsnn{

void Split::inference()
{
    Matrix input_mat=get_input()[0]->get_value<Matrix>();
    size_t length=input_mat.rows();
    size_t channels=input_mat.cols();

    size_t num_split=get_config<size_t>("num_split");

    size_t out_channels=size_t(channels/num_split);


    for(int i=0;i<num_split;i++)
        get_output()[i]->set_value(input_mat.block(0,out_channels*i,length,out_channels));

}

}
