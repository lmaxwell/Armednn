#include "dense.h"
#include "iostream"


void Dense::inference()
{
    size_t dims=get_config<size_t>("dims");
    weight=Eigen::MatrixXf::Identity(dims,dims);
    bias=Eigen::VectorXf::Zero(dims);


    /*
    Matrix in_mat=get_input()[0]->get_value<Matrix>(); // 1st copy, copy from input, allocate matrix "in_mat"

    Matrix mat(in_mat.rows(),dims);  // allocate a matrix "mat"
    mat.noalias()=in_mat*weight;
    mat.rowwise()+=bias.transpose();
    get_output()[0]->set_value(mat);  // 2nd copy, copy "mat" to output

    // in total three memory allocates. 
    */


    //avoid copy 
    Matrix& in_mat=get_input()[0]->get_value<Matrix>(); // use reference, no copy here
    //std::cout<<"return side:"<<&(get_input()[0]->get_value<Matrix>())<<" "<<&in_mat<<std::endl;   

    Matrix& mat=get_output()[0]->get_value<Matrix>(); 
    mat.resize(in_mat.rows(),dims); // use resize() to reallocate memory

    //mat.setZero(); //need to do this in some cases

    mat.noalias()=in_mat*weight;
    mat.rowwise()+=bias.transpose();
    // onlly one allocate

}
