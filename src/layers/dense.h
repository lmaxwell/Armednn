
#ifndef _DENSE_H_
#define _DENSE_H_


#include "tsnn/layer.h"
#include <vector>
#include <string>

namespace Tsnn
{

class Dense: public Layer{
    public:
            Dense(Config config,std::string name):Layer(config,name)
            {
                set_num_input(1);
                set_num_output<Matrix>(1);

                add_param("weight",get_config<size_t>("dim0"),get_config<size_t>("dim1"));
                add_param("bias",1,get_config<size_t>("dim1"));
            }

            Matrix weight;
            Matrix bias;

            size_t dim0;
            size_t dim1;
            std::string activation;
            
            void prepare();

            void inference();
            
};


REGISTER_LAYER(Dense, dim0 dim1 activation)

}
#endif
