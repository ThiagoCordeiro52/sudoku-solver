#include "graph.h"

void Graph::add_node(number_type value)
{
    Node *new_node{new Node};
    new_node->value = value;
}

void Graph::print_graph()
{
    for (Node *node : nodes)
    {
        std::cout << "Node " << node->value << " adjacent to: ";
        for (Node *adjacent : node->adjacentNodes)
        {
            std::cout << adjacent->value << " ";
        }
        std::cout << std::endl;
    }
}