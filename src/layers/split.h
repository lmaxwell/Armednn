
#ifndef _SPLIT_H_
#define _SPLIT_H_


#include "tsnn/layer.h"
#include <vector>
#include <string>
#include <iostream>

namespace Tsnn{

class Split: public Layer{

    public:
            Split(Config config,std::string name):Layer(config,name)
            {
                set_num_input(1);
                set_num_output<Matrix>(get_config<size_t>("num_split"));
            }

            size_t axis;
            size_t num_split;

            void inference();
            
};

REGISTER_LAYER(Split,num_split axis)

}
#endif
