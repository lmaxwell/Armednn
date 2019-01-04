#include "armednn.h"

using namespace Armednn;

int main()
{

    Data d;
    INFO<<d.get().size()<<" "<<d.get().data();

    d.allocate(10,10);
    INFO<<d.get().size()<<" "<<d.get().data();

    d.allocate(5,5);
    INFO<<d.get().size()<<" "<<d.get().data();

    d.allocate(100,1000);
    INFO<<d.get().size()<<" "<<d.get().data();


}
