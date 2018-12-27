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
            }

            size_t num_input;
            void inference();
            void prepare();
};

REGISTER_LAYER(Concat).doc("Concat: split channels along the last axis")
                      .add_config<size_t>("num_input","number of inputs")
                      .set_num_input("num_input")
                      .set_num_output("1");

}
#endif
