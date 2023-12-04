#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>

#include "errors.h"
#include "genetic.h"
#include "fileio.h"
#include "graph.h"

int main(int argc, char *argv[]) {
  std::ifstream file;
  std::istream *input_stream;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "could not open file";
      return COULD_NOT_OPEN;
    }
    input_stream = &file;
  } else {
    input_stream = &std::cin;
  }

  std::string data_str;
  number_type rank;

  auto result{readFile(input_stream, data_str, rank)};
  if (result != 0) {
    return result;
  }

  SudokuBoard board{data_str, rank};

  board.print_board();

  std::cout << std::endl;

  board.genetic_algorithm(0.1, 1000);

  board.print_board();

  /* std::cout << "How close is the solution from the best? " */
  /*           << graph.largest_value - rank * rank << std::endl; */

  return 0;
}
