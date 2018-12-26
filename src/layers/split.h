
#ifndef _SPLIT_H_
#define _SPLIT_H_


#include "tsnn/layer.h"
#include <vector>
#include <string>
#include <iostream>

namespace Tsnn{

class Split: public Layer{

    public:
            Split(ConfigMap configs,std::string name):Layer(configs,name)
            {
                set_num_input(1);
                set_num_output<Matrix>(get_config<size_t>("num_split"));
            }

            size_t num_split;

            void prepare();

            void inference();

            
};

//REGISTER_LAYER(Split,num_split);
REGISTER_LAYER(Split).add_config("num_split","number of split outputs, only support along last dim",(size_t)1);

}
#endif
