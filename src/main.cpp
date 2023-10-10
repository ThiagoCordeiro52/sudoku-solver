#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

enum Cell {
  DOT,
  NUMBER1,
  NUMBER2,
  NUMBER3,
  NUMBER4,
  NUMBER5,
  NUMBER6,
  NUMBER7,
  NUMBER8,
  NUMBER9,
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "You should provide a file name as argument";
    return 1;
  }
  auto filename{argv[1]};

  std::ifstream file{filename};

  while (not file.eof()) {
    if (file.fail()) {
      std::cerr << "could not open file";
      return 2;
    }

    int rank;
    std::string n_str;

    if (not getline(file >> std::ws, n_str)) {
      std::cerr << "could not read the value of n from file";
      return 3;
    }

    try {
      rank = static_cast<int>(std::stoul(n_str));
    } catch (const std::invalid_argument &e) {
      std::cerr << "the rank provided in the file is not an integer";
      return 4;
    } catch (const std::out_of_range &e) {
      std::cerr << "the rank provided cannot be stored in a int";
      return 5;
    }

    std::string data_str;

    if (not getline(file >> std::ws, data_str)) {
      std::cerr << "could not read data from file";
      return 6;
    }

    auto n_cells = std::pow(rank, 4);
    if (data_str.size() != n_cells) {
      std::cerr << "there should be " << n_cells
                << " characters in second line";
      return 7;
    }

    std::vector<Cell> grid{0};

    std::transform(data_str.begin(), data_str.end(), std::back_inserter(grid),
                   [rank](auto letter) {
                     if (letter == '.') {
                       return DOT;
                     } else {
                       auto number = letter - '0';

                       if (number <= 0 or number > 9) {
                         std::cerr << "each char in second line must be either "
                                      "a dot or a number from 0 to "
                                   << rank * rank;
                         exit(8);
                       }

                       return (Cell)number;
                     }
                   });

    for (auto value : grid) {
      std::cout << value << ',';
    }
    return 0;
  }
}
