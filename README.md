

## definition
 
arm = { config , param } 
 
arm + op =armed; // check arm and op compatible

armed + inputs = node; // node { armed , inputs, outputs }, outputs is auto allocated 


node->run()
{
    armed->run(inputs,outputs)
}


armed->run(inputs,outputs)
{
    op->run(inputs,outputs,arm)
}




