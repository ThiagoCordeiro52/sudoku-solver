#ifndef GENETIC_H
#define GENETIC_H

#include <string>
#include <utility>
#include <vector>

using number_type = unsigned int;
using cell_type = unsigned int;

class SudokuBoard {
public:
  SudokuBoard(){};
  SudokuBoard(std::string data_str, number_type rank);
  void print_board() const;
  void genetic_algorithm(double mutation_rate, int max_generations);
  int fitness() const;

private:
  static std::pair<SudokuBoard, SudokuBoard>
  crossover(const SudokuBoard &first, const SudokuBoard &second);
  void mutate(SudokuBoard &individual, double mutation_rate) const;
  bool is_valid(number_type row, number_type col, cell_type number) const;
  SudokuBoard generate();

  std::vector<cell_type> nodes;
  number_type rank;
};

#endif
