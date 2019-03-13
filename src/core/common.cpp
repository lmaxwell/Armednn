#include "armednn/common.h"

namespace Armednn
{

#ifndef NDEBUG
bool LogMessage::_enable=true;
#else
bool LogMessage::_enable=false;
#endif

}//namespace Armednn
