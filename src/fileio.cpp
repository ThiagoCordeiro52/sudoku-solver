#include "fileio.h"
#include "errors.h"
#include "graph.h"
#include <fstream>

int readFile(std::istream *input_stream, std::string &data_str,
             number_type &rank) {
  if (input_stream->fail()) {
    std::cerr << "could not open file";
    return COULD_NOT_OPEN;
  }

  std::string rank_str;

  if (not getline(*input_stream >> std::ws, rank_str)) {
    std::cerr << "could not read rank from file";
    return MISSING_RANK;
  }

  try {
    rank = static_cast<int>(std::stoul(rank_str));
  } catch (const std::invalid_argument &e) {
    std::cerr << "the rank provided in the file is not an integer";
    return RANK_NOT_INT;
  } catch (const std::out_of_range &e) {
    std::cerr << "the rank provided cannot be stored in a int";
    return RANK_OUT_OF_RANGE;
  }

  if (not getline(*input_stream >> std::ws, data_str)) {
    std::cerr << "could not read data from file";
    return MISSING_DATA;
  }

  auto n_cells = std::pow(rank, 4);

  if (data_str.size() != n_cells) {
    std::cerr << "there should be " << n_cells
              << " characters in the first line";
    return DATA_TOO_SMALL;
  }

  return 0;
}
