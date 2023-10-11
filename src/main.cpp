#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

#include "fileio.h"
#include "dateparse.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "You should provide a file name as argument";
    return 1;
  }

  auto filename{argv[1]};
  int rank;
  std::string data_str;
  std::vector<Cell> grid{0};

  if (!readFile(filename, rank, data_str))
  {
    std::cerr << "could read the file" << filename << std::endl;
    return 2;
  }

  parseDataString(data_str, rank, grid);

  for (int i = 0; i < rank * rank; i++)
  {
    for (int j = 0; j < rank * rank; j++)
    {
      std::cout << static_cast<int>(grid[i * rank * rank + j]) << ' ';
      if ((j + 1) % rank == 0)
      {
        std::cout << "  "; 
      }
    }
    std::cout << std::endl;
    if ((i + 1) % rank == 0 && (i + 1) < rank * rank)
    {
      std::cout << std::endl;
    }
  }

  return 0;
}
