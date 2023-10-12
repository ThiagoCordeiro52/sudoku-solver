#ifndef GRAPH_H
#define GRAPH_H

#include "cell.h"
#include <iostream>
#include <list>
#include <string>
#include <vector>

using number_type = unsigned int;
const number_type RANK = 3;

/// This a class that represents a graph.
class Graph {
public:
  struct Node {
    Cell value;
    bool adjacent[RANK * RANK];
  };

  // Constructor
  Graph(std::string data_str);

  // Public methods

  /**
   *
   * @brief print the graph
   *
   */
  void print_graph();

private:
  // Private methods

  // Attributes
  Node *nodes[RANK * RANK * RANK * RANK];
};
#endif
