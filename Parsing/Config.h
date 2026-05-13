#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>

struct Config
{
    int M;
    int N;
    std::vector<int> operationsTimes;
    std::vector<std::vector<int>> initialQueues;

    int getTime(int operationType, int machineId) const;

    static Config loadFromFile(const std::string& fileName);
};

inline int Config::getTime(int operationType, int machineId) const
{
    return operationsTimes[operationType * N + machineId];
}

#endif