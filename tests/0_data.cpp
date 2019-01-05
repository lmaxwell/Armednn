#include "armednn.h"

using namespace Armednn;


void test1()
{
    Data d;
    INFO<<d.get().size()<<" "<<d.get().data();

    d.allocate(10,10);
    INFO<<d.get().size()<<" "<<d.get().data();

    Eigen::internal::set_is_malloc_allowed(false);
    d.allocate(5,5);
    INFO<<d.get().size()<<" "<<d.get().data();
    Eigen::internal::set_is_malloc_allowed(true);

    d.allocate(100,1000);
    INFO<<d.get().size()<<" "<<d.get().data();
}

void test2()
{

    Timer timer;
    int NUM=40;
    Matrix a(NUM,NUM);

    Matrix b(NUM,NUM);

    Matrix c(NUM,NUM);


    timer.reset();
    c.noalias()=a*b;
    INFO<<timer.elapsed();


    Data d;
    d.allocate(2*NUM,2*NUM);
    timer.reset();
    d.allocate(NUM,NUM);
    INFO<<"no malloc, cost:"<<timer.elapsed();


    Eigen::internal::set_is_malloc_allowed(false);

    timer.reset();
    INFO<<d.get().data();
    d.get().noalias()=a*b;
    INFO<<d.get().data();
    INFO<<timer.elapsed();


    CHECK(d.get()==c);

    d.get()=c;
    Eigen::internal::set_is_malloc_allowed(true);

    timer.reset();
    c=a*b;
    INFO<<"alias:"<<timer.elapsed();
    timer.reset();
    c.noalias()=a*b;
    INFO<<"no alias:"<<timer.elapsed();

}



int main()
{
    test1();
    test2();
}
