#include "split.h"
#include "iostream"


void Split::inference()
{
    size_t length=get_input()[0]->get_length();
    size_t channels=get_input()[0]->get_channels();
    size_t num_split=get_config<size_t>("num_split");
    size_t out_channels=size_t(channels/num_split);
    for(int i=0;i<num_split;i++)
        get_output()[i]->data=get_input()[0]->data.block(0,out_channels*i,length,out_channels);
}
