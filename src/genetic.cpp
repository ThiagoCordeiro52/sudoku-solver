#include "include/genetic.h"
#include "include/errors.h"
#include <iomanip>
#include <iostream>

const int POPULATION_SIZE = 100;

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
      if (number <= 0 && number > rank * rank) {
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
  
std::pair<SudokuBoard, SudokuBoard> SudokuBoard::crossover(const SudokuBoard &first, const  SudokuBoard &second){
  auto size = first.rank * first.rank;
  auto first_child = SudokuBoard{new std::vector(size * size), first.rank};
  auto second_child = SudokuBoard{new std::vector(size * size), first.rank};
  int crossoverPoint = rand() % (size - 1) + 1;

  for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
          auto index = i * size * size + j;
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
  void SudokuBoard::mutate(SudokuBoard& individual, double mutation_rate) const {
    auto size = rank * rank;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            auto index = i * size * size + row;
            if (nodes[index] != 0 && (rand() % 100) < mutation_rate * 100) {
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
            int num = nodes[row][col];
            if (num != 0) {
                for (int i = 0; i < size; ++i) {
                    conflicts += (num == nodes[row][i] || num == nodes[i][col] ||
                                   num == nodes[row / size + i / rank][col / size + i % rank]);
                }
            }
        }
    }
    return conflicts;
  };

  void SudokuBoard::genetic_algorithm(double mutation_rate, int max_generations){
    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<SudokuBoard> population(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        population[i] = generate_from();
    }

    for (int generation = 0; generation < max_generations; ++generation) {
        std::sort(population.begin(), population.end(), [](const SudokuBoard& a, const SudokuBoard& b) {
            return a.fitness() < b.fitness();
        });

        if (population[0].fitness() == 0) {
            cout << "Solution found in generation " << generation + 1 << endl;
            return population[0];
        }

        std::vector<SudokuBoard> next_generation;
        for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
            int parent1_index = rand() % (POPULATION_SIZE / 2);
            int parent2_index = rand() % (POPULATION_SIZE / 2);
            auto children = crossover(population[parent1_index], population[parent2_index]);
            next_generation.push_back(mutate(children.first, mutationRate));
            next_generation.push_back(mutate(children.second, mutationRate));
        }

        population = next_generation;
    }

    std::cout << "No solution found." << std::endl;
    return SudokuBoard{};
  };

  bool SudokuBoard::is_valid(number_type row, number_type col, cell_type number) const {
    auto size = rank * rank;
    // check lines and cols
    for (int i = 0; i < size; ++i) {
      if (nodes[row * size * size + i] == num || nodes[i * size * size + col] == num) {
          return false;
      }
    }

    // check subgrid
    int start_row = row - row % rank;
    int start_col = col - col % rank;
    for (int i = 0; i < rank; ++i) {
      for (int j = 0; j < rank; ++j) {
          auto index = (start_row + i) * size * size + (start_col + j);
          if (nodes.nodes[index] == num) {
              return false;
          }
      }
    }

    return true;
  };

  SudokuBoard SudokuBoard::generate_from() const{
    auto size = rank * rank;
    SudokuBoard individual;
    individual.rank = rank;
    individual.nodes = new std::vector(nodes);
    for (int i = 1; i <= size; ++i) {
        for (int j = 0; j < size; ++j) {
            int row, col;
            auto index = row * size * size + col;
            do {
                row = rand() % size;
                col = rand() % size;
            } while (individual.nodes[index]);

            individual.nodes[row][col] = i;
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
      auto cell = nodes[i * rank * rank * rank * rank + j];
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
