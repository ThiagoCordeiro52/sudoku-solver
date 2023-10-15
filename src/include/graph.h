#ifndef GRAPH_H
#define GRAPH_H

#include "cell.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using number_type = unsigned int;
const number_type RANK = 3;
const number_type N_CELLS = RANK * RANK * RANK * RANK;

/// This a class that represents a graph.
class Graph {
public:
  struct Node {
    Cell value;
    bool adjacent[N_CELLS];
    number_type i;
    number_type j;

    int saturation() const {
      return std::count_if(std::begin(adjacent), std::end(adjacent),
                           [](auto has) { return has; });
    }

    Cell next_free() const {
      for (number_type i{0}; i < N_CELLS; i++) {
        if (!adjacent[i]) {
          return (Cell)(i + 1);
        }
      }
      throw new std::exception();
      bool adjacent[RANK * RANK];
    };
  };

  // Constructor
  Graph(std::string data_str);

  // Public methods

  /**
   *
   * @brief gets a node from the graph
   * @param i the line of the node
   * @param j the column of the node
   *
   */
  Node const &get_node_const(number_type i, number_type j) const;
  Node *get_node(number_type i, number_type j);

  /**
   *
   * @brief print the graph
   *
   */
  void print_graph() const;

  /**
   *
   * @brief print the graph
   *
   */
  void solve();

private:
  // Private methods
  bool is_valid_graph();
  void mark_adjacent_nodes();

  // Attributes
  Node nodes[N_CELLS];
};
#endif
