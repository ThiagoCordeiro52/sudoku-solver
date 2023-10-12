#include "graph.h"
#include <algorithm>
#include <cmath>

Graph::Graph(std::string data_str) {
  for (auto i{0}; i < data_str.size(); i++) {
    nodes[i] = new Node;
    if (data_str[i] == '.') {
      nodes[i]->value = DOT;
    } else {
      auto number = data_str[i] - '0';

      if (number <= 0 or number > 9) {
        std::cerr << "each char in second line must be either "
                     "a dot or a number from 0 to "
                  << RANK * RANK;
        exit(5);
      }
      nodes[i]->value = (Cell)number;
    }
  }
  // TODO: check if grid is in valid state
  // TODO: update adjacent of each node
}

void Graph::print_graph() {
  for (int i = 0; i < RANK * RANK; i++) {
    for (int j = 0; j < RANK * RANK; j++) {
      std::cout << static_cast<int>(nodes[i * RANK * RANK + j]->value) << ' ';
      if ((j + 1) % RANK == 0) {
        std::cout << "  ";
      }
    }
    std::cout << std::endl;
    if ((i + 1) % RANK == 0 && (i + 1) < RANK * RANK) {
      std::cout << std::endl;
    }
  }
}
