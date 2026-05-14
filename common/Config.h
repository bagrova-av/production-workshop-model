#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>

#include "types.h"

struct Config
{
    int countProductsTypes;
    int countMachines;
    std::vector<TimePoint> operationsTimes;
    std::vector<std::vector<OperationId>> initialQueues;

    TimePoint getTime(OperationId operationType, MachineId machineId) const;
};

inline TimePoint Config::getTime(OperationId operationType, MachineId machineId) const
{
    return operationsTimes[operationType * countMachines + machineId];
}

#endif