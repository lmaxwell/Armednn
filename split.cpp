#include "split.h"
#include "iostream"

std::vector<Node *> Split::get_output()
{
    if(out_nodes.size()==0)
    {
        size_t num_split;
        get_config("num_split",num_split);
        for(int i=0;i<num_split;i++)
        {
            add_output(size_t(in_nodes[0]->channels/num_split));
        }
    }
    return out_nodes;
}

void Split::inference()
{
    size_t length=in_nodes[0]->get_length();
    size_t channels=in_nodes[0]->get_channels();
    size_t num_split;
    get_config("num_split",num_split);
    size_t out_channels=size_t(channels/num_split);
    for(int i=0;i<num_split;i++)
        out_nodes[i]->data=in_nodes[0]->data.block(0,out_channels*i,length,out_channels);
}
