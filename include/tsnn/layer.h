
#ifndef _Layer_H_
#define _Layer_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include <functional>

#include "tsnn/core.h"
#include "tsnn/data.h"
#include "tsnn/config.h"
#include "tsnn/param.h"


namespace Tsnn{



/*
struct LayerProp
{
    LayerProp():num_input(-1),num_output(-1){}
    LayerProp(ConfigMap _configs, ParamMap _params):configs(_configs),params(_params),num_input(-1),num_output(-1){}
    ConfigMap configs;
    ParamMap  params;
    size_t num_input;
    size_t num_output;
};
*/

class Layer{ 

    private:

        /*
        LayerProp prop;
        */

        ConfigMap configs;

        ParamMap params;

        size_t num_input;
        size_t num_output;

        std::vector<pData> in_nodes;

        std::vector<pData> out_nodes;

        Layer* add_output();

    protected:

        explicit Layer(ConfigMap _configs,std::string _name);

        void add_param(std::string name, size_t rows, size_t cols);

        bool has_param(std::string name);

        void set_num_input(size_t num)
        {
            num_input=num;
        }

        void set_num_output(size_t num)
        {
            for(size_t  i=0;i<num;i++)
                add_output();
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
        bool get_config(const std::string& name, T& value);

        std::vector<pData>& get_output(){return out_nodes;};
        std::vector<pData>& get_input(){return in_nodes;};


        Layer* load_param( std::string name,  Matrix value);

        Matrix& get_param(std::string name);

        static Layer& create(const std::string , const ConfigMap , const std::string );
        


};

template <typename T>
bool Layer::get_config(const std::string& name, T& value)
{
    return configs[name].get<T>(value);
}

class LayerFactory
{
    public:
        virtual Layer *create(ConfigMap configs,std::string name)=0;
};

typedef std::map<std::string,std::string> Parammap;


struct InOutMapping
{
    std::string name;
    InOutMapping(std::string _name):name(_name){}
    size_t operator()(ConfigMap& configs)
    {
            if(configs.find(name)!=configs.end())
            {
                size_t tmp;
                CHECK(configs[name].get<size_t>(tmp))<<"InputMapping name" <<name<<" must be type size_t";
                return tmp;
            }
            else
            {
                return (size_t) atoi(name.c_str());
            }
    }
};

struct RegContent
{


    RegContent():num_input("?"),num_output("?"){}
    std::string name;

    RegContent& set_factory(LayerFactory *_f)
    {
        factory=_f;
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
        T *tmp=new T();
        configs[name];
        configs[name].set(name,desc,*tmp);
        delete tmp;
        return *this;
    }

    RegContent& add_param(std::string name, std::vector<string> shape_mapping=std::vector<string>())
    {
        params[name]=Param(name,shape_mapping);
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

    std::string _doc;
    LayerFactory* factory;
    ConfigMap configs;
    ParamMap params;
    std::string num_input;
    std::string num_output;

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


}//namespace Tsnn


#endif
