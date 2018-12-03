
#ifndef _Layer_H_
#define _Layer_H_

#include <string>
#include <vector>
#include "node.h"
#include <map>
#include <set>
#include <sstream>
#include <iostream>


class Layer;
class LayerFactory
{
    public:
        virtual Layer *create()=0;
};

class Layer{ 

    public:

        Layer(){mark=2;}

        virtual void inference(){}
        
        virtual ~Layer(){}

        //type of the layer;
        std::string type;

        //name of the layer
        std::string name;

        //input nodes of the layer
        std::vector<Node*> in_nodes;

        //output nodes of the layer
        std::vector<Node*> out_nodes;

        std::map<std::string,std::string> configs;

        template <typename T>
        T get_config(const std::string& name)
        {
            T value;
            get_config(name,value);
            return value;
        }
        
        void get_config(const std::string& name, int& value);
        void get_config(const std::string& name, size_t& value);
        void get_config(const std::string& name, float& value);
        void get_config(const std::string& name, std::string& value);


        Layer* set_config(const std::string name,const std::string value);
        
        Layer* set_input(std::vector<Node*> inputs);

        Layer* set_input(Node* input);

        Layer* add_output();
        Layer* add_output(size_t channels);

        virtual std::vector<Node*> get_output(){};

        static void register_config(const std::string& layer_name, const std::string& config_names)
        {
            std::stringstream ss(config_names);
            std::string config_name;
            while(ss>>config_name)
            {
                configs_type[layer_name].insert(config_name);
            }
        }

        static void register_layer(const std::string& name, LayerFactory *factory)
        {
            factories[name]=factory;
        }

        //static std::vector<Node*> create(const std::string &name)
        static Layer* create(const std::string &type)
        {

            Layer* layer= factories[type]->create();
            layer->type=type;
            layer->name=type;
            return layer;
        }

        static Layer* create(const std::string &type,const std::string &name)
        {

            Layer* layer= factories[type]->create();
            layer->type=type;
            layer->name=name;
            return layer;
            //return layer->out_nodes;
        }

    //private:
        static std::map<std::string,LayerFactory*> factories;
        static std::map<std::string,std::set<std::string> > configs_type;
        size_t mark;

};


#define REGISTER_LAYER(Type,Config) \
    class Type##Factory : public LayerFactory { \
        public: \
                Type##Factory() \
        { \
            Layer::register_layer(#Type, this); \
            Layer::register_config(#Type, #Config); \
        } \
        virtual Layer *create() { \
            return new Type(); \
        } \
    }; \
static Type##Factory global_##Type##Factory;



#endif
