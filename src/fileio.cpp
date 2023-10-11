#include "fileio.h"

bool readFile(const std::string &filename, int &rank, std::string &data_str)
{
    std::ifstream file{filename};

    if (file.fail())
    {
        std::cerr << "could not open file";
        return 2;
    }

    std::string n_str;

    if (not getline(file >> std::ws, n_str))
    {
        std::cerr << "could not read the value of n from file";
        return 3;
    }

    try
    {
        rank = static_cast<int>(std::stoul(n_str));
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "the rank provided in the file is not an integer";
        return 4;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "the rank provided cannot be stored in a int";
        return 5;
    }

    if (not getline(file >> std::ws, data_str))
    {
        std::cerr << "could not read data from file";
        return 6;
    }

    auto n_cells = std::pow(rank, 4);

    if (data_str.size() != n_cells)
    {
        std::cerr << "there should be " << n_cells
                  << " characters in second line";
        return 7;
    }

    return true;
}