
#include "operator.h"
#include "op_regist.h"
#include "node.h"

namespace Armednn
{

Data::Data(uint32_t id):_id(id){

}

void Data::allocate(uint32_t rows,uint32_t cols)
{
    if(rows*cols<=(uint32_t)_value.size())   
    {
        _rows=rows;
        _cols=cols;
    }
    else
    {
        _value.resize(rows,cols);
        DEBUG<<"allocate(id="<<_id<<") "<<_value.rows()<<" "<<_value.cols();
        _rows=rows;
        _cols=cols;
    }
}

Eigen::Map<Matrix,Eigen::Aligned> Data::get()
{
    return Eigen::Map<Matrix,Eigen::Aligned>(_value.data(),_rows,_cols);
}


uint32_t Data::id()
{
    return _id;
}

Arm::Arm(ConfigMap& config, ParamMap& param):_config(config),_param(param){

}

Arm::Arm(ConfigMap& config):_config(config){

}

ConfigMap& Arm::config()
{
    return _config;
}

ParamMap& Arm::param()
{
    return _param;
}

Matrix& Arm::param(std::string name)
{
    CHECK(_param.find(name)!=_param.end())
        <<"param: "<<name<<" not found!";
    return _param[name].value;
}


Armed::Armed( Arm& arm,std::unique_ptr<Operator> op):
    _arm(arm),
    _op(std::move(op)){

        auto config_regit=Registry::get().get_op(_op->type()).config;

        // check config key 
        CHECK(compare_key<Config>(config_regit,_arm.config())==1)
            <<"Config wrong(key not match)!\t"
            <<_op->name();          

        // check type
        for(auto& item:_arm.config())
        {
            CHECK(config_regit[item.first].set(item.second.value))
                <<"Config wrong(type not match)!\t"
                <<_op->name()<<" "<<item.first;
        }

        auto param_regit=Registry::get().get_op(_op->type()).param;
        // check param key
        CHECK(compare_key<Param>(param_regit,_arm.param())==1)
            <<"Param wrong(key not match)!\t"
            <<_op->name();

        // check param shape
        for(auto& item: _arm.param())
        {
            if(param_regit[item.first].shape_mapping.size()>0)
            {
                auto shape=ParamHelp::ShapeMapping(param_regit[item.first].shape_mapping)(config_regit);
                CHECK(shape==item.second.shape())
                    <<"Shape not match!\t"
                    <<_op->name()<<" "<<item.first;
            }
        }

        // num_input & num_output
        std::string  num_input_mapping=Registry::get().get_op(_op->type()).num_input;
        std::string  num_output_mapping=Registry::get().get_op(_op->type()).num_output;
        _num_input=ConfigHelp::InOutMapping(num_input_mapping)(_arm.config());
        _num_output=ConfigHelp::InOutMapping(num_output_mapping)(_arm.config());

        auto state_regit=Registry::get().get_op(_op->type()).state;

        // add state to arm
        _arm.state()=state_regit;
        for(auto& item: _arm.state())
        {
            _arm.state(item.first).set_shape(ParamHelp::ShapeMapping(state_regit[item.first].shape_mapping)(config_regit));
        }
}


void Armed::run(DataPtr& inputs, DataPtr& outputs)
{
    _op->inference(inputs,outputs,_arm);
}

uint32_t Armed::num_input()
{
    return _num_input;
}

uint32_t Armed::num_output()
{
    return _num_output;
}

std::string Armed::name()
{
    return _op->name();
}

Node::Node(DataPtr& inputs, std::unique_ptr<Armed> armed):_inputs(inputs),_armed(std::move(armed)){


    _id=objects_created;

    //check input size
    CHECK(inputs.size()==_armed->num_input())
        <<"Input size not match\t"
        <<_armed->name();
    
    //allocate output 
    for(uint32_t i=0;i<_armed->num_output();i++)
        _outputs.push_back(std::unique_ptr<Data>(new Data(_id)));
}


void Node::run(bool reset)
{
    if(reset)
        _armed->reset();
    _armed->run(_inputs,_outputs);
}

DataPtr& Node::input()
{
    return _inputs;
}

DataPtr& Node::output()
{
    return _outputs;
}

std::shared_ptr<Data>& Node::input(uint32_t index)
{
    CHECK(index<_inputs.size());
    return _inputs[index];
}

std::shared_ptr<Data>& Node::output(uint32_t index)
{
    CHECK(index<_outputs.size());
    return _outputs[index];
}

uint32_t Node::id()
{
    return _id;
}
std::string Node::name()
{
    return _armed->name();
}



}
