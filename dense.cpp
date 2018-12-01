#include "dense.h"
#include "iostream"

std::vector<Node *> Dense::get_output()
{
    add_output();
    return out_nodes;
}

void Dense::inference()
{
    size_t length=in_nodes[0]->get_length();
    size_t channels=in_nodes[0]->get_channels();
    size_t num_split;
    get_config("num_split",num_split);
    size_t out_channels=size_t(channels/num_split);
    for(int i=0;i<num_split;i++)
        out_nodes[i]->data=in_nodes[0]->data.block(0,out_channels*i,length,out_channels);
}
