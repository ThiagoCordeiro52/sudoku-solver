#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

#include "errors.h"
#include "fileio.h"
#include "graph.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "You should provide a file name as argument";
    return NO_ARGUMENT;
  }

  auto filename{argv[1]};
  std::string data_str;
  number_type rank;

  auto result{readFile(filename, data_str, rank)};
  if (result != 0) {
    return result;
  }

  Graph graph{data_str, rank};

  graph.print_graph();

  std::cout << "---------------------" << std::endl;

  graph.solve();

  graph.print_graph();

  return 0;
}
