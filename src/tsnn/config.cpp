#include "tsnn/config.h"

namespace Tsnn{

bool config_has_key(ConfigMap& configs,std::string& name)
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

bool config_compare_key(ConfigMap& a ,ConfigMap &b)
{
    //CHECK
    return true;
}
}// namespace Tsnn
