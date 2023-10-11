#ifndef DATAPARSE_H
#define DATAPARSE_H

#include <string>
#include <vector>
#include "fileio.h" 

enum Cell
{
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

void parseDataString(const std::string& data_str, int rank, std::vector<Cell> & grid);

#endif