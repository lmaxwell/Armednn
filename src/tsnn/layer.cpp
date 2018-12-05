#include "tsnn/layer.h"

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



Layer* Layer::create(const std::string &type, const Config _config, const std::string &name)
{

    fmap::iterator ite=factories.get().find(type);
    if(ite==factories.get().end())
    {
        //exception
        std::cout<<type<<" not implemented!"<<std::endl;
        return nullptr;
    }
    else
    {
        Layer* layer= factories.get()[type]->create(_config,name);
        layer->type=type;
        return layer;
    }
}


template <typename T>
T Layer::get_config(const std::string& name)
{
    return config.get<T>(name);
}

template size_t Layer::get_config<size_t>(const std::string&);
template int Layer::get_config<int>(const std::string&);
template float Layer::get_config<float>(const std::string&);
template std::string Layer::get_config<std::string>(const std::string&);


Layer* Layer::set_config(const std::string name,const std::string value)
{

    std::set<std::string>::iterator ite=configs_type.get()[this->type].find(name);
    if(ite==configs_type.get()[this->type].end())
    {
        //TODO:exception handler
        std::cout<<"ERROR: no config name "+ name+" for "+ this->type + " layer "<<std::endl;
        exit(1);
    }
    else
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

template Layer* Layer::add_output<std::string>();
template Layer* Layer::add_output<Matrix>();

