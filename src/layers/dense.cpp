#include "dense.h"
#include "iostream"


void Dense::inference()
{
    size_t dims=get_config<size_t>("dims");
    weight=Eigen::MatrixXf::Identity(dims,dims);
    bias=Eigen::VectorXf::Zero(dims);
    (get_output()[0]->data).noalias()=get_input()[0]->data*weight;
    get_output()[0]->data.rowwise()+=bias.transpose();
}
