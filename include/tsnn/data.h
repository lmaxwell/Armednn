#ifndef _Data_H
#define _Data_H

#include <iostream>
#include "tsnn/core.h"
#include <string>
namespace Tsnn{
typedef std::string string;




template <typename T>
string GET_TYPE()
{
    if(typeid(T)==typeid(string))
        return "string";
    else if(typeid(T)==typeid(Matrix))
        return "Matrix";
    else
        return typeid(T).name();
}


class Layer;
template <typename T> class Data;
class Data_
{
    public:
        virtual void set_value(int){};

        virtual void set_value(string){};

        virtual void set_value(Matrix){};

        virtual string& get_type()=0;

        template <typename T>
        T& get_value() 
        {
           return static_cast<Data<T>&>(*this).get_value();
        }

        std::string name;
        Layer* from;
        Layer* to;

    protected:
        Data_(){};
        Data_(string _name):name(_name){};

};

typedef Data_* pData;

template <typename T>
class Data:public Data_
{
    public:
        Data():Data_(){type=GET_TYPE<T>();};
        Data(string name):Data_(name){type=GET_TYPE<T>();};

        void set_value(T _value)
        {
            value=_value;
        }
        
        T& get_value()
        {
            CHECK_EQ(get_type(),GET_TYPE<T>())<<"cannot get_value, "<<GET_TYPE<T>()<<"; This DataPort holds data of "<<get_type();
            return value;
        }
        string& get_type()
        {
            return type;
        }
        
    private:
        T value;
        string type;
};

}

#endif
