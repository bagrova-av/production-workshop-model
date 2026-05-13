#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <algorithm>

struct Item
{
    int id;
    int type;
};

struct Config
{
    int M, N;
    std::vector<int> T;
    std::vector<std::vector<int>> initialQueues;
    int get_time(int i, int j) const
    {
        return T[i * N + j];
    }
};

void exitWithError(const std::string& lineWithError)
{
    std::cout << lineWithError << std::endl;
    exit(0);
}

Config parseInput(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        exit(0);
    }

    std::string line;
    Config config;

    if (!std::getline(file, line))
    {
        exit(0);
    }
    std::stringstream ss(line);
    if (!(ss >> config.M >> config.N))
    {
        exitWithError(line);
    }
    
    int unnecessaryInput;
    if (ss >> unnecessaryInput)
    {
        exitWithError(line);
    }

    for (int i = 0; i < config.M - 1; ++i)
    {
        if (!std::getline(file, line))
        {
            exitWithError(line);
        }
        std::stringstream ssTimes(line);
        for (int j = 0; j < config.N; ++j)
        {
            int time;
            if (!(ssTimes >> time))
            {
                exitWithError(line);
            }
            config.T.push_back(time);
        }
        if (ssTimes >> unnecessaryInput)
        {
            exitWithError(line);
        }
    }

    for (int j = 0; j < config.N; ++j)
    {
        if (!std::getline(file, line))
        {
            exitWithError(line);
        }
        std::stringstream ssQueue(line);
        int qSize;
        if (!(ssQueue >> qSize))
        {
            exitWithError(line);
        }
        
        std::vector<int> queue;
        for (int counter = 0; counter < qSize; ++counter)
        {
            int type;
            if (!(ssQueue >> type))
            {
                exitWithError(line);
            }
            if (type < 0 || type > config.M - 2)
            {
                exitWithError(line);
            }
            queue.push_back(type);
        }
        if (ssQueue >> unnecessaryInput)
        {
            exitWithError(line);
        }
        config.initialQueues.push_back(queue);
    }

    return config;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 1;
    }

    Config config = parseInput(argv[1]);

    std::cout << "Successfully parsed M=" << config.M << " N=" << config.N << '\n';

    return 0;
}