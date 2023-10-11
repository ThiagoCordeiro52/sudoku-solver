#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

bool readFile(const std::string& filename, int& rank, std::string& data_str);

#endif