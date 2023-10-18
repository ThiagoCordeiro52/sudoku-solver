#include "graph.h"
#include "cell.h"
#include "errors.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <ranges>

Graph::Graph(std::string data_str, number_type rank) {
  this->rank = rank;
  this->nodes.reserve(rank * rank * rank * rank);
  this->largest_value = DOT;
  for (auto k{0}; k < data_str.size(); k++) {
    Cell cell;
    if (data_str[k] == '.') {
      cell = DOT;
    } else {
      auto number = data_str[k] - '0';
      if (number > 9) {
        number = data_str[k] - 'A' + 10;
      }
      if (rank >= 5) {
        number = data_str[k] - 'A' + 1;
      }
      if (number <= 0 && number > rank * rank) {
        std::cerr << "each char in second line must be either "
                     "a dot or a number from 1 to 9 or a letter from A to "
                  << (char)(rank * rank - 10);
        exit(INVALID_CHAR);
      }
      cell = (Cell)number;
    }
    if (cell > this->largest_value) {
      this->largest_value = cell;
    }
    std::vector<bool> adjacent(rank * rank * rank * rank, false);

    auto i = k / (rank * rank);
    auto j = k % (rank * rank);
    nodes.push_back({cell, adjacent, i, j});
  }

  if (!is_valid_graph()) {
    std::cerr << "Invalid sudoku read from input file.";
    exit(INVALID_SUDOKU);
  }

  mark_adjacent_nodes();
}

Graph::Node const &Graph::get_node_const(number_type i, number_type j) const {
  return nodes[i * rank * rank + j];
}

Graph::Node *Graph::get_node(number_type i, number_type j) {
  return &nodes[i * rank * rank + j];
}

void Graph::print_graph() const {
  std::cout << "|";
  std::cout << std::setfill('-') << std::setw(rank * (2 * rank + 2) - 1) << "";
  std::cout << "|" << std::endl;
  for (int i = 0; i < rank * rank; i++) {
    std::cout << "| ";
    for (int j = 0; j < rank * rank; j++) {
      auto cell = get_node_const(i, j).value;
      if (cell == DOT) {
        std::cout << "  ";
      } else if (rank >= 5) {
        std::cout << (char)('A' + static_cast<int>(cell) - 1) << ' ';
      } else if (cell <= 9) {
        std::cout << static_cast<int>(cell) << ' ';
      } else {
        std::cout << (char)('A' + static_cast<int>(cell) - 10) << ' ';
      }
      if ((j + 1) % rank == 0) {
        std::cout << "| ";
      }
    }
    std::cout << std::endl;
    if ((i + 1) % rank == 0) {
      std::cout << "|";
      std::cout << std::setfill('-') << std::setw(rank * (2 * rank + 2) - 1)
                << "";
      std::cout << "|" << std::endl;
    }
  }
}

bool Graph::is_valid_graph() {
  // check the lines
  for (int i = 0; i < rank * rank; i++) {
    std::vector<bool> isNumberPresent(rank * rank, false);

    for (int j = 0; j < rank * rank; j++) {
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
  for (int j = 0; j < rank * rank; j++) {
    std::vector<bool> isNumberPresent(rank * rank, false);

    for (int i = 0; i < rank * rank; i++) {
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
  for (int i = 0; i < rank; i++) {
    for (int j = 0; j < rank; j++) {
      std::vector<bool> isNumberPresent(rank * rank, false);

      for (int row = i * rank; row < (i + 1) * rank; row++) {
        for (int col = j * rank; col < (j + 1) * rank; col++) {
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

  for (int i = 0; i < rank * rank * rank * rank; i++) {
    int row = i / (rank * rank);
    int col = i % (rank * rank);

    auto node_value = get_node(row, col)->value;

    if (node_value != DOT) {
      for (int j = 0; j < rank * rank; j++) {
        get_node(row, j)->adjacent[node_value - 1] = true;
        get_node(j, col)->adjacent[node_value - 1] = true;
      }

      int gridRow = row / rank;
      int gridColumn = col / rank;

      for (int k = gridRow * rank; k < (gridRow + 1) * rank; k++) {
        for (int l = gridColumn * rank; l < (gridColumn + 1) * rank; l++) {
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

    max->value = max->next_free(rank * rank * rank * rank);

    if (max->value > this->largest_value) {
      this->largest_value = max->value;
    }

    for (auto j{0}; j < rank * rank; j++) {
      if (j != max->j) {
        auto node = get_node(max->i, j);
        node->adjacent[max->value - 1] = true;
      }
    }
    for (auto i{0}; i < rank * rank; i++) {
      if (i != max->i) {
        auto node = get_node(i, max->j);
        node->adjacent[max->value - 1] = true;
      }
    }
    int startRow = (max->i / rank) * rank;
    int startCol = (max->j / rank) * rank;

    for (int i = startRow; i < startRow + rank; ++i) {
      for (int j = startCol; j < startCol + rank; ++j) {
        if (i != max->i && j != max->j) {
          auto node = get_node(i, j);
          node->adjacent[max->value - 1] = true;
        }
      }
    }
  }
}
