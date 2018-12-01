#include "dense.h"
#include "iostream"

std::vector<Node *> Dense::get_output()
{
    add_output();
    return out_nodes;
}

void Dense::inference()
{
    weight=Eigen::MatrixXf::Random(in_nodes[0]->get_channels(),4);
    bias=Eigen::VectorXf::Random(4);
    out_nodes[0]->data=in_nodes[0]->data*weight;
    out_nodes[0]->data.rowwise()+=bias.transpose();
}
