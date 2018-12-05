#include <iostream>
#include <vector>
#include <string>
using namespace std;
#include "Eigen/Dense"

/*
#define GET(type) \
#type get_##type(){};
*/

class Data_
{
    public:
        virtual void set(int){};
        virtual const int& get_(int&){};

        virtual void set(string){};
        virtual const string& get_(string&){};

        virtual void set(Eigen::MatrixXf){};
        virtual const Eigen::MatrixXf& get_(Eigen::MatrixXf&){};


        template <typename T>
        const T& get() 
        {
            T _;
            return get_(_);
        }

        /*
        int get_int()
        {
            int _;
            return get_(_);
        }
        string get_string()
        {
            string _;
            return get_(_);
        }

        Eigen::MatrixXf get_matrix()
        {
            Eigen::MatrixXf _;
            return get_(_);
        }
        */

    protected:
        Data_(){};
};

typedef Data_* pData;

template <typename T>
class Data:public Data_
{
    public:

        void set(T _value)
        {
            value=_value;
        }
        
        T get()
        {
            return value;
        }


        const T& get_(T& _value)
        {
            return value;
        }
        
    private:
        T value;
};

template <typename T>
class InputData:public Data<T>
{
    ;
};



int main()
{
    std::vector<pData> dv;
    Data<int> d;
    d.set(1);
    cout<<d.get()<<endl;
    InputData<string> id;
    id.set("world");
    cout<<id.get()<<endl;
    dv.push_back(&d);
    dv.push_back(&id);

    Data<Eigen::MatrixXf> dd;
    dd.set(Eigen::MatrixXf::Identity(2,2));
    dv.push_back(&dd);

    cout<<dv[0]->get<int>()<<endl;
    cout<<dv[0]->get<int>()<<endl;
    cout<<dv[1]->get<string>()<<endl;
    cout<<dv[2]->get<Eigen::MatrixXf>()<<endl;
    return 1;
}

