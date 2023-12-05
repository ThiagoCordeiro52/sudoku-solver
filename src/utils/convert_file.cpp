#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<std::vector<int>> readSudoku(const std::string &fileName, int &rank)
{
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    std::cerr << "Error opening the file.\n";
    exit(1);
  }

  std::vector<std::vector<int>> sudoku;
  std::string line;

  while (std::getline(file, line))
  {
    std::vector<int> lineValues;
    int value;
    std::istringstream lineStream(line);
    while (lineStream >> value)
    {
      lineValues.push_back(value);
    }
    sudoku.push_back(lineValues);
  }

  file.close();

  rank = sudoku[0][0];

  return sudoku;
}

std::string generateSudoku(const std::vector<std::vector<int>> &sudoku, int rank)
{
  std::string newSudoku = std::to_string(rank) + "\n";

  for (size_t i = 0; i < sudoku.size(); ++i)
  {
    for (size_t j = 0; j < sudoku[i].size(); ++j)
    {
      if (sudoku[i][j] == 0)
      {
        newSudoku += ".";
      }
      else
      {
        newSudoku += std::to_string(sudoku[i][j]);
      }
    }
  }

  return newSudoku;
}

void writeNewSudoku(const std::string &fileName, const std::string &newSudoku)
{
  std::ofstream file(fileName);
  if (!file.is_open())
  {
    std::cerr << "Error creating the file.\n";
    exit(1);
  }
  file << newSudoku;
  file.close();
}

int main()
{
  int rank;
  std::string inputFileName = "instances/hard/s12c";
  std::string outputFileName = "instances/hard/s12c";

  std::vector<std::vector<int>> sudoku = readSudoku(inputFileName, rank);
  std::string newSudoku = generateSudoku(sudoku, 3);
  writeNewSudoku(outputFileName, newSudoku);

  std::cout << "New file generated successfully: " << outputFileName << std::endl;

  return 0;
}