
#include "tsnn/param.h"

namespace Tsnn{

struct ShapeMapping
{
    std::vector<string> names;
    explicit ShapeMapping(std::vector<string> _names):names(_names){}
    std::vector<size_t> operator()(ConfigMap &configs)
    {
        std::vector<size_t> shape;
        for(auto name:names)
        {
            if(configs.find(name)!=configs.end())
            {
                size_t tmp;
                CHECK(configs[name].get<size_t>(tmp))<<"ShapeMapping name" <<name<<" must be type size_t";
                shape.push_back(tmp);
            }
            else
            {
                shape.push_back((size_t)atoi(name.c_str()));
            }
        }
        return shape;
    }
};
}//namespace Tsnn
