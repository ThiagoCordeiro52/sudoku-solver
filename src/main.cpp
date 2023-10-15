#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

#include "fileio.h"
#include "graph.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "You should provide a file name as argument";
    return 1;
  }

  auto filename{argv[1]};
  std::string data_str;

  auto result{readFile(filename, data_str)};
  if (result != 0) {
    return result;
  }

  Graph graph{data_str};

  graph.print_graph();

  std::cout << "---------------------" << std::endl;

  graph.solve();

  graph.print_graph();

  return 0;
}
