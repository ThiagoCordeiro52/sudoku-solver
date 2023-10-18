#ifndef FILEIO_H
#define FILEIO_H

#include "graph.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int readFile(std::istream *input_stream, std::string &data_str,
             number_type &rank);

#endif
