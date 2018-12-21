#include "tsnn/network.h"
namespace Tsnn{

Network::Network(std::vector<pData> _in_nodes,std::vector<pData> _out_nodes){
    bind(_in_nodes,_out_nodes);
}

Network::~Network()
{
    std::for_each(layers_v.begin(),layers_v.end(),[](Layer *p){delete p;p=nullptr;});
    std::for_each(in_nodes.begin(),in_nodes.end(),[](Data *p){delete p;p=nullptr;});
    std::cout<<"delete network"<<std::endl;
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

std::vector<pData> Network::Input()
{
    Data *p =new Data("input");
    in_nodes.push_back(p);
    std::vector<pData> vtmp;
    vtmp.emplace_back(p);
    return vtmp;
}

void Network::Output(std::vector<pData> outs)
{
    out_nodes=outs;

    for(int i=0;i<out_nodes.size();i++)
    {
        iterate_layers(out_nodes[i]);
    }

    prepare();
}

Layer* Network::get_layer(std::string name)
{
    auto it=std::find_if(layers_v.begin(),layers_v.end(),
            [name](Layer * l){return l->name==name;});
    CHECK(it!=layers_v.end())<<"cannot find layer: "<<name;
    return *it;
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
