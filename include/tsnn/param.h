

#ifndef _Param_H_
#define _Param_H_

namespace Tsnn{

/*
template <size_t N>
struct Shape
{
    Shape(){}
    Shape(size_t t0)
    {
        static_assert(N==1,"N!=1");
        _d[0]=t0;
    }
    Shape(size_t t0,size_t t1)
    {
        static_assert(N==2,"N!=2");
        _d[0]=t0;
        _d[1]=t1;
    }
    size_t& operator[](size_t i)
    {
        CHECK(i<N)<<"overflow!";
        return _d[i];
    }
    size_t _d[N];
};


template <size_t N>
std::vector<size_t> shape_flat(Shape<N> shape)
{
    std::vector<size_t> tmp;
    for(size_t i=0;i<N;i++)
        tmp.push_back(shape[i]);
    return tmp;
}


template <size_t N> 
using ShapeFun=std::function<Shape<N>(ConfigMap&)>;

typedef std::function<std::vector<size_t> (ConfigMap&)> ShapeFun;
*/




class Param
{
    public:

        Param(){};

        Param(std::string _name, std::vector<string>  _shape_mapping):name(_name),shape_mapping(_shape_mapping){};

        Param(std::string _name,size_t _rows,size_t _cols):name(_name){shape[0]=_rows;shape[1]=_cols;};

        std::string name;

        std::vector<size_t> shape;
        std::vector<string> shape_mapping;


        Matrix value;

        void load(Matrix _value)
        {
            CHECK((size_t)_value.rows()==shape[0] && (size_t)_value.cols()==shape[1])<<"loading Matrix has wrong dimension";
            value=_value;

        }

};

typedef  std::map<std::string,Param> ParamMap;

struct ShapeMapping
{
    std::vector<string> names;
    explicit ShapeMapping(std::vector<string> _names):names(_names){}
    std::vector<size_t> operator()(ConfigMap &configs)
    {
        std::vector<size_t> shape;
        for(auto name:names)
        {
            if(configs.find(name)!=configs.end())
            {
                size_t tmp;
                CHECK(configs[name].get<size_t>(tmp))<<"ShapeMapping name" <<name<<" must be type size_t";
                shape.push_back(tmp);
            }
            else
            {
                shape.push_back((size_t)atoi(name.c_str()));
            }
        }
        return shape;
    }
};

}//namespace Tsnn
#endif
