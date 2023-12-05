#include "include/genetic.h"
#include "include/errors.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

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
    nodes.push_back(cell);
  }
}

std::pair<SudokuBoard, SudokuBoard>
SudokuBoard::crossover(const SudokuBoard &first, const SudokuBoard &second) {
  auto size = first.rank * first.rank;
  SudokuBoard first_child;
  SudokuBoard second_child;
  first_child.nodes = second_child.nodes = std::vector<cell_type>(size * size);
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
    for (int j = 0; j < size; ++j) {
      auto index = i * size + j;
      if (nodes[index] == 0 && (rand() % 100) < mutation_rate * 100) {
        individual.nodes[index] = rand() % size + 1;
      }
    }
  }
};

int SudokuBoard::fitness() const {
  auto size = rank * rank;
  int conflicts = 0;
  for (int row = 0; row < size; ++row) {
    for (int col = 0; col < size; ++col) {
      int num = nodes[row * size + col];
      if (num != 0) {
        for (int i = 0; i < size; ++i) {
          if (num == nodes[row * size + i]) {
            // conflict in row
            conflicts += 1;
          } else if (num == nodes[i * size + col]) {
            // conflict in col
            conflicts += 1;
          } else {
            auto checkRow = (row / rank) * rank + (i / rank);
            auto checkCol = (col / rank) * rank + (i % rank);
            if (checkRow == row && checkCol == col) {
              continue;
            } else if (num == nodes[checkRow * size + checkCol]) {
              // conflict in block
              conflicts += 1;
              conflicts += 1;
            }
          }
        }
      }
    }
  }
  return conflicts;
};

void SudokuBoard::genetic_algorithm(double mutation_rate, int max_generations) {
  srand(static_cast<unsigned>(time(nullptr)));

  std::vector<SudokuBoard> population(POPULATION_SIZE);
  population[0] = *this;
  for (int i = 1; i < POPULATION_SIZE; ++i) {
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

  std::sort(population.begin(), population.end(), compare_fitness);

  this->nodes = population[0].nodes;
};

bool SudokuBoard::is_valid(number_type row, number_type col,
                           cell_type num) const {
  auto size = rank * rank;
  // check lines and cols
  for (int i = 0; i < size; ++i) {
    if (nodes[row * size * size + i] == num ||
        nodes[i * size * size + col] == num) {
      return false;
    }
  }

  // check subgrid
  int start_row = row - row % rank;
  int start_col = col - col % rank;
  for (int i = 0; i < rank; ++i) {
    for (int j = 0; j < rank; ++j) {
      auto index = (start_row + i) * size * size + (start_col + j);
      if (nodes[index] == num) {
        return false;
      }
    }
  }

  return true;
};

SudokuBoard SudokuBoard::generate() {
  auto size = rank * rank;
  SudokuBoard individual;
  individual.rank = rank;
  individual.nodes.resize(size * size, 0);
  for (auto i = 1; i <= size; ++i) {
    for (auto j = 0; j < size; ++j) {
      int index;
      do {
        index = rand() % (size * size);
      } while (individual.nodes[index] != 0);

      individual.nodes[index] = i;
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
      if (cell == 0) {
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
