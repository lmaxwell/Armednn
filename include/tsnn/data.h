#ifndef _Data_H
#define _Data_H

#include <iostream>
#include "tsnn/core.h"
#include <string>
namespace Tsnn{
typedef std::string string;




class Layer;

class Data
{
    public:

        void set_value(Matrix _value){value=_value;};
        Data(){};
        Data(string _name):name(_name){};

        template <typename T>
        Matrix& get_value() 
        {
            return value;
        }

        /*
        Matrix& get_value() 
        {
            return value
        }
        */

        std::string name;
        Layer* from=nullptr;
        Layer* to=nullptr;


    private:
        Matrix value;
    protected:

};

typedef Data* pData;


        

}

#endif
