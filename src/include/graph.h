#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
using std::cout;
#include <list>
using std::list;

/// This a class that represents a graph.
class Graph {
    public:
        using number_type = unsigned int;

        struct Node {
            number_type value;
            list<Node*> adjacentNodes;
        };
        
        // Constructor
        Graph();

        // Public methods

        /**
        *
        * @brief add a node to the graph
        * @param value the value of the node
        */
        void add_node(number_type value);

        /**
        *
        * @brief print the graph
        *
        */
        void print_graph();
  
    private:
        // Private methods

        // Attributes
        list<Node*> nodes;
};
#endif