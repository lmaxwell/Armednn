#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <iostream>
#include <map>
#include <initializer_list>
namespace Tsnn
{
class Config{
    public:
        Config(){};
        
        //Config(std::initializer_list<std::pair<std::string,std::string> > _configs):configs(_configs)  
        //not work
        //Config(std::initializer_list<std::pair<const std::string, const std::string> > _configs):configs(_configs)  
        //not work
        //https://cplusplus.github.io/LWG/issue3025

        Config(std::initializer_list<std::pair<const std::string, std::string> > _configs):configs(_configs)
        {
        }

        //Config(std::map<std::string,std::string>  _configs):configs(_configs){};

        //Config(std::pair<std::string,std::string> _configs){configs.insert(_configs);};

        /*
        Config(const Config &other)
        {
            configs=other.configs;
        }

        */


        void set(std::string name,std::string value);

        void add(std::string name);

        bool has(std::string name);

        std::map<std::string,std::string>& getall()
        {
            return configs;
        }

        template <typename T>
        T get(const std::string& name);

    private:
        std::map<std::string, std::string> configs;
        void str2(const std::string&, int&);
        void str2(const std::string&, size_t&);
        void str2(const std::string&, float&);
        void str2(const std::string&, std::string&);
};
}

#endif
