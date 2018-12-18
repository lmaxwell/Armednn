#include "tsnn/network.h"
namespace Tsnn{

Network::Network(std::vector<pData> _in_nodes,std::vector<pData> _out_nodes){
    bind(_in_nodes,_out_nodes);
}

Network* Network::bind(std::vector<pData> _in_nodes,std::vector<pData> _out_nodes)
{
    in_nodes=_in_nodes;
    out_nodes=_out_nodes;

    for(int i=0;i<out_nodes.size();i++)
    {
        iterate_layers(out_nodes[i]);
    }

    prepare();

    return this;
}

Network* Network::feed(const std::vector<Matrix> &data)
{
    for(int i=0;i<in_nodes.size();i++)
    {
        in_nodes[i]->set_value(data[i]);
    }
    return this;
}


void Network::prepare()
{
    for(auto ite=layers_v.begin();ite!=layers_v.end();ite++)
    {
        (*ite)->prepare();
    }
}


void Network::compute()
{
    for(auto ite=layers_v.begin();ite!=layers_v.end();ite++)
    {
        (*ite)->inference();
    }
}

void Network::iterate_layers(pData node)
{
    std::vector<pData>::iterator ite=std::find(in_nodes.begin(),in_nodes.end(),node);

    if (ite!=in_nodes.end()) 
        return;


    Layer* from=node->from;

    if(std::find(layers_v.begin(),layers_v.end(),from)!=layers_v.end()) 
    {
        return;
    }

    for(int i=0;i<from->get_input().size();i++)
    {
        iterate_layers(from->get_input()[i]);
    }

    layers_v.push_back(from);
}


void Network::print()
{

    for(auto it=layers_v.begin();it!=layers_v.end();it++)
    {
        std::string temp=(*it)->name;
        temp+="\t{ ";
        for(int i=0;i<(*it)->get_output().size();i++)
        {
            temp+=(*it)->get_output()[i]->name;
            if(i!=(*it)->get_output().size()-1)
                temp+=" ";
        }
        temp+=" }\t{ ";
        for(int i=0;i<(*it)->get_input().size();i++)
        {
            temp+=(*it)->get_input()[i]->name;
            if(i!=(*it)->get_input().size()-1)
                temp+=" ";
        }
        temp+=" }";
        std::cout<<temp<<std::endl;
    }
    std::string temp="OUTPUT:\t";
    for(int i=0;i<out_nodes.size();i++)
    {
        temp+=out_nodes[i]->name;
        if(i!=out_nodes.size()-1)
            temp+=" ";
    }
    std::cout<<temp<<std::endl;
}

std::vector<pData>& Network::get_output()
{
    return out_nodes;
}
std::vector<pData>& Network::get_input()
{
    return in_nodes;
}

}
