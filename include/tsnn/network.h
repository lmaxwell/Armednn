#ifndef _Network_H_
#define _Network_H_
#include "tsnn/layer.h"

namespace Tsnn{
class Network
{
    public:

        Network(){
            mark=0;
        }

        Network(std::vector<pData> _in_nodes,std::vector<pData> _out_nodes){
            mark=0;
            bind(_in_nodes,_out_nodes);
        }

        Network* input(std::vector<pData> nodes)
        {
            in_nodes=nodes;
            return this;
        }

        Network* output(std::vector<pData> nodes)
        {
            out_nodes=nodes;
            return this;
        }

        Network* bind(std::vector<pData> _in_nodes,std::vector<pData> _out_nodes)
        {
            input(_in_nodes);
            output(_out_nodes);
            return this;
        }

        Network* feed(const std::vector<Eigen::MatrixXf> &data)
        {
            for(int i=0;i<in_nodes.size();i++)
            {
                in_nodes[i]->set_value(data[i]);
            }
            return this;
        }

        void compute_node(pData node)
        {


            std::vector<pData>::iterator ite=std::find(in_nodes.begin(),in_nodes.end(),node);

            if (ite!=in_nodes.end()) //this node is input node
                return;

            Layer* from=node->from;

            if(from->mark==mark) // this node has been computed
            {
                return;
            }

            for(int i=0;i<from->get_input().size();i++)
            {
                compute_node(from->get_input()[i]);
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


        void iterate_layers(pData node)
        {
            std::vector<pData>::iterator ite=std::find(in_nodes.begin(),in_nodes.end(),node);

            if (ite!=in_nodes.end()) 
                return;

            Layer* from=node->from;

            if(layers.find(from)!=layers.end()) 
            {
                return;
            }

            layers.insert(from);
            for(int i=0;i<from->get_input().size();i++)
            {
                iterate_layers(from->get_input()[i]);
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
                for(int i=0;i<(*it)->get_output().size();i++)
                {
                    temp+=(*it)->get_output()[i]->name;
                    if(i!=(*it)->get_output().size()-1)
                        temp+=" ";
                }
                temp+=" }\t{ ";
                for(int i=0;i<(*it)->get_input().size();i++)
                {
                    temp+=(*it)->get_input()[i]->name;
                    if(i!=(*it)->get_input().size()-1)
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

        std::vector<pData>in_nodes;
        std::vector<pData>out_nodes;
        std::set<Layer *>layers;

};
}


#endif
