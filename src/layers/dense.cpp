#include "dense.h"
#include "tsnn/op.h"
#include "iostream"
#include  <chrono>

namespace Tsnn{


void Dense::prepare()
{
    weight=get_param("weight");
    bias=get_param("bias");
    dim0=get_config<size_t>("dim0");
    dim1=get_config<size_t>("dim1");
    activation=get_config<std::string>("activation");
}


void Dense::inference()
{

    //Eigen::Map weight=get_param("weight");
    //Matrix weight=get_param("weight");
    //Vector& bias=Eigen::Map<Vector>(get_param("bias").data(),1,get_param("bias").cols());
    //Vector bias=get_param<Vector>("bias");



    /*
    Matrix in_mat=get_input()[0]->get_value(); // 1st copy, copy from input, allocate matrix "in_mat"

    Matrix mat(in_mat.rows(),dims);  // allocate a matrix "mat"
    mat.noalias()=in_mat*weight;
    mat.rowwise()+=bias.transpose();
    get_output()[0]->set_value(mat);  // 2nd copy, copy "mat" to output

    // in total three memory allocates. 
    */

    //avoid copy 
    Matrix& in_mat=get_input()[0]->get_value(); // use reference, no copy here
    //std::cout<<"return side:"<<&(get_input()[0]->get_value())<<" "<<&in_mat<<std::endl;   

    Matrix& mat=get_output()[0]->get_value(); 

    mat.resize(in_mat.rows(),dim1); // use resize() to reallocate memory

    //mat.setZero(); //need to do this in some cases

    auto start=std::chrono::system_clock::now();

    CHECK(Op::multiply(in_mat,weight,mat));

    auto end=std::chrono::system_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    INFO<<"Multiply COST "<<double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

    start=std::chrono::system_clock::now();

    CHECK(Op::add(mat,bias));

    end=std::chrono::system_clock::now();
    duration=std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    INFO<<"Add COST "<<double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;


    CHECK(Op::activation(activation)(mat));


}


}//Tsnn 
