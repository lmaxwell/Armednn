
#include "concat.h"
#include "iostream"


void Concat::inference()
{

    size_t rows=get_input()[0]->get_value<Matrix>().rows();
    size_t cols=0;

    for(int i=0;i<get_input().size();i++)
    {
        cols+=get_input()[i]->get_value<Matrix>().cols();
    }

    Eigen::MatrixXf D(rows,cols);

    cols=0;
    for(int i=0;i<get_input().size();i++)
    {
        D.block(0,cols,rows,get_input()[i]->get_value<Matrix>().cols())=get_input()[i]->get_value<Matrix>();
        cols+=get_input()[i]->get_value<Matrix>().cols();
    }

    get_output()[0]->set_value(D);
}
