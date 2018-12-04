
#include "concat.h"
#include "iostream"


void Concat::inference()
{
    size_t rows=get_input()[0]->data.rows();
    size_t cols=0;

    for(int i=0;i<get_input().size();i++)
    {
        cols+=get_input()[i]->data.cols();
    }

    Eigen::MatrixXf D(rows,cols);
    cols=0;
    for(int i=0;i<get_input().size();i++)
    {
        D.block(0,cols,get_input()[i]->data.rows(),get_input()[i]->data.cols())=get_input()[i]->data;
        cols+=get_input()[i]->data.cols();
    }
    get_output()[0]->data=D;
}
