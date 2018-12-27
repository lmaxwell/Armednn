#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <iostream>
#include <map>
#include <typeinfo>
namespace Tsnn
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

    //move constructor
    ConfigValue(ConfigValue&& other)
    {
        //std::cout<<"MOVE CTOR"<<std::endl;
        base=other.base;
        other.base=nullptr;
    }

    //copy constructor
    ConfigValue(const ConfigValue& other)
    {
        //std::cout<<"COPY CTOR"<<std::endl;
        base=other.base->copy();
    }


    //move assignment
    ConfigValue& operator=(ConfigValue&& other)=delete;
    /*
    {
        std::cout<<"MOVE ASSIGN"<<std::endl;
        if(base!=nullptr)
        {
            delete base;
        }
        base=other.base;
        other.base=nullptr;
    }
    */

    ConfigValue& operator=(const ConfigValue& other)=delete;
    /*
    {
        std::cout<<"COPY ASSIGN"<<std::endl;
        if(base!=nullptr)
            delete base;
        base=other.base->copy();
        return *this;
    }
    */





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

    //proto
    template <typename T>
    Config(std::string _name,std::string _desc,T _value):name(_name),desc(_desc),value(_value){}

    /*
    //user input
    template <typename T>
    Config(T&& _value):value(std::forward<T>(_value)){
        std::cout<<"only value constructor"<<std::endl;
    }
    */

    //user input
    template <typename T>
    Config(T _value):value(_value){
        //std::cout<<"only value constructor"<<std::endl;
    }

    Config(const Config& other)
    {
        //std::cout<<"copy ctor"<<std::endl;
        name=other.name;
        desc=other.desc;
        value.set(other.value);
    }
    Config(Config&& other)
    {
        //std::cout<<"move ctor"<<std::endl;
        name=std::move(other.name);
        desc=std::move(other.desc);
        value.set(std::move(other.value));
    }

    Config& operator=(const Config&)=default;
    Config& operator=(Config&&)=default;

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


    std::string name;
    std::string desc;
    ConfigValue value;
};

typedef std::map<std::string,Config> ConfigMap;


bool config_has_key(const ConfigMap& configs,std::string& name);
bool config_compare_key(const ConfigMap& a ,const ConfigMap &b);
bool config_compare_type(const ConfigMap& a ,const ConfigMap &b);


}// namespace Tsnn

#endif
