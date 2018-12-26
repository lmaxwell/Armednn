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

bool has_config(const std::string name, const std::string type)
{
    /*
    std::set<std::string>::iterator ite=ConfigsType::get()[type].find(name);
    if(ite==ConfigsType::get()[type].end())
    {
        return false;
    }
    else
    {
        return true;
    }
    */
    return true;
}




template <typename T>
T Layer::get_config(const std::string& name)
{
    //TODO CHECK(
    T tmp;
    CHECK(configs[name].get<T>(tmp))<<"failed to get config: "<<name;;
    return tmp;
}

template size_t Layer::get_config<size_t>(const std::string&);
template int Layer::get_config<int>(const std::string&);
template float Layer::get_config<float>(const std::string&);
template std::string Layer::get_config<std::string>(const std::string&);


Layer* Layer::set_config(const std::string name,const std::string value)
{

    //To do CHECK 
    return this;
}

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

void Layer::add_param( std::string name,size_t rows,size_t cols)
{
    param[name]=Param(name,rows,cols);
}

bool Layer::has_param(std::string name)
{
    auto it=param.find(name);
    if(it==param.end())
        return false;
    else
        return true;
}

Layer* Layer::load_param( std::string name,  Matrix value)
{
    CHECK(has_param(name))<<"no param:\""<<name<<"\""<<" in \""<<type<<"\"!";
    param[name].load(value);
    return this;
}

Matrix& Layer::get_param(std::string name)
{
    CHECK(has_param(name))<<"no param:\""<<name<<"\""<<" in \""<<type<<"\"!";
    return param[name].value;
}


Layer& create(const std::string type,const ConfigMap _configs, const std::string name)
{

    auto ite=Registry::get().regcont.find(type);
    if(ite==Registry::get().regcont.end())
    {
        std::cout<<type<<" not implemented!"<<std::endl;
        exit(1);
    }
    else
    {
         
        //CHECK KEY
        ConfigMap regit=Registry::get().get_layer(type).configs;

        for(auto it=_configs.begin();it!=_configs.end();it++)
        {
            if(regit[it->first].set(it->second.value))
            {
                //CHECK VALUE TYPE
            }
        }

        Layer* layer= Registry::get().get_layer(type).factory->create(regit,name);
        layer->type=type;
        return *layer;
    }
}


}//namespace Tsnn
