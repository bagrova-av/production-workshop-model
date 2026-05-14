#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>

struct Config
{
    int countMachines;
    int countProductsTypes;
    std::vector<int> operationsTimes;
    std::vector<std::vector<int>> initialQueues;

    int getTime(int operationType, int machineId) const;
    int getCountMachines() const;
    int getCountProductsTypes() const;
};

inline int Config::getTime(int operationType, int machineId) const
{
    return operationsTimes[operationType * countProductsTypes + machineId];
}

#endif