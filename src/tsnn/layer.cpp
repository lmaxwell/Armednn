#include "tsnn/layer.h"

namespace Tsnn{
/*=============================
 layer factories registery 
*/
typedef std::map<std::string,LayerFactory*> fmap;  
typedef std::map<std::string,std::set<std::string>> confmap;

struct Factories{
    static fmap &get(){
        static fmap factories;
        return factories;
    }
} factories;

struct Configs_Type{
    static confmap &get(){
        static confmap configs_type;
        return configs_type;
    }
} configs_type;


void Layer::register_layer(const std::string& name, LayerFactory *factory)
{
    fmap::iterator ite=factories.get().find(name);
    if(ite==factories.get().end())
        factories.get()[name]=factory;
    else
    {
        //throw a exception
        std::cout<<name<<" already registered!"<<std::endl;
    }

}

void Layer::register_config(const std::string& layer_name, const std::string& config_names)
{
    std::stringstream ss(config_names);
    std::string config_name;
    while(ss>>config_name)
    {
        configs_type.get()[layer_name].insert(config_name);
    }
}
/*=============================*/

Layer::Layer(Config _config,std::string _name):config(_config),name(_name)
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
    std::set<std::string>::iterator ite=configs_type.get()[type].find(name);
    if(ite==configs_type.get()[type].end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

Layer& Layer::create(const std::string &type, Config _config, const std::string &name)
{

    fmap::iterator ite=factories.get().find(type);
    if(ite==factories.get().end())
    {
        //exception
        std::cout<<type<<" not implemented!"<<std::endl;
        exit(1);
    }
    else
    {
        for(auto it=_config.getall().begin();it!=_config.getall().end();it++)
        {
            
            CHECK(has_config(it->first,type))<<"no config:\""<<it->first<<"\" in "<<type<<"!";
        }
        for(auto it=configs_type.get()[type].begin();it!=configs_type.get()[type].end();it++)
        {
            CHECK(_config.has(*it))<<"config:\""<<*it<<"\" in "<<type<<" is not set!";
        }
        Layer* layer= factories.get()[type]->create(_config,name);
        layer->type=type;
        INFO<<"adrress of "<<name<<":"<<layer;
        return *layer;
    }
}




template <typename T>
T Layer::get_config(const std::string& name)
{
    CHECK(config.has(name))<<"no config:"<<name;
    return config.get<T>(name);
}

template size_t Layer::get_config<size_t>(const std::string&);
template int Layer::get_config<int>(const std::string&);
template float Layer::get_config<float>(const std::string&);
template std::string Layer::get_config<std::string>(const std::string&);


Layer* Layer::set_config(const std::string name,const std::string value)
{

    CHECK(has_config(name,type))<<"no config:"<<name<<" in "<<type;
    config.set(name,value);
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


template <typename T>
Layer* Layer::add_output()
{
    pData node=new Data<T>();
    node->from=this;
    node->name=name+"["+std::to_string(out_nodes.size())+"]";
    out_nodes.push_back(node);
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

/*
Layer* Layer::load_param( std::string name, float* value)
{
    param[name].load(value);
    return this;
}
*/


template Layer* Layer::add_output<std::string>();
template Layer* Layer::add_output<Matrix>();

}
