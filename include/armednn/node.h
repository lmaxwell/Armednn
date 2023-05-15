

#ifndef _NODE_H
#define _NODE_H


#include <memory>
#include <vector>
#include <unordered_map>
#include "armednn/common.h"
#include "armednn/config.h"
#include "armednn/param.h"

namespace Armednn
{

struct Data
{

    private:
        Matrix _value;
        uint32_t _id=199009;
        
        uint32_t _rows=0; // map _rows
        uint32_t _cols=0; // map _cols

        uint32_t _size=0; //true size

    public:
        

        Data()=default;
        Data(uint32_t id);

        void allocate(uint32_t rows,uint32_t cols);

        Eigen::Map<Matrix,Eigen::Aligned> get();

        uint32_t id();

};


typedef std::vector<std::shared_ptr<Data>> DataPtr;



struct Arm
{
    private:
        ConfigMap _config;
        ParamMap _param;
    public:
        explicit Arm()=default;
        explicit Arm(ConfigMap& config, ParamMap& param);
        explicit Arm(ConfigMap& config);

        template <typename Type>
        Type config(std::string name);
        ConfigMap& config();

        Matrix& param(std::string name);
        ParamMap& param();
        
};


template <typename Type>
Type Arm::config(std::string name)
{
    CHECK(_config.find(name)!=_config.end())
        <<"config: "<<name<<" not found!";
    std::unique_ptr<Type> tmp(new Type());
    CHECK(_config[name].get<Type>(*tmp))
        <<"config: "<<name<<" you provided wrong type";;
    return *tmp;
}

class Operator;

struct Armed
{
    
    private:
        Arm _arm;
        std::unique_ptr<Operator> _op;
        uint32_t _num_input=0;
        uint32_t _num_output=0;
        uint32_t _num_state=0;

    public:
        explicit Armed( Arm& arm,std::unique_ptr<Operator> op);

        ~Armed(){DEBUG<<"Armed destory";}
        
        void run(DataPtr& inputs, DataPtr& outputs);

        uint32_t num_input();
        uint32_t num_output();
        uint32_t num_state();
        std::string name();
        std::string type();


};


typedef std::vector<std::unique_ptr<Armed>> ArmedPtr;



struct Node: public Counter<Node>
{
    private:
        uint32_t _id;
        DataPtr _inputs;
        DataPtr _outputs;
        std::unique_ptr<Armed> _armed;
    public:
        explicit Node(DataPtr& inputs, std::unique_ptr<Armed> armed, DataPtr outputs=DataPtr());

        ~Node(){DEBUG<<"Node destory";}

        void run();

        DataPtr& input();

        DataPtr& output();

        std::shared_ptr<Data>& input(uint32_t index);

        std::shared_ptr<Data>& output(uint32_t index);

        uint32_t id();

        std::string name();
        std::string type();

        void feed(Matrix& in);

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
