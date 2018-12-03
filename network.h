#ifndef _Network_H_
#define _Network_H_
#include "layer.h"

class Network
{
    public:

        Network(){
            mark=0;
        }

        Network(std::vector<Node *> _in_nodes,std::vector<Node *> _out_nodes){
            mark=0;
            bind(_in_nodes,_out_nodes);
        }

        Network* input(std::vector<Node *> nodes)
        {
            in_nodes=nodes;
            return this;
        }

        Network* output(std::vector<Node *> nodes)
        {
            out_nodes=nodes;
            return this;
        }

        Network* bind(std::vector<Node *> _in_nodes,std::vector<Node *> _out_nodes)
        {
            input(_in_nodes);
            output(_out_nodes);
            return this;
        }

        Network* feed(const std::vector<Eigen::MatrixXf> &data)
        {
            for(int i=0;i<in_nodes.size();i++)
            {
                in_nodes[i]->data=data[i];
            }
            return this;
        }

        void compute_node(Node* node)
        {


            std::vector<Node*>::iterator ite=std::find(in_nodes.begin(),in_nodes.end(),node);

            if (ite!=in_nodes.end()) //this node is input node
                return;

            Layer* from=node->from;

            if(from->mark==mark) // this node has been computed
            {
                return;
            }

            for(int i=0;i<in_nodes.size();i++)
            {
                compute_node(from->in_nodes[i]);
            }

            from->inference();
            from->mark=mark;
        }


        void* compute()
        {
            mark=1-mark; // mark of computation

            for(int i=0;i<out_nodes.size();i++)
            {
                compute_node(out_nodes[i]);
            }
        }


        void iterate_layers(Node *node)
        {
            std::vector<Node*>::iterator ite=std::find(in_nodes.begin(),in_nodes.end(),node);

            if (ite!=in_nodes.end()) 
                return ;

            Layer* from=node->from;

            if(layers.find(from)!=layers.end()) 
            {
                return ;
            }

            layers.insert(from);
            for(int i=0;i<in_nodes.size();i++)
            {
                iterate_layers(from->in_nodes[i]);
            }
        }


        void print()
        {
            for(int i=0;i<out_nodes.size();i++)
            {
                iterate_layers(out_nodes[i]);
            }

            std::set<Layer *>::iterator it;
            for(it=layers.begin();it!=layers.end();it++)
            {
                std::string temp=(*it)->name;
                temp+="\t{ ";
                for(int i=0;i<(*it)->out_nodes.size();i++)
                {
                    temp+=(*it)->out_nodes[i]->name;
                    if(i!=(*it)->out_nodes.size()-1)
                        temp+=" ";
                }
                temp+=" }\t{ ";
                for(int i=0;i<(*it)->in_nodes.size();i++)
                {
                    temp+=(*it)->in_nodes[i]->name;
                    if(i!=(*it)->in_nodes.size()-1)
                        temp+=" ";
                }
                temp+=" }";
                std::cout<<temp<<std::endl;
            }
            std::string temp="OUTPUT:\t";
            for(int i=0;i<out_nodes.size();i++)
            {
                temp+=out_nodes[i]->name;
                if(i!=out_nodes.size()-1)
                    temp+=" ";
            }
            std::cout<<temp<<std::endl;

        }

        size_t mark;

        std::vector<Node *>in_nodes;
        std::vector<Node *>out_nodes;
        std::set<Layer *>layers;

};


#endif
