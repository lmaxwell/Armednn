
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


        /*
        void load( float *p)
        {
            value=Eigen::Map<Matrix>(p,rows,cols);
        }
        */
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

        static void register_config(const std::string& layer_name, const std::string& config_names);

        static void register_layer(const std::string& name, LayerFactory *factory);


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



#define REGISTER_LAYER(Type,ConfigNames) \
    class Type##Factory : public LayerFactory { \
        public: \
                Type##Factory() \
        { \
            DEBUG<<"registering "<<#Type;\
            Layer::register_layer(#Type, this); \
            DEBUG<<"register "<<#Type<<" done";\
            Layer::register_config(#Type, #ConfigNames); \
        } \
        virtual Layer *create(Config _config,std::string name) { \
            return new Type(_config,name); \
        } \
    }; \
static Type##Factory global_##Type##Factory;

}


#endif
