#ifndef _Network_H_
#define _Network_H_
#include "tsnn/layer.h"

namespace Tsnn{
class Network
{
    public:

        explicit Network(std::vector<pData> _in_nodes,std::vector<pData> _out_nodes);

        Network* feed(const std::vector<Matrix> &data);

        void prepare();

        void compute();

        void print();

        std::vector<pData>& get_output();
        std::vector<pData>& get_input();

        ~Network();


    private:

        Network* bind(std::vector<pData> _in_nodes,std::vector<pData> _out_nodes);
        void iterate_layers(pData node);
        std::vector<pData>in_nodes;
        std::vector<pData>out_nodes;
        std::vector<Layer *>layers_v;

};

}


#endif
