
#ifndef _DENSE_H_
#define _DENSE_H_


#include "layer.h"
#include "node.h"
#include <vector>
#include <string>

class Dense: public Layer{
    public:
            Dense()
            {
            }

            std::string activation;

            Eigen::MatrixXf weight;
            Eigen::VectorXf bias;

            void inference();
            
            std::vector<Node *> get_output();
};

REGISTER_LAYER(Dense,dims activation)
#endif
