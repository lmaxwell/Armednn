
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
typedef  std::map<std::string,Param> ParamMap;

class Layer;
class LayerFactory
{
    public:
        virtual Layer *create(ConfigMap configs,std::string name)=0;
};


class Layer{ 
    protected:

        explicit Layer(ConfigMap _configs,std::string _name);

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

        static Layer& create(const std::string &type, const ConfigMap _config, const std::string &name);


        Layer* load_param( std::string name,  Matrix value);

        Matrix& get_param(std::string name);
        

    private:



        ConfigMap configs;

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



struct RegContent
{

    std::string name;


    RegContent& set_factory(LayerFactory *_f)
    {
        factory=_f;
        return *this;
    }

    template <typename T>
    RegContent& add_config(std::string name,std::string desc, T default_value)
    {
        configs[name];
        configs[name].set(name,desc,default_value);
        return *this;
    }

    RegContent& add_param(std::string name,std::string desc)
    {
        params[name]=desc;
        return *this;
    }

    typedef std::map<std::string,std::string> Parammap;

    LayerFactory* factory;
    ConfigMap configs;
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
        virtual Layer* create(ConfigMap config,std::string name)
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
