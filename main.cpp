#include <vector>
#include <string>
#include <algorithm>

#include "./parsing/InputParser.h"
#include "./simulation/Simulator.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 1;
    }

    Config config = InputParser::parseFile(argv[1]);
    Simulator simulator(config);
    simulator.runSimulation();

    return 0;
}