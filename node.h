

#ifndef _NODE_H
#define _NODE_H

#include <memory>
#include <unordered_map>
#include "common.h"
#include "config.h"
#include "param.h"

namespace Armednn
{

struct Data
{
    private:

        Matrix _value;
        uint32_t _id=0;

    public:

        Data()=default;
        Data(uint32_t id);
        Matrix& get();
        void set(Matrix&& value);
        uint32_t id();

};


typedef std::vector<std::shared_ptr<Data>> DataPtr;


struct Arm
{
    private:
        ConfigMap _config;
        ParamMap _param;
        StateMap _state;
    public:
        explicit Arm()=default;
        explicit Arm(ConfigMap& config, ParamMap& param);
        explicit Arm(ConfigMap& config);

        template <typename Type>
        Type config(std::string name);
        ConfigMap& config();

        Matrix& param(std::string name);
        ParamMap& param();
        
        State& state(std::string name)
        {
            return _state[name];
        }

        StateMap& state()
        {
            return _state;
        }
};


template <typename Type>
Type Arm::config(std::string name)
{
    CHECK(_config.find(name)!=_config.end())
        <<"config: "<<name<<" not found!";
    Type tmp;
    CHECK(_config[name].get<Type>(tmp))
        <<"config: "<<name<<" you provided wrong type";;
    return tmp;
}

class Operator;

struct Armed
{
    
    private:
        Arm _arm;
        std::unique_ptr<Operator> _op;
        uint32_t _num_input=0;
        uint32_t _num_output=0;

    public:
        explicit Armed( Arm& arm,std::unique_ptr<Operator> op);

        ~Armed(){std::cout<<"Armed destory"<<std::endl;}
        
        void run(DataPtr& inputs, DataPtr& outputs);

        uint32_t num_input();
        uint32_t num_output();
        std::string name();

        void reset()
        {
            for(auto& item:_arm.state())
            {
                _arm.state(item.first).reset();
            }
        }

};


typedef std::vector<std::unique_ptr<Armed>> ArmedPtr;



struct Node: public Counter<Node>
{
    private:
        uint32_t _id;
        DataPtr _inputs;
        DataPtr _outputs;
        std::unique_ptr<Armed> _armed;
        StateMap _state;
    public:
        explicit Node(DataPtr& inputs, std::unique_ptr<Armed> armed);

        ~Node(){std::cout<<"Node destory"<<std::endl;}

        void run(bool reset=true);

        DataPtr& input();

        DataPtr& output();

        std::shared_ptr<Data>& input(uint32_t index);

        std::shared_ptr<Data>& output(uint32_t index);

        uint32_t id();

        std::string name();

        void reset()
        {
            _armed->reset();
        }
};

typedef std::vector<std::unique_ptr<Node>> NodePtr;

/*
 
arm = { config , param } 
 
arm + op =armed; // check arm and op compatible

armed + inputs = node; // node { armed , inputs, outputs }, outputs is auto allocated 

node->run()
{
    armed->run(inputs,outputs)
}


*/
    

}//namespace Armednn

#endif
