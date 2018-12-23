#include <iostream>
#include "tsnn/tsnn.h"

#include <map>
#include <string>
#include <chrono>

using namespace std;
using Eigen::MatrixXd;
using Eigen::Map;
using namespace Tsnn;

double diffclock(clock_t clock1,clock_t clock2)
{
    double diffticks=clock1-clock2;
    double diffms=(diffticks)/CLOCKS_PER_SEC;
    return diffms;
} 

class MyNet:public Network
{
    public:
        void build()
        {
            auto xx= Input();

            std::string s="tanh";

            auto x=Layer::create("Dense",
                                 {{"activation",s},{"dim0","10"},{"dim1","10"}},
                                 "Dense-0")(xx);

            auto y=Layer::create("Split",{{"num_split","2"}},"Split-0")(x);

            y=Layer::create("Dense",
                            {{"activation","tanh"},{"dim0","5"},{"dim1","5"}},
                            "Dense-1")({y[0]});

            Output(y);
        }

        void load_param()
        {
            INFO<<"here";
            auto d=get_layer("Dense-0");
            d->load_param("weight",Matrix::Identity(10,10));
            d->load_param("bias",Matrix::Zero(1,10));

            d=get_layer("Dense-1");
            d->load_param("weight",Matrix::Identity(5,5));
            d->load_param("bias",Matrix::Zero(1,5));

        }

};


int main()
{

    Tsnn::LogMessage::enable(true);


    MyNet *net=new MyNet();
    
    net->build();
    
    net->load_param();

    net->prepare();

    net->print();

    int num=4;
    for(int i=0;i<2;i++)
    {
        for(int k=0;k<1;k++)
        {
            num+=1;
            Tsnn::LogMessage::enable(true);
            auto start=std::chrono::system_clock::now();
            INFO<<k<<"-th run:";
            net->feed({Matrix::Random(num,10)})->compute();
            INFO<<"input:"<<endl<<net->get_input()[0]->get_value<Matrix>();;
            for(int i=0;i<net->get_output().size();i++)
            {
                INFO<<i<<"-th output:"<<net->get_output()[i]->name<<endl
                    <<net->get_output()[i]->get_value<Matrix>()<<endl;
            }
            auto end=std::chrono::system_clock::now();
            auto duration=std::chrono::duration_cast<std::chrono::microseconds>(end-start);
            Tsnn::LogMessage::enable(true);
            INFO<<"COST "<<double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
        }
    }
    //INFO<<"input:"<<endl<<input->get_value<Matrix>();;
    delete net;
    //delete input;
}
