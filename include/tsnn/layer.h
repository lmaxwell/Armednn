
#ifndef _Layer_H_
#define _Layer_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include <functional>

#include "tsnn/data.h"
#include "tsnn/config.h"
#include "tsnn/core.h"


namespace Tsnn{

class Param
{
    public:
        Param(){};

        Param(std::string _name,size_t _rows,size_t _cols):name(_name),rows(_rows),cols(_cols){};

        std::string name;

        size_t rows;
        size_t cols;

        Matrix value;

        void load(Matrix _value)
        {
            CHECK(_value.rows()==rows && _value.cols()==cols)<<"loading Matrix has wrong dimension";
            value=_value;

        }

        void load(Vector _value)
        {
            CHECK(_value.rows()==rows && _value.cols()==cols)<<"loading Matrix has wrong dimension";
            value=_value;
        }


};

typedef  std::map<std::string,Param> Params;

class Layer;
class LayerFactory
{
    public:
        virtual Layer *create(Config config,std::string name)=0;
};


class Layer{ 
    protected:

        explicit Layer(Config _config,std::string _name);

        void add_param(std::string name, size_t rows, size_t cols);

        bool has_param(std::string name);

        void set_num_input(size_t num)
        {
            num_input=num;
        }

        template <typename T>
        void set_num_output(size_t num)
        {
            for(int i=0;i<num;i++)
                add_output<T>();
            num_output=num;
        }


    public:

        
        virtual ~Layer();

        virtual void inference(){};

        virtual void prepare(){};

        std::string type;

        std::string name;

        std::vector<pData>& operator()(std::vector<pData> inputs);

        Layer* set_input(std::vector<pData> inputs);

        Layer* set_input(pData input);

        template <typename T>

        T get_config(const std::string& name);
        Layer* set_config(const std::string name,const std::string value);

        std::vector<pData>& get_output(){return out_nodes;};
        std::vector<pData>& get_input(){return in_nodes;};

        static Layer& create(const std::string &type, const Config _config, const std::string &name);


        Layer* load_param( std::string name,  Matrix value);

        Matrix& get_param(std::string name);
        

    private:



        Config config;

        Params param;

        size_t num_input;
        size_t num_output;
        //input nodes of the layer
        std::vector<pData> in_nodes;
        //output nodes of the layer
        std::vector<pData> out_nodes;

        template <typename T>
        Layer* add_output();

};

typedef std::map<std::string,LayerFactory*> Fmap;  
typedef std::map<std::string,std::set<std::string>> Confmap;
//typedef std::map<std::string,Param> Parammap;
typedef std::map<std::string,std::string> Parammap;

struct Factories{
    static Fmap &get(){
        static Fmap factories;
        return factories;
    }
};

struct ConfigsType{
    static Confmap &get(){
        static Confmap configs_type;
        return configs_type;
    }
};

namespace{//internal


void str2(const std::string& value1, int& value2)
{
        value2= atoi(value1.c_str());
}
void str2(const std::string& value1, size_t& value2)
{
        value2= atoi(value1.c_str());
}
void str2(const std::string& value1, float& value2)
{
        value2=atof(value1.c_str());
}
void str2(const std::string& value1, std::string& value2)
{
        value2=value1.c_str();
}
void str2(const std::string& value1, bool& value2)
{
        if(value1=="ture")
            value2=true;
        else if (value1=="false")
            value2=false;
}

}// internal

struct Config_
{

    template <typename T>
    T get()
    {
        T tmp_value;
        str2(value,tmp_value);
        return  tmp_value;
    }
    std::string name;
    std::string desc;
    std::string value;
};

struct RegContent
{

    std::string name;


    RegContent& set_factory(LayerFactory *_f)
    {
        factory=_f;
        return *this;
    }

    RegContent& add_config(std::string name)
    {
        configs.add(name);
        return *this;
    }

    RegContent& add_param(std::string name,std::string desc)
    {
        params[name]=desc;
        return *this;
    }


    
    typedef std::map<std::string,Config> Confmap;
    //typedef std::map<std::string,Param> Parammap;
    typedef std::map<std::string,std::string> Parammap;

    LayerFactory* factory;
    Config configs;
    Parammap params;

};


struct Registry
{
    static Registry& get()
    {
        static Registry reg;
        return reg;
    }

    Registry& set_layer(std::string name)
    {
        if(regcont.find(name)!=regcont.end())
        {
            std::cout<<name<<" already registred!"<<std::endl;
            return *this;
        }
        RegContent tmp;
        regcont[name]=tmp;
        return *this;
    }

    RegContent& get_layer(std::string name)
    {
        return regcont[name];
    }

    std::map<std::string,RegContent> regcont;

};

//Layer::register_layer(#Type, this); 
//

template <typename T>
class Factory:public LayerFactory
{
    public:
        Layer* create(Config config,std::string name)
        {
            return new T(config,name);
        }
};

#define REGISTER_LAYER(Type) \
    static Factory<Type>  g_##Type##LayerFactory; \
    static RegContent reg##Type=Registry::get().set_layer(#Type).get_layer(#Type).set_factory(&( g_##Type##LayerFactory )) 

/*
#define REGISTER_LAYER(Type,ConfigNames) \
    class Type##Factory : public LayerFactory { \
        public: \
                Type##Factory() \
        { \
            DEBUG<<"registering "<<#Type;\
            DEBUG<<"register "<<#Type<<" done";\
            Layer::register_config(#Type, #ConfigNames); \
        } \
        virtual Layer *create(Config _config,std::string name) { \
            return new Type(_config,name); \
        } \
    }; \
static Type##Factory global_##Type##Factory;\
;
//static RegContent reg##Type=Registry::get().set_layer(#Type).get_layer(#Type).set_factory(&global_##Type##Factory) 
//static RegContent reg##Type=Registry::get().set_layer(#Type).get_layer(#Type).set_factory((LayerFactory*)(&global_##Type##Factory)) 
*/

}


#endif
