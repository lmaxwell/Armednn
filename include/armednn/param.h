

#ifndef _Param_H_
#define _Param_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "armednn/common.h"

namespace Armednn{

/*
template <uint32_t N>
struct Shape
{
    Shape(){}
    Shape(uint32_t t0)
    {
        static_assert(N==1,"N!=1");
        _d[0]=t0;
    }
    Shape(uint32_t t0,uint32_t t1)
    {
        static_assert(N==2,"N!=2");
        _d[0]=t0;
        _d[1]=t1;
    }
    uint32_t& operator[](uint32_t i)
    {
        CHECK(i<N)<<"overflow!";
        return _d[i];
    }
    uint32_t _d[N];
};


template <uint32_t N>
std::vector<uint32_t> shape_flat(Shape<N> shape)
{
    std::vector<uint32_t> tmp;
    for(uint32_t i=0;i<N;i++)
        tmp.push_back(shape[i]);
    return tmp;
}


template <uint32_t N> 
using ShapeFun=std::function<Shape<N>(ConfigMap&)>;

typedef std::function<std::vector<uint32_t> (ConfigMap&)> ShapeFun;
*/




class Arm;

class Param
{
    public:

        Param()=default;

        Param(std::vector<std::string>  _shape_mapping):shape_mapping(_shape_mapping){};
        Param(std::function<std::vector<uint32_t>(ConfigMap& config)> _shape_func):shape_func(_shape_func){};

        Param(Matrix&& _value):value(std::move(_value)){};

        Param(Matrix& _value):value(_value){};


        std::vector<std::string> shape_mapping;
        std::function<std::vector<uint32_t> (ConfigMap& config)> shape_func=nullptr;

        Matrix value;

        std::vector<uint32_t> shape()
        {
            return {(uint32_t)value.rows(),(uint32_t)value.cols()};
        }


};

typedef  std::unordered_map<std::string,Param> ParamMap;


namespace ParamHelp
{
struct ShapeMapping
{
    std::vector<std::string> names;
    explicit ShapeMapping(std::vector<std::string> _names):names(_names){}
    std::vector<uint32_t> operator()(ConfigMap &configs)
    {
        std::vector<uint32_t> shape;
        for(auto name:names)
        {
            if(configs.find(name)!=configs.end())
            {
                uint32_t tmp;
                CHECK(configs[name].get<uint32_t>(tmp))<<"ShapeMapping name" <<name<<" must be type uint32_t";
                shape.push_back(tmp);
            }
            else
            {
                shape.push_back((uint32_t)atoi(name.c_str()));
            }
        }
        return shape;
    }
};
}//namespace Param

}//namespace Armednn
#endif
