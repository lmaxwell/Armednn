
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
            }

            size_t num_split;

            void prepare();

            void inference();

            
};

REGISTER_LAYER(Split).add_config<size_t>("num_split","number of split outputs, only support along last dim")
                     .set_num_input("1")
                     .set_num_output("num_split");


}
#endif
