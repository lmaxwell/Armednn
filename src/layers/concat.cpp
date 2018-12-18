
#include "concat.h"
#include "iostream"

namespace Tsnn{

void Concat::prepare()
{
    num_input=get_config<size_t>("num_input");
}

void Concat::inference()
{

    Matrix& in_mat0=get_input()[0]->get_value<Matrix>();

    size_t rows=in_mat0.rows();
    size_t cols=0;

    for(int i=0;i<num_input;i++)
    {
        cols+=get_input()[i]->get_value<Matrix>().cols();
    }

    Matrix& out_mat=get_output()[0]->get_value<Matrix>();
    out_mat.resize(rows,cols);

    cols=0;
    for(int i=0;i<num_input;i++)
    {
        out_mat.block(0,cols,rows,get_input()[i]->get_value<Matrix>().cols())=get_input()[i]->get_value<Matrix>();
        cols+=get_input()[i]->get_value<Matrix>().cols();
    }

}

}
