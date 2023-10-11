#include "dateparse.h"
#include <algorithm>

void parseDataString(const std::string &data_str, int rank, std::vector<Cell> & grid)
{
    std::transform(data_str.begin(), data_str.end(), std::back_inserter(grid),
                   [rank](auto letter)
                   {
                       if (letter == '.')
                       {
                           return DOT;
                       }
                       else
                       {
                           auto number = letter - '0';

                           if (number <= 0 or number > 9)
                           {
                               std::cerr << "each char in second line must be either "
                                            "a dot or a number from 0 to "
                                         << rank * rank;
                               exit(8);
                           }

                           return (Cell)number;
                       }
                   });

}