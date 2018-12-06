#include "dense.h"
#include "iostream"

namespace Tsnn{

void Dense::inference()
{

    Eigen::Map<Matrix> weight=get_param<Matrix>("weight");
    //Vector& bias=Eigen::Map<Vector>(get_param<Matrix>("bias").data(),1,get_param<Matrix>("bias").cols());
    Eigen::Map<Vector> bias=get_param<Vector>("bias");

    std::cout<<weight.data()<<" "<<bias.data()<<std::endl;


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
    mat.resize(in_mat.rows(),get_config<size_t>("dim1")); // use resize() to reallocate memory

    //mat.setZero(); //need to do this in some cases

    mat.noalias()=in_mat*weight;
    mat.rowwise()+=bias;
    // onlly one allocate

}


}
