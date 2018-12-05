
#ifndef _DENSE_H_
#define _DENSE_H_


#include "tsnn/layer.h"
#include <vector>
#include <string>


/* 参考dynamic-graph
#include <map>
class Param
{
    public:
        Param(){}
        Eigen::MatrixXf& get(std::string name)
        {
            return 
        }

    private:
        std::map<std::string,Eigen::MatrixXf> params;
}
*/

class Dense: public Layer{
    public:
            Dense(Config config,std::string name):Layer(config,name)
            {
                set_num_input(1);
                set_num_output<Matrix>(1);
                //define parameter
            }

            std::string activation;

            Eigen::MatrixXf weight;
            Eigen::VectorXf bias;

            void inference();
            
};

REGISTER_LAYER(Dense,dims activation)
#endif
