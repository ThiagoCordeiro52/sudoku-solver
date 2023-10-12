#include "fileio.h"
#include "graph.h"

int readFile(const std::string &filename, std::string &data_str) {
  std::ifstream file{filename};

  if (file.fail()) {
    std::cerr << "could not open file";
    return 2;
  }

  if (not getline(file >> std::ws, data_str)) {
    std::cerr << "could not read data from file";
    return 3;
  }

  auto n_cells = std::pow(RANK, 4);

  if (data_str.size() != n_cells) {
    std::cerr << "there should be " << n_cells
              << " characters in the first line";
    return 4;
  }

  return 0;
}
