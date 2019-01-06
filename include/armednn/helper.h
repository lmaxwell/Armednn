#ifndef _HELPER_H
#define _HELPER_H

#include <memory>
#include "armednn/common.h"
#include "armednn/config.h"
#include "armednn/param.h"
#include "armednn/operator.h"
#include "armednn/op_regist.h"
#include "armednn/node.h"

namespace Armednn
{
/*
make_op
list_op
help_op
*/

/*
 make_armed
*/

/*
 make_node
*/

std::unique_ptr<Operator> make_op(std::string type,std::string name);

std::unique_ptr<Armed> make_armed(Arm& arm, std::unique_ptr<Operator> op);

std::unique_ptr<Node> make_node(DataPtr inputs, std::unique_ptr<Armed> armed, DataPtr outputs=DataPtr());



std::unique_ptr<Node> make_node(std::string type, Arm& arm, DataPtr inputs, std::string name, DataPtr outputs=DataPtr());

std::unique_ptr<Node> make_input(std::string name);

std::unique_ptr<Node> make_state(std::string name, Matrix value );

}// namespace Armednn
#endif
