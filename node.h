#ifndef _Node_H
#define _Node_H

#include "core.h"
#include <string>

class Layer;

class Node
{
    public:

        Node(){mark=2;}
        Node(size_t _channels):channels(_channels){mark=2;}
        Node(std::string _name):name(_name){mark=2;}
        Node(std::string _name,size_t _channels):name(_name),channels(_channels){mark=2;}

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
