#include "graph.h"
#include "cell.h"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <ostream>
#include <ranges>

Graph::Graph(std::string data_str) {
  for (auto i{0}; i < data_str.size(); i++) {
    nodes[i].i = i / (RANK * RANK);
    nodes[i].j = i % (RANK * RANK);
    if (data_str[i] == '.') {
      nodes[i].value = DOT;
    } else {
      auto number = data_str[i] - '0';

      if (number <= 0 or number > 9) {
        std::cerr << "each char in second line must be either "
                     "a dot or a number from 1 to "
                  << RANK * RANK;
        exit(5);
      }
      nodes[i].value = (Cell)number;
    }
    for (auto j{0}; j < N_CELLS; j++) {
      nodes[i].adjacent[j] = false;
    }
  }

  if (!is_valid_graph()) {
    std::cerr << "Invalid sudoku read from input file.";
    exit(6);
  }

  mark_adjacent_nodes();
}

Graph::Node const &Graph::get_node_const(number_type i, number_type j) const {
  return nodes[i * RANK * RANK + j];
}

Graph::Node *Graph::get_node(number_type i, number_type j) {
  return &nodes[i * RANK * RANK + j];
}

void Graph::print_graph() const {
  for (int i = 0; i < RANK * RANK; i++) {
    for (int j = 0; j < RANK * RANK; j++) {
      auto cell = get_node_const(i, j).value;
      if (cell == DOT) {
        std::cout << "  ";
      } else if (cell <= 9) {
        std::cout << static_cast<int>(cell) << ' ';
      } else {
        std::cout << (char)('A' + static_cast<int>(cell) - 10) << ' ';
      }
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

bool Graph::is_valid_graph() {
  // check the lines
  for (int i = 0; i < RANK * RANK; i++) {
    bool isNumberPresent[9]{false};

    for (int j = 0; j < RANK * RANK; j++) {
      int value = static_cast<int>(get_node_const(i, j).value);

      if (value == DOT)
        continue;

      if (isNumberPresent[value - 1]) {
        return false;
      }

      isNumberPresent[value - 1] = true;
    }
  }

  // check the columns
  for (int j = 0; j < RANK * RANK; j++) {
    bool isNumberPresent[9]{false};

    for (int i = 0; i < RANK * RANK; i++) {
      int value = static_cast<int>(get_node_const(i, j).value);

      if (value == DOT)
        continue;

      if (isNumberPresent[value - 1]) {
        return false;
      }

      isNumberPresent[value - 1] = true;
    }
  }

  // check the subgrid
  for (int i = 0; i < RANK; i++) {
    for (int j = 0; j < RANK; j++) {
      bool isNumberPresent[9]{false};

      for (int row = i * RANK; row < (i + 1) * RANK; row++) {
        for (int col = j * RANK; col < (j + 1) * RANK; col++) {
          int value = static_cast<int>(get_node_const(row, col).value);

          if (value == DOT)
            continue;

          if (isNumberPresent[value - 1]) {
            return false;
          }

          isNumberPresent[value - 1] = true;
        }
      }
    }
  }

  return true;
}

void Graph::mark_adjacent_nodes() {

  for (int i = 0; i < N_CELLS; i++) {
    int row = i / (RANK * RANK);
    int col = i % (RANK * RANK);

    auto node_value = get_node(row, col)->value;

    if (node_value != DOT) {
      for (int j = 0; j < RANK * RANK; j++) {
        get_node(row, j)->adjacent[node_value - 1] = true;
        get_node(j, col)->adjacent[node_value - 1] = true;
      }

      int gridRow = row / RANK;
      int gridColumn = col / RANK;

      for (int k = gridRow * RANK; k < (gridRow + 1) * RANK; k++) {
        for (int l = gridColumn * RANK; l < (gridColumn + 1) * RANK; l++) {
          get_node(k, l)->adjacent[node_value - 1] = true;
        }
      }
    }
  }
}

void Graph::solve() {
  auto is_empty = [](Node &node) { return node.value == DOT; };

  while (true) {
    auto empty = nodes | std::views::filter(is_empty);

    auto max = std::max_element(
        empty.begin(), empty.end(), [](auto node_a, auto node_b) {
          return node_a.saturation() <= node_b.saturation();
        });

    if (max == empty.end()) {
      break;
    }

    max->value = max->next_free();

    for (auto j{0}; j < RANK * RANK; j++) {
      if (j != max->j) {
        auto node = get_node(max->i, j);
        node->adjacent[max->value - 1] = true;
      }
    }
    for (auto i{0}; i < RANK * RANK; i++) {
      if (i != max->i) {
        auto node = get_node(i, max->j);
        node->adjacent[max->value - 1] = true;
      }
    }
    int startRow = (max->i / 3) * 3;
    int startCol = (max->j / 3) * 3;

    for (int i = startRow; i < startRow + 3; ++i) {
      for (int j = startCol; j < startCol + 3; ++j) {
        if (i != max->i && j != max->j) {
          auto node = get_node(i, j);
          node->adjacent[max->value - 1] = true;
        }
      }
    }
  }
}
