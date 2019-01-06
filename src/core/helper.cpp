
#include "armednn/helper.h"

namespace Armednn
{


std::unique_ptr<Operator> make_op(std::string type,std::string name)
{
    return Registry::get().get_op(type).create(name);
}


std::unique_ptr<Armed> make_armed(Arm& arm, std::unique_ptr<Operator> op)
{
    return std::unique_ptr<Armed>(new Armed(arm,std::move(op)));
}


std::unique_ptr<Node> make_node(DataPtr& inputs, std::unique_ptr<Armed> armed)
{
    return std::unique_ptr<Node>(new Node(inputs,std::move(armed)));
}


std::unique_ptr<Node> make_node(DataPtr&& inputs, std::unique_ptr<Armed> armed)
{
    return std::unique_ptr<Node>(new Node(inputs,std::move(armed)));
}


std::unique_ptr<Node> make_input(std::string name)
{
    auto null=make_op("_Null_",name);
    Arm arm;
    auto armed=make_armed(arm,std::move(null));
    DataPtr inputs;
    return make_node(inputs,std::move(armed));
}

std::unique_ptr<Node> make_state(std::string name, Matrix value )
{
    auto op=make_op("_Const_",name);
    ConfigMap config;
    ParamMap param;
    param.insert({"value",{value}});
    Arm arm(config,param);
    auto armed=make_armed(arm,std::move(op));
    DataPtr inputs;
    return make_node(inputs,std::move(armed));
}

}//namespace Armednn
