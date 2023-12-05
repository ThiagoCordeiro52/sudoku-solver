#ifndef GENETIC_H
#define GENETIC_H

#include <string>
#include <utility>
#include <vector>

using number_type = unsigned int;
using cell_type = unsigned int;

class SudokuBoard {
public:
  struct Node {
    cell_type value;
    std::vector<bool> adjacent;
    number_type i;
    number_type j;

    cell_type get_random_free(number_type n_cells) const {
      std::vector<cell_type> free;
      for (number_type i{0}; i < n_cells; i++) {
        if (!adjacent.at(i)) {
          free.push_back(i + 1);
        }
      }

      return free.size() == 0 ? rand() % n_cells + 1
                              : free[rand() % free.size()];
    };
  };
  SudokuBoard(){};
  SudokuBoard(std::string data_str, number_type rank);
  void print_board() const;
  void genetic_algorithm(double mutation_rate, int max_generations);
  int fitness() const;
  Node *get_node(number_type i, number_type j);

private:
  void mark_adjacent_nodes();
  static std::pair<SudokuBoard, SudokuBoard>
  crossover(const SudokuBoard &first, const SudokuBoard &second);
  void mutate(SudokuBoard &individual, double mutation_rate) const;
  bool is_valid(number_type row, number_type col, cell_type number) const;
  SudokuBoard generate();

  std::vector<Node> nodes;
  number_type rank;
};

#endif
