#pragma once
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <queue>

#include "../parsing/InputParser.h"
#include "../domainEntities/Machine.h"
#include "../domainEntities/Product.h"
#include "../domainEntities/Event.h"

class Simulator
{
public:
    explicit Simulator(const Config& config);

    void runSimulation();

private:
    int selectBestMachine(OperationId operationId) const;
    void initializeProducts(Machine& machine, MachineId machineId,
                            ProductId& currentProductId);
    void tryStartMachine(MachineId machineId, TimePoint currentTime,
                        std::priority_queue<Event, std::vector<Event>, EventComparator>& eventQueue);

private:
    Config workshopConfig;
    std::vector<Machine> machines;
    std::vector<Product> products;
    int totalProductsCount = 0;
};

#endif