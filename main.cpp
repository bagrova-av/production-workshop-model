#include <iostream>

#include <vector>
#include <string>
#include <algorithm>

#include "Parsing/InputParser.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 1;
    }

    Config config = Config::loadFromFile(argv[1]);

    std::cout << "Successfully parsed M=" << config.M << " N=" << config.N << '\n';

    return 0;
}