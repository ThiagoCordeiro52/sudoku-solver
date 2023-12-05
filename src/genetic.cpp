#include "include/genetic.h"
#include "include/errors.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ranges>

const int POPULATION_SIZE = 3;

SudokuBoard::SudokuBoard(std::string data_str, number_type rank) {
  this->rank = rank;
  this->nodes.reserve(rank * rank * rank * rank);
  for (auto k{0}; k < data_str.size(); k++) {
    cell_type cell;
    if (data_str[k] == '.') {
      cell = 0;
    } else {
      auto number = data_str[k] - '0';
      if (number > 9) {
        number = data_str[k] - 'A' + 10;
      }
      if (rank >= 5) {
        number = data_str[k] - 'A' + 1;
      }
      if (number <= 0 || number > rank * rank) {
        std::cerr << "each char in second line must be either "
                     "a dot or a number from 1 to 9 or a letter from A to "
                  << (char)(rank * rank - 10);
        exit(INVALID_CHAR);
      }
      cell = number;
    }
    std::vector<bool> adjacent(rank * rank, false);

    auto i = k / (rank * rank);
    auto j = k % (rank * rank);
    nodes.push_back({cell, adjacent, i, j});
  }
  mark_adjacent_nodes();
}

SudokuBoard::Node *SudokuBoard::get_node(number_type i, number_type j) {
  return &nodes[i * rank * rank + j];
}

void SudokuBoard::mark_adjacent_nodes() {

  for (int i = 0; i < rank * rank * rank * rank; i++) {
    int row = i / (rank * rank);
    int col = i % (rank * rank);

    auto node_value = get_node(row, col)->value;

    if (node_value != 0) {
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

std::pair<SudokuBoard, SudokuBoard>
SudokuBoard::crossover(const SudokuBoard &first, const SudokuBoard &second) {
  auto size = first.rank * first.rank;
  SudokuBoard first_child;
  SudokuBoard second_child;
  first_child.nodes = second_child.nodes = std::vector<Node>(size * size);
  first_child.rank = second_child.rank = first.rank;
  int crossoverPoint = rand() % (size - 1) + 1;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      auto index = i * size + j;
      if (j < crossoverPoint) {
        first_child.nodes[index] = first.nodes[index];
        second_child.nodes[index] = second.nodes[index];
      } else {
        first_child.nodes[index] = second.nodes[index];
        second_child.nodes[index] = first.nodes[index];
      }
    }
  }

  return std::make_pair(first_child, second_child);
};
void SudokuBoard::mutate(SudokuBoard &individual, double mutation_rate) const {
  auto size = rank * rank;
  for (int i = 0; i < size; ++i) {
    if ((rand() % 100) < mutation_rate * 100) {
      int j;
      do {
        j = rand() % size;
      } while (nodes[i * size + j].value != 0);
      int best_candidate = j == 0 ? 1 : 0;
      std::iter_swap(individual.nodes.begin() + i * size + j,
                     individual.nodes.begin() + i * size + best_candidate);
      number_type best_fitness = fitness();
      std::iter_swap(individual.nodes.begin() + i * size + j,
                     individual.nodes.begin() + i * size + best_candidate);
      for (auto candidate = 0; candidate < size; candidate++) {
        if (j == candidate ||
            individual.nodes[i * size + candidate].value == 0 ||
            nodes[i * size + candidate].value != 0) {
          continue;
        }
        std::iter_swap(individual.nodes.begin() + i * size + j,
                       individual.nodes.begin() + i * size + candidate);
        number_type fitness_ = fitness();
        std::iter_swap(individual.nodes.begin() + i * size + j,
                       individual.nodes.begin() + i * size + candidate);
        if (fitness_ < best_fitness) {
          best_candidate = candidate;
          best_fitness = fitness_;
        }
      }
      std::iter_swap(individual.nodes.begin() + i * size + j,
                     individual.nodes.begin() + i * size + best_candidate);
    }
  }
};

int SudokuBoard::fitness() const {
  auto size = rank * rank;
  int conflicts = 0;
  for (int row = 0; row < size; ++row) {
    for (int col = 0; col < size; ++col) {
      int num = nodes[row * size + col].value;
      if (num != 0) {
        for (int i = 0; i < size; ++i) {
          conflicts +=
              (num == nodes[row * size + i].value ||
               num == nodes[i * size + col].value ||
               num == nodes[(row / rank) * rank * size + (col / rank) * rank +
                            (i / rank) * size + (i % rank)]
                          .value);
        }
      }
    }
  }
  return conflicts;
};

void SudokuBoard::genetic_algorithm(double mutation_rate, int max_generations) {
  srand(static_cast<unsigned>(time(nullptr)));

  std::vector<SudokuBoard> population(POPULATION_SIZE);
  for (int i = 0; i < POPULATION_SIZE; ++i) {
    population[i] = generate();
  }
  auto compare_fitness = [](const SudokuBoard &a, const SudokuBoard &b) {
    return a.fitness() <= b.fitness();
  };

  for (int generation = 0; generation < max_generations; ++generation) {
    std::sort(population.begin(), population.end(), compare_fitness);

    if (population[0].fitness() == 0) {
      std::cout << "Solution found in generation " << generation + 1
                << std::endl;
      this->nodes = population[0].nodes;
      return;
    }

    std::vector<SudokuBoard> next_generation;
    for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
      int parent1_index = rand() % (POPULATION_SIZE / 2);
      int parent2_index = rand() % (POPULATION_SIZE / 2);
      auto children =
          crossover(population[parent1_index], population[parent2_index]);
      mutate(children.first, mutation_rate);
      mutate(children.second, mutation_rate);
      next_generation.push_back(children.first);
      next_generation.push_back(children.second);
    }

    population = next_generation;
  }

  std::cout << "No solution found" << std::endl;

  std::sort(population.begin(), population.end(), compare_fitness);

  this->nodes = population[0].nodes;
};

bool SudokuBoard::is_valid(number_type row, number_type col,
                           cell_type num) const {
  auto size = rank * rank;
  // check lines and cols
  for (int i = 0; i < size; ++i) {
    if (nodes[row * size * size + i].value == num ||
        nodes[i * size * size + col].value == num) {
      return false;
    }
  }

  // check subgrid
  int start_row = row - row % rank;
  int start_col = col - col % rank;
  for (int i = 0; i < rank; ++i) {
    for (int j = 0; j < rank; ++j) {
      auto index = (start_row + i) * size * size + (start_col + j);
      if (nodes[index].value == num) {
        return false;
      }
    }
  }

  return true;
};

SudokuBoard SudokuBoard::generate() {
  SudokuBoard individual;
  individual.nodes.reserve(rank * rank * rank * rank);
  for (auto node : nodes) {
    Node new_node;
    new_node.adjacent = node.adjacent;
    new_node.value = node.value;
    new_node.i = node.i;
    new_node.j = node.j;
    individual.nodes.push_back(new_node);
  }
  individual.rank = rank;

  while (true) {
    bool noEmpty = true;

    for (auto node : individual.nodes) {
      if (node.value == 0) {
        noEmpty = false;
        break;
      }
    }

    if (noEmpty) {
      break;
    }

    Node *rand_node;

    do {
      rand_node = &individual.nodes[rand() % individual.nodes.size()];
    } while (rand_node->value != 0);

    rand_node->value = rand_node->get_random_free(rank * rank);

    for (auto j{0}; j < rank * rank; j++) {
      if (j != rand_node->j) {
        auto node = individual.get_node(rand_node->i, j);
        node->adjacent[rand_node->value - 1] = true;
      }
    }
    for (auto i{0}; i < rank * rank; i++) {
      if (i != rand_node->i) {
        auto node = individual.get_node(i, rand_node->j);
        node->adjacent[rand_node->value - 1] = true;
      }
    }
    int startRow = (rand_node->i / rank) * rank;
    int startCol = (rand_node->j / rank) * rank;

    for (int i = startRow; i < startRow + rank; ++i) {
      for (int j = startCol; j < startCol + rank; ++j) {
        if (i != rand_node->i && j != rand_node->j) {
          auto node = individual.get_node(i, j);
          node->adjacent[rand_node->value - 1] = true;
        }
      }
    }
  }
  return individual;
};

void SudokuBoard::print_board() const {
  std::cout << "|";
  std::cout << std::setfill('-') << std::setw(rank * (2 * rank + 2) - 1) << "";
  std::cout << "|" << std::endl;
  for (int i = 0; i < rank * rank; i++) {
    std::cout << "| ";
    for (int j = 0; j < rank * rank; j++) {
      auto cell = nodes[i * rank * rank + j];
      if (cell.value == 0) {
        std::cout << "  ";
      } else if (rank >= 5) {
        std::cout << (char)('A' + static_cast<int>(cell.value) - 1) << ' ';
      } else if (cell.value <= 9) {
        std::cout << static_cast<int>(cell.value) << ' ';
      } else {
        std::cout << (char)('A' + static_cast<int>(cell.value) - 10) << ' ';
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
