#pragma once
#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <vector>
#include <string>

struct Config
{
    int M;
    int N;
    std::vector<int> operationsTimes;
    std::vector<std::vector<int>> initialQueues;

    int getTime(int operationId, int machineId) const;

    static Config loadFromFile(const std::string& fileName);
};

inline int Config::getTime(int operationId, int machineId) const
{
    return operationsTimes[operationId * N + machineId];
}

#endif