
#ifndef _OPERATOR_H 
#define _OPERATOR_H

#include <memory>
#include <vector>
#include <string>


namespace Armednn
{

class Data;
class Arm;
typedef std::vector<std::shared_ptr<Data>> DataPtr;


class Operator
{
    public:

        virtual void inference(DataPtr& inputs, DataPtr& outputs, Arm& arm)=0;
        virtual ~Operator()=default;

        void type(std::string type)
        {
            _type=type;
        }

        std::string type()
        {
            return _type;
        }

        void name(std::string name)
        {
            _name=name;
        }

        std::string name()
        {
            return _name;
        }

    private:
        std::string _type="None"; 
        std::string _name="None"; 

};



}// namespace Armednn

#endif
