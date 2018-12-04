#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <map>
class Config{
    public:
        Config(){}

        void set(std::string name,std::string value);

        template <typename T>
        T get(const std::string& name);

    private:
        std::map<std::string,std::string> configs;
        void str2(const std::string&, int&);
        void str2(const std::string&, size_t&);
        void str2(const std::string&, float&);
        void str2(const std::string&, std::string&);
};

#endif
