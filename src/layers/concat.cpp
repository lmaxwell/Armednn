
#include "concat.h"
#include "iostream"

namespace Tsnn{

void Concat::prepare()
{
    CHECK(get_config("num_input",num_input));

}

void Concat::inference()
{

    Matrix& in_mat0=get_input()[0]->get_value();

    size_t rows=in_mat0.rows();
    size_t cols=0;

    for(int i=0;i<num_input;i++)
    {
        cols+=get_input()[i]->get_value().cols();
    }

    Matrix& out_mat=get_output()[0]->get_value();
    out_mat.resize(rows,cols);

    cols=0;
    for(int i=0;i<num_input;i++)
    {
        out_mat.block(0,cols,rows,get_input()[i]->get_value().cols())=get_input()[i]->get_value();
        cols+=get_input()[i]->get_value().cols();
    }

}

}
