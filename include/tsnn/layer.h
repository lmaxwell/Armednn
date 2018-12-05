
#ifndef _Layer_H_
#define _Layer_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>

#include "tsnn/data.h"
#include "tsnn/config.h"




class Layer;
class LayerFactory
{
    public:
        virtual Layer *create(Config config,std::string name)=0;
};

class Layer{ 
    protected:
        Layer(){mark=2;}

        Layer(Config _config,std::string _name):config(_config),name(_name){mark=2;}

    public:
        
        virtual ~Layer(){}

        virtual void inference()=0;


        //type of the layer;
        std::string type;
        //name of the layer
        std::string name;



        void set_num_input(size_t num)
        {
            num_input=num;
        }
        void set_num_output(size_t num)
        {
            for(int i=0;i<num;i++)
                add_output();
            num_output=num;
        }

        Layer* set_input(std::vector<Data*> inputs);

        Layer* set_input(Data* input);

        template <typename T>
        T get_config(const std::string& name);

        

        Layer* set_config(const std::string name,const std::string value);
        

        std::vector<Data*> get_output(){return out_nodes;};
        std::vector<Data*> get_input(){return in_nodes;};

        static void register_config(const std::string& layer_name, const std::string& config_names);
        static void register_layer(const std::string& name, LayerFactory *factory);

        static Layer* create(const std::string &type, const Config _config, const std::string &name);


        size_t mark;

    private:
        Config config;
        size_t num_input;
        size_t num_output;
        //input nodes of the layer
        std::vector<Data*> in_nodes;
        //output nodes of the layer
        std::vector<Data*> out_nodes;

        Layer* add_output();
        Layer* add_output(size_t channels);
};

#define FORCE_LINK_THIS(x) int force_link_##x = 0;
#define FORCE_LINK_THAT(x) { extern int force_link_##x; force_link_##x = 1; }

#define REGISTER_LAYER(Type,ConfigNames) \
    class Type##Factory : public LayerFactory { \
        public: \
                Type##Factory() \
        { \
            std::cout<<"registering "<<#Type<<std::endl;\
            Layer::register_layer(#Type, this); \
            std::cout<<"register "<<#Type<<" done"<<std::endl;\
            Layer::register_config(#Type, #ConfigNames); \
        } \
        virtual Layer *create(Config _config,std::string name) { \
            return new Type(_config,name); \
        } \
    }; \
static Type##Factory global_##Type##Factory;



#endif
