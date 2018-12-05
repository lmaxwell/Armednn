#include "dense.h"
#include "iostream"


void Dense::inference()
{
    size_t dims=get_config<size_t>("dims");
    weight=Eigen::MatrixXf::Identity(dims,dims);
    bias=Eigen::VectorXf::Zero(dims);

    Matrix mat;
    mat.noalias()=get_input()[0]->get_value<Matrix>()*weight;
    mat.rowwise()+=bias.transpose();

    get_output()[0]->set_value(mat);
}
