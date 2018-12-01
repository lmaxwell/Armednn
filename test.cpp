#include <iostream>
#include "split.h"
//#include "node.h"
#include "layer.h"
#include "net.h"

using namespace std;
using Eigen::MatrixXf;


vector<Node *> build(string name, Node* x)
{
    return Layer::create(name)->set_config("num_split","2")->set_input(x)->get_output();
}


int main()
{
    Node* input=new Node("input");

    auto y=Layer::create("Split",
                         "Split-0")->set_config("num_split","2")
                                   ->set_config("axis","1")
                                   ->set_input(input)
                                   ->get_output();

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


    Net *net =new Net({input},out);
    //net->bind({input},out);
    net->print();

    for(int k=0;k<2;k++)
    {
        cout<<k<<"-th run:"<<endl;
        net->feed({MatrixXf::Random(3,4)})->compute();
        cout<<"input:"<<endl;
        cout<<input->data<<endl;
        for(int i=0;i<net->out_nodes.size();i++)
        {
            cout<<i<<"-th output:"<<net->out_nodes[i]->name<<endl;
            cout<<net->out_nodes[i]->data<<endl;
        }
    }
}
