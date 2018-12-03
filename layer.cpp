#include "layer.h"
//initialize factories
std::map<std::string,LayerFactory*> Layer::factories;
//initialize configs
std::map<std::string,std::set<std::string> > Layer::configs_type;



void Layer::get_config(const std::string& name, int& value)
{
        value= atoi(configs[name].c_str());
}
void Layer::get_config(const std::string& name, size_t& value)
{
        value= atoi(configs[name].c_str());
}
void Layer::get_config(const std::string& name, float& value)
{
        value=atof(configs[name].c_str());
}
void Layer::get_config(const std::string& name, std::string& value)
{
        value=configs[name].c_str();
}


Layer* Layer::set_config(const std::string name,const std::string value)
{

    std::set<std::string>::iterator ite=configs_type[this->type].find(name);
    if(ite==configs_type[this->type].end())
    {
        std::cout<<"ERROR: no config name "+ name+" for "+ this->type + " layer "<<std::endl;
        exit(1);
    }
    else
        configs[name]=value;
    return this;
}

Layer* Layer::set_input(std::vector<Node*> inputs)
{
    in_nodes=inputs;
    return this;
}

Layer* Layer::set_input(Node* input)
{
    in_nodes.push_back(input);
    return this;
}

Layer* Layer::add_output()
{
    Node *node=new Node();
    node->from=this;
    node->name=name+"["+std::to_string(out_nodes.size())+"]";
    out_nodes.push_back(node);
}

Layer* Layer::add_output(size_t channels)
{
    Node *node=new Node(channels);
    node->from=this;
    node->name=name+"["+std::to_string(out_nodes.size())+"]";
    out_nodes.push_back(node);
}
