#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <iostream>
#include <unordered_map>
#include <typeinfo>

namespace Armednn
{

struct ConfigValue;
template <typename T>
T config_cast(ConfigValue&);

//any like container, used to store variable value
struct ConfigValue
{
    
    ConfigValue()=default;

    //constructor
    template <typename T>
    ConfigValue(T _value):base(new Store<T>(_value))
    {
        //std::cout<<"CTOR"<<std::endl;
    }


    ~ConfigValue()
    {
        //std::cout<<"DTOR"<<std::endl;
        if(base!=nullptr)
        {

            delete base;
            base=nullptr;
        }
    }

    //copy constructor
    ConfigValue(const ConfigValue& other)
    {
        //std::cout<<"COPY CTOR"<<std::endl;
        base=other.base->copy();
    }

    //move constructor
    ConfigValue(ConfigValue&& other)
    {
        //std::cout<<"MOVE CTOR"<<std::endl;
        base=other.base;
        other.base=nullptr;
    }


    ConfigValue& operator=(const ConfigValue& other)
    {
        std::cout<<"COPY ASSIGN"<<std::endl;
        if(base!=nullptr)
            delete base;
        base=other.base->copy();
        return *this;
    }




    //move assignment
    ConfigValue& operator=(ConfigValue&& other)
    {
        std::cout<<"MOVE ASSIGN"<<std::endl;
        if(base!=nullptr)
        {
            delete base;
        }
        base=other.base;
        other.base=nullptr;
        return *this;
    }



    struct StoreBase{
        
        virtual StoreBase* copy()=0;
        virtual ~StoreBase(){}
    };


    template <typename T>
    struct Store:public StoreBase
    {
        Store(T _value):value(_value)
        {
        }
        StoreBase* copy()
        {
            return new Store(value);
        }


        T value;
        virtual ~Store(){}
    };



    template <typename T>
    bool get(T &_value)
    {
        ConfigValue::Store<T>* tmp=dynamic_cast<ConfigValue::Store<T>*>(base);
        if(tmp==nullptr)
        {
            std::cout<<"wrong type! cannot get value, check type for this config"<<std::endl;
            return false;
        }
        _value= tmp->value;
        return true;
    }

    bool set(ConfigValue other)
    {

        if(base==nullptr)
        {
            base=other.base->copy();
            return true;
        }

        if (typeid(*base)==typeid(*(other.base)))
        {
            delete base;
            base=other.base->copy();
            return true;
        }

        std::cout<<"you provide wrong type!"<<std::endl;
        return false;
    }

    template <typename T>
    bool set(T _value)
    {

        if(base==nullptr)
        {
            base=new Store<T>(_value);
            return true;
        }

        Store<T>* tmp=dynamic_cast<ConfigValue::Store<T>*>(base);
        if(tmp==nullptr)
        {
            std::cout<<"wrong type! cannot set value, check type for this config"<<std::endl;
            return false;
        }
        tmp->value=_value;
        return true;
    }

    StoreBase *base=nullptr;
};


template <typename T>
T config_cast(ConfigValue& c)
{
    ConfigValue::Store<T>* tmp=dynamic_cast<ConfigValue::Store<T>*>(c.base);
    if(tmp==nullptr)
    {
        std::cout<<"wrong type! cannot get value"<<std::endl;
    }
    return tmp->value;
}


struct Config
{
    Config()=default;

    template <typename T>
    Config(std::string _name,std::string _desc,T _value):name(_name),desc(_desc),value(_value){}

    template <typename T>
    Config(T _value):value(_value){}

    bool set(ConfigValue _value)
    {
        if(value.set(_value))
        {
            return true;
        }
        else
        {
            //std::cout<<"name:"<<name<<"\tdesc:"<<desc<<std::endl;
            return false;
        }
    }
    template <typename T>
    bool set(T _value)
    {
        if(value.set(_value))
        {
            return true;
        }
        else
        {
            //std::cout<<"name:"<<name<<"\tdesc:"<<desc<<std::endl;
            return false;
        }
    }

    template <typename T>
    bool set(std::string _name,std::string _desc,T _value)
    {
        name=_name;
        desc=_desc;
        if(value.set(_value))
        {
            return true;
        }
        else
        {
            //std::cout<<"name:"<<name<<"\tdesc:"<<desc<<std::endl;
            return false;
        }
    }



    template <typename T>
    bool get(T& _value)
    {
      if(value.get<T>(_value))
      {
          return true;
      }
      else
      {
          //std::cout<<"name:"<<name<<"\tdesc:"<<desc<<std::endl;
          return false;
      }
      
    }

    std::string name="None";
    std::string desc="None";
    ConfigValue value=ConfigValue();
};

typedef std::unordered_map<std::string,Config> ConfigMap;


bool config_has_key(const ConfigMap& config,std::string& name);
bool config_compare_key(const ConfigMap& a ,const ConfigMap &b);
bool config_compare_type(const ConfigMap& a ,const ConfigMap &b);
namespace ConfigHelp
{

struct InOutMapping
{
    std::string name;
    InOutMapping(std::string _name):name(_name){}
    uint32_t operator()(ConfigMap& config)
    {
            if (name=="?")
                return 0;
            if(config.find(name)!=config.end())
            {
                uint32_t tmp;
                CHECK(config[name].get<uint32_t>(tmp))<<"InputMapping name" <<name<<" must be type uint32_t";
                return tmp;
            }
            else
            {
                return (uint32_t) atoi(name.c_str());
            }
    }
};

}//namespace Config


}// namespace Armednn

#endif
