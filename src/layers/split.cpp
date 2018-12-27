#include "split.h"
#include "iostream"

namespace Tsnn{

void Split::prepare()
{
    CHECK(get_config("num_split",num_split));
}

void Split::inference()
{
    Matrix &input_mat=get_input()[0]->get_value();


    size_t out_channels=size_t(input_mat.cols()/num_split);


    for(size_t i=0;i<num_split;i++)
        get_output()[i]->set_value(input_mat.block(0,out_channels*i,input_mat.rows(),out_channels));

}

}
