#ifndef _Data_H
#define _Data_H

#include "tsnn/core.h"
#include <string>

class Layer;

class Data
{
    public:

        Data(){mark=2;}
        Data(size_t _channels):channels(_channels){mark=2;}
        Data(std::string _name):name(_name){mark=2;}
        Data(std::string _name,size_t _channels):name(_name),channels(_channels){mark=2;}

        //name of the node
        std::string name;

        //name of the layer which this node is from
        Layer* from;

        //name of the layer which this node point to, not used now
        Layer* to;

        //data of the node
        Eigen::MatrixXf data;

        size_t channels;
        size_t length;

        size_t get_channels(){return data.cols();}
        size_t get_length(){return data.rows();}
        size_t mark;
};
#endif