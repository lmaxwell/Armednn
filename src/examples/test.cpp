#include <iostream>
#include "tsnn/tsnn.h"

#include <map>
#include <string>
#include <chrono>
#include <time.h>

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


int main()
{

    Tsnn::LogMessage::enable(true);

    DEBUG<<"info";



    /*
    Layer* layer=new Layer();
    return 1;
    MatrixXd *xf;


    MatrixXd mat(2,2);
    mat(0,0)=1;
    mat(0,1)=2;
    mat(1,0)=3;
    mat(1,1)=4;
    MatrixXd mat2=mat;
    for(int i=0;i<4;i++)
        cout<<mat.data()[i]<<endl;
    cout<<mat.data()<<"\t"<<mat.array().data()<<"\t"<<mat2.data()<<endl;
    for(int i=0;i<4;i++)
        cout<<mat2.data()[i]<<endl;
    cout<<mat<<endl;
    mat.row(0).array()*=mat2.row(0).array();
    cout<<mat.row(0).array()<<endl;
    cout<<mat<<endl;

    typedef Matrix<float,Dynamic,Dynamic,RowMajor> RowMajMat; 
    RowMajMat mat3(2,6);
    mat3<<1,2,3,4,5,6,
         7,8,9,10,11,12;
    Map<RowMajMat> mat4(mat3.data(),6,2);
    cout<<mat3<<endl;
    cout<<mat4<<endl;
    cout<<mat3.data()<<"\t"<<mat4.data()<<endl;

    cout<<mat4.data()+3<<endl;
    cout<<mat4.block(1,1,1,1).data()<<endl;
    exit(0);

    std::map<std::string,std::string> configs;

    test({{"v1","0"},{"v2","1"}});
    */

    //std::cout<<"here"<<Layer::factories().size()<<std::endl;
    
    pData input=new Data<Matrix>("input");

    Config *config= new Config();
    
    config->set("activation","tanh");
    config->set("dim0","100");
    config->set("dim1","1024");
    auto d=Layer::create("Dense",
                         *config,
                        "Dense")->set_input(input);
    d->load_param("weight",Matrix::Identity(100,1024));
    d->load_param("bias",Matrix::Zero(1,1024));
    auto x=d->get_output();
    INFO<<x.size();
    

    /*
    config= new Config();
    config->set("num_split","2");
    auto l=Layer::create("Split",
                         *config,
                         "Split-0")->set_input(x);
    INFO<<l->get_config<size_t>("num_split");

    auto y=l->get_output();


    config= new Config();
    config->set("num_input","2");
    auto y2=Layer::create("Concat",
                         *config,
                         "Concat")->set_input(y)
                                  ->get_output();


    vector<pData> out;

    for(int i=0;i<y2.size();i++)
    {
        config= new Config();
        config->set("num_split","2");
        auto z=Layer::create("Split",
                             *config,
                             "Split-"+to_string(i+1))->set_input(y2[i])
                                                     ->get_output();
        out.insert(out.end(),z.begin(),z.end());
    }
    */



    //Network *net =new Network({input},out);
    Network *net =new Network({input},{x});
    net->print();

    Eigen::MatrixXd a=Eigen::MatrixXd::Random(50000,100);
    Eigen::MatrixXd b=Eigen::MatrixXd::Random(100,1000);
    Eigen::MatrixXd c;
    clock_t starttime = clock(); 
    c.noalias()=a*b;
    clock_t endtime = clock();

    cout << "Time elapsed: " << diffclock( endtime, starttime) \
        << " sec." << endl;
    

    for(int k=0;k<2;k++)
    {
        Tsnn::LogMessage::enable(true);
        auto start=std::chrono::system_clock::now();
        INFO<<k<<"-th run:";
        net->feed({Matrix::Random(48000,100)})->compute();
        /*
        INFO<<"input"<<endl<<input->get_value<Matrix>();
        for(int i=0;i<net->get_output().size();i++)
        {
            INFO<<i<<"-th output:"<<net->get_output()[i]->name<<endl
                <<net->get_output()[i]->get_value<Matrix>();
        }
        */
        auto end=std::chrono::system_clock::now();
        auto duration=std::chrono::duration_cast<std::chrono::microseconds>(end-start);
        Tsnn::LogMessage::enable(true);
        INFO<<"COST "<<double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
    }
    //delete l;
}
