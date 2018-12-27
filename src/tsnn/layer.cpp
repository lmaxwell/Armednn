#include "tsnn/layer.h"

namespace Tsnn{

Layer::Layer(ConfigMap _configs,std::string _name):configs(_configs),name(_name)
{

}

Layer::~Layer()
{
    std::for_each(out_nodes.begin(),out_nodes.end(),[](pData p){delete p;p=nullptr;});
    std::cout<<"delete layer"<<std::endl;
}

std::vector<pData>& Layer::operator()(std::vector<pData> inputs)
{
    in_nodes=inputs;
    return out_nodes;
}




/*
Layer* Layer::set_config(const std::string name,const std::string value)
{

    //To do CHECK 
    return this;
}
*/

Layer* Layer::set_input(std::vector<pData> inputs)
{
    in_nodes=inputs;
    return this;
}

Layer* Layer::set_input(pData input)
{
    in_nodes.push_back(input);
    return this;
}


Layer* Layer::add_output()
{
    pData node=new Data();
    node->from=this;
    node->name=name+"["+std::to_string(out_nodes.size())+"]";
    out_nodes.push_back(node);
    return this;
}

void Layer::add_param( std::string name,size_t rows, size_t cols)
{
    params[name]=Param(name,rows,cols);
}

bool Layer::has_param(std::string name)
{
    auto it=params.find(name);
    if(it==params.end())
        return false;
    else
        return true;
}

Layer* Layer::load_param( std::string name,  Matrix value)
{
    CHECK(has_param(name))<<"no param:\""<<name<<"\""<<" in \""<<type<<"\"!";
    params[name].load(value);
    return this;
}

Matrix& Layer::get_param(std::string name)
{
    CHECK(has_param(name))<<"no param:\""<<name<<"\""<<" in \""<<type<<"\"!";
    return params[name].value;
}


Layer& Layer::create(const std::string type,const ConfigMap _configs, const std::string name)
{

    auto ite=Registry::get().regcont.find(type);
    if(ite==Registry::get().regcont.end())
    {
        std::cout<<type<<" not implemented!"<<std::endl;
        exit(1);
    }
    else
    {
         
        ConfigMap regit_configs=Registry::get().get_layer(type).configs;

        CHECK(config_compare_key(regit_configs,_configs))<<"config key not match";
        CHECK(config_compare_type(regit_configs,_configs))<<"config type not match";

        ParamMap params=Registry::get().get_layer(type).params;

        for(auto it=params.begin();it!=params.end();it++)
        {
            if (it->second.shape_mapping.size()>0)
            {
                params[it->first].shape=ShapeMapping(it->second.shape_mapping)(const_cast<ConfigMap&>(_configs));
            }
        }

        Layer* layer= Registry::get().get_layer(type).factory->create(_configs,name);

        layer->type=type;

        layer->params=params;

        std::string num_input=Registry::get().get_layer(type).num_input;
        std::string num_output=Registry::get().get_layer(type).num_output;
        layer->set_num_input(InOutMapping(num_input)(const_cast<ConfigMap&>(_configs)));
        layer->set_num_output(InOutMapping(num_output)(const_cast<ConfigMap&>(_configs)));

        return *layer;
    }
}


}//namespace Tsnn
