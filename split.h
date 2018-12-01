
#ifndef _SPLIT_H_
#define _SPLIT_H_


#include "layer.h"
#include "node.h"
#include <vector>
#include <string>


class Split: public Layer{
    public:
            Split()
            {
            }

            size_t axis;
            size_t num_split;

            void inference();
            
            std::vector<Node *> get_output();
};

REGISTER_LAYER(Split,num_split axis)
#endif
