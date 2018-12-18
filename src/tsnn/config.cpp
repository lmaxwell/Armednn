#include "tsnn/config.h"

namespace Tsnn{

void Config::str2(const std::string& value1, int& value2)
{
        value2= atoi(value1.c_str());
}

void Config::str2(const std::string& value1, size_t& value2)
{
        value2= atoi(value1.c_str());
}
void Config::str2(const std::string& value1, float& value2)
{
        value2=atof(value1.c_str());
}
void Config::str2(const std::string& value1, std::string& value2)
{
        value2=value1.c_str();
}

template <typename T>
T Config::get(const std::string& name)
{
    T value;
    str2(configs[name],value);
    return value;
}

template size_t Config::get(const std::string&);
template int Config::get(const std::string&);
template float Config::get(const std::string&);
template std::string Config::get(const std::string&);

void Config::add(std::string name)
{
    configs[name]="";
}

void Config::set(std::string name,std::string value)
{

    configs[name]=value;
}
bool Config::has(std::string name)
{
    if(configs.find(name)==configs.end())
        return false;
    else
        return true;
}

}
