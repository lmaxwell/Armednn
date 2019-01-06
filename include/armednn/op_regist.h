
#ifndef _OP_REGIST_H_
#define _OP_REGIST_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include <functional>
#include <memory>

#include "armednn/common.h"
#include "armednn/config.h"
#include "armednn/param.h"
#include "armednn/operator.h"


namespace Armednn{


struct RegContent
{
    typedef std::function< std::unique_ptr<Operator> (std::string )> CreateFun;
    std::string _doc="None";
    CreateFun create;
    ConfigMap config;
    ParamMap param;
    StateMap state;
    std::string num_input="?";
    std::string num_output="?";
    std::string num_state="0";



    RegContent& set_factory(CreateFun f)
    {
        create=f;
        return *this;
    }

    RegContent& doc(std::string __doc)
    {
        _doc=__doc;
        return *this;
    }

    template <typename T>
    RegContent& add_config(std::string name,std::string desc)
    {
        std::unique_ptr<T> tmp(new T());
        config.insert({name,Config(name,desc,*tmp)});
        return *this;
    }

    RegContent& add_param(std::string name, std::vector<std::string> shape_mapping=std::vector<std::string>())
    {
        param[name]=Param(shape_mapping);
        return *this;
    }

    RegContent& add_state(std::string name, std::vector<std::string> shape_mapping=std::vector<std::string>())
    {
        state[name]=State(shape_mapping);
        return *this;
    }

    RegContent& set_num_input(std::string _num_input)
    {
        num_input=_num_input;
        return *this;
    }

    RegContent& set_num_output(std::string _num_output)
    {
        num_output=_num_output;
        return *this;
    }

    RegContent& set_num_state(std::string _num_state)
    {
        num_state=_num_state;
        return *this;
    }

};


struct Registry
{
    std::map<std::string,RegContent> regcont;
    Registry(const Registry&)=delete; 
    Registry(Registry&&)=delete; 
    Registry& operator=(const Registry&)=delete; 
    Registry& operator=(Registry&&)=delete; 


    static Registry& get()
    {
        static Registry reg;
        return reg;
    }

    Registry& set_op(std::string name)
    {
        if(regcont.find(name)!=regcont.end())
        {
            DEBUG<<name<<" already registred!";
            return *this;
        }
        DEBUG<<"registing "<<name;
        RegContent tmp;
        regcont[name]=tmp;
        DEBUG<<"registing "<<name<<" done";
        return *this;
    }

    RegContent& get_op(std::string name)
    {
        CHECK(regcont.find(name)!=regcont.end())
            <<name<<" not implemented!";
        return regcont[name];
    }

    private:
        Registry()=default;
        ~Registry()=default;

};



template <typename Op>
class Creator
{
    public:
        std::string _type;
        Creator(std::string type):_type(type){}
        std::unique_ptr<Operator> operator()(std::string name)
        {
            std::unique_ptr<Operator>op(new Op());
            op->type(_type);
            op->name(name);
            return op;
        }
};



#define REGISTER_OP(Op) \
    static Creator<Op>  g_##Op##OpCreator(#Op); \
    static RegContent reg##Op=Registry::get().set_op(#Op).get_op(#Op).set_factory( g_##Op##OpCreator ) 

} //namespace Armednn
#endif
