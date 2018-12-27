#include "tsnn/config.h"

namespace Tsnn{

bool config_has_key(const ConfigMap& configs,std::string& name)
{
    if(configs.find(name)!=configs.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool config_compare_key(const ConfigMap& a ,const ConfigMap &b)
{
    //CHECK
    return true;
}

bool config_compare_type(const ConfigMap& a ,const ConfigMap &b)
{
    return true;
}

}// namespace Tsnn
