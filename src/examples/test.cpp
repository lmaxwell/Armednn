#include <iostream>
#include "tsnn/tsnn.h"

#include <map>
#include <string>

using namespace std;
using Eigen::MatrixXf;
using Eigen::Map;
using Eigen::Matrix;




int main()
{

    /*
    Layer* layer=new Layer();
    return 1;
    MatrixXf *xf;


    MatrixXf mat(2,2);
    mat(0,0)=1;
    mat(0,1)=2;
    mat(1,0)=3;
    mat(1,1)=4;
    MatrixXf mat2=mat;
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
    
    Node* input=new Node("input");


    Config *config= new Config();
    
    config->set("activation","tanh");
    config->set("dims","6");
    auto x=Layer::create("Dense",
                         *config,
                        "Dense")->set_input(input)
                                ->get_output();
    

    config= new Config();
    config->set("num_split","2");
    config->set("axis","1");
    auto l=Layer::create("Split",
                         *config,
                         "Split-0")->set_input(x);
    cout<<l->get_config<size_t>("num_split")<<endl;

    auto y=l->get_output();


    config= new Config();
    config->set("num_input","2");
    auto y2=Layer::create("Concat",
                         *config,
                         "Concat")->set_input(y)
                                  ->get_output();


    vector<Node*> out;

    for(int i=0;i<y2.size();i++)
    {
        config= new Config();
        config->set("num_split","2");
        config->set("axis","1");
        auto z=Layer::create("Split",
                             *config,
                             "Split-"+to_string(i+1))->set_input(y2[i])
                                                     ->get_output();
        out.insert(out.end(),z.begin(),z.end());
    }



    Network *net =new Network({input},out);
    net->print();

    for(int k=0;k<2;k++)
    {
        cout<<k<<"-th run:"<<endl;
        net->feed({MatrixXf::Random(2,6)})->compute();
        cout<<"input:"<<endl;
        cout<<input->data<<endl;
        for(int i=0;i<net->out_nodes.size();i++)
        {
            cout<<i<<"-th output:"<<net->out_nodes[i]->name<<endl;
            cout<<net->out_nodes[i]->data<<endl;
        }
    }
}
