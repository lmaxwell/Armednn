
#ifndef _SPLIT_H_
#define _SPLIT_H_


#include "layer.h"
#include "node.h"
#include <vector>
#include <string>


class Split: public Layer{
    public:
            Split(Config _config):Layer(_config)
            {
                set_num_input(1);
                set_num_output(get_config("num_split"));
            }

            size_t axis;
            size_t num_split;

            void inference();
            
            std::vector<Node *> get_output();
};

REGISTER_LAYER(Split,num_split axis)
#endif
