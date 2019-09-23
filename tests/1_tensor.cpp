#include <iostream>
#define EIGEN_USE_THREADS
#include <unsupported/Eigen/CXX11/Tensor>
#include <unsupported/Eigen/CXX11/ThreadPool>
#include "armednn.h"

using namespace Armednn;

int main()
{
    std::cout << "Eigen Version " << EIGEN_WORLD_VERSION << "." << EIGEN_MAJOR_VERSION << "." << EIGEN_MINOR_VERSION << std::endl;

    // Create the Eigen ThreadPoolDevice.
    Eigen::ThreadPool pool(32);
    Eigen::ThreadPoolDevice my_device(&pool,4 /* number of threads to use */);

    int size=1000;
    Eigen::Tensor<float, 2> a(size, size);
    Eigen::Tensor<float, 2> b(size, size);

    // Now just use the device when evaluating expressions.
    Eigen::array<Eigen::IndexPair<int>, 1> product_dims = { Eigen::IndexPair<int>(1, 0) };
    Eigen::Tensor<float, 2> c(size,size);
    Timer timer;
    timer.reset();
    c.device(my_device) = a.contract(b, product_dims);
    std::cout<<"Time cost: "<<timer.elapsed()<<std::endl;


    return 0;
}