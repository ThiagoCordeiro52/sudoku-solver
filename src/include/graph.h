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

/// This a class that represents a graph.
class Graph {
public:
  struct Node {
    Cell value;
    std::vector<bool> adjacent;
    number_type i;
    number_type j;

    int saturation() const {
      return std::count_if(std::begin(adjacent), std::end(adjacent),
                           [](auto has) { return has; });
    }

    Cell next_free(number_type n_cells) const {
      for (number_type i{0}; i < n_cells; i++) {
        if (!adjacent[i]) {
          return (Cell)(i + 1);
        }
      }
      // unreachable
      throw new std::exception();
    };
  };

  // Constructor
  Graph(std::string data_str, number_type rank);

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
   * @brief solve the sudoku puzzle
   *
   */
  void solve();

private:
  // Private methods
  bool is_valid_graph();
  void mark_adjacent_nodes();

  // Attributes
  std::vector<Node> nodes;
  number_type rank;
};
#endif
