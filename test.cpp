#include <iostream>
#include "split.h"
//#include "node.h"
#include "layer.h"
#include "network.h"

#include <map>
#include <string>

using namespace std;
using Eigen::MatrixXf;
using Eigen::Map;
using Eigen::Matrix;
using namespace Eigen;


vector<Node *> build(string name, Node* x)
{
    return Layer::create(name)->set_config("num_split","2")->set_input(x)->get_output();
}

void test(map<std::string,std::string> configs)
{
    ;
}

int main()
{

    /*
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

    Node* input=new Node("input");


    auto x=Layer::create("Dense",
                        "Dense")->set_config("activation","tanh")
                                ->set_config("dims","4")
                                ->set_input(input)
                                ->get_output();

    auto l=Layer::create("Split",
                         "Split-0")->set_config("num_split","2")
                                   ->set_config("axis","1")
                                   ->set_input(x);

    auto y=l->get_output();

    vector<Node*> out;

    for(int i=0;i<y.size();i++)
    {
        auto z=Layer::create("Split",
                             "Split-"+to_string(i+1))->set_config("num_split","2")
                                                     ->set_config("axis","1")
                                                     ->set_input(y[i])
                                                     ->get_output();
        out.insert(out.end(),z.begin(),z.end());
    }

    cout<<l->configs["num_split"]<<endl;


    Network *net =new Network({input},out);
    //net->bind({input},out);
    net->print();

    for(int k=0;k<2;k++)
    {
        cout<<k<<"-th run:"<<endl;
        net->feed({MatrixXf::Random(2,2)})->compute();
        cout<<"input:"<<endl;
        cout<<input->data<<endl;
        for(int i=0;i<net->out_nodes.size();i++)
        {
            cout<<i<<"-th output:"<<net->out_nodes[i]->name<<endl;
            cout<<net->out_nodes[i]->data<<endl;
        }
    }
}
