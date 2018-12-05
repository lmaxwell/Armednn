#ifndef _CONCAT_H_
#define _CONCAT_H_


#include "tsnn/layer.h"
#include <vector>
#include <string>
#include <iostream>


class Concat: public Layer{
    public:

            Concat(Config config,std::string name):Layer(config,name)
            {
                set_num_input(get_config<size_t>("num_input"));
                set_num_output(1);
            }

            void inference();
};

REGISTER_LAYER(Concat,num_input)
#endif