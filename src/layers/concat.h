#ifndef _CONCAT_H_
#define _CONCAT_H_


#include "tsnn/layer.h"
#include <vector>
#include <string>
#include <iostream>


namespace Tsnn
{
class Concat: public Layer{
    public:

            Concat(ConfigMap configs,std::string name):Layer(configs,name)
            {
                set_num_input(get_config<size_t>("num_input"));
                set_num_output<Matrix>(1);
            }

            size_t num_input;
            void inference();
            void prepare();
};

//REGISTER_LAYER(Concat,num_input);//.add_config().add_param("null","for test");
REGISTER_LAYER(Concat).add_config("num_input","number of inputs",(size_t)1);

}
#endif
