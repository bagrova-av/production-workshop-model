#include "Simulator.h"
#include "../domainEntities/Event.h"

#include <iostream>
#include <queue>
#include <vector>
#include <limits>

Simulator::Simulator(const Config& config) :
    workshopConfig(config)
{
    int currentProductId = 0;
    MachineId machineId = 0;
    for (int id = 0; id < workshopConfig.countMachines; ++id)
    {
        machineId = static_cast<MachineId>(id);
        Machine machine(machineId);
        initializeProducts(machine, machineId, currentProductId);
        machines.push_back(machine);
    }
    totalProductsCount = products.size();
}

void Simulator::initializeProducts(Machine& machine, MachineId machineId,
                                   ProductId& currentProductId)
{
    for (OperationId productType : workshopConfig.initialQueues[machineId])
    {
        Product newProduct;
        newProduct.setId(currentProductId);
        newProduct.setCurrentType(productType);
        
        products.push_back(newProduct);
        machine.addToQueue(newProduct.getId());
        
        ++currentProductId;
    }
}

void Simulator::runSimulation()
{
    int completedProducts = 0;
    TimePoint currentTime = 0;

    if (totalProductsCount == 0)
    {
        std::cout << "stop " << currentTime << "\n";
        return;
    }

    std::priority_queue<Event, std::vector<Event>, EventComparator> eventQueue;
    for (MachineId machineId = 0; machineId < workshopConfig.countMachines; ++machineId)
    {
        tryStartMachine(machineId, 0, eventQueue);
    }

    while (!eventQueue.empty())
    {
        Event currentEvent = eventQueue.top();
        eventQueue.pop();
        
        currentTime = currentEvent.time;
        ProductId productId = currentEvent.productId;
        MachineId machineId = currentEvent.machineId;
        OperationId operationId = currentEvent.operationId;

        switch (currentEvent.type)
        {
            case EventType::START:
            {
                std::cout << "start " << currentTime << " " << productId << " "
                          << operationId << " " << machineId << "\n";
                
                TimePoint duration = workshopConfig.getTime(operationId, machineId);
                eventQueue.push({currentTime + duration, EventType::FINISH,
                                productId, machineId, operationId});
                break;
            }
            case EventType::FINISH:
            {
                std::cout << "finish " << currentTime << " " << productId << " "
                          << operationId << " " << machineId << "\n";
                machines[machineId].finishProcessingProduct();

                // Further cycle of the processed product
                products[productId].setCurrentType(products[productId].getCurrentType() + 1);
                OperationId nextOperation = products[productId].getCurrentType();

                if (nextOperation == (workshopConfig.countProductsTypes - 1))
                {
                    eventQueue.push({currentTime, EventType::READY,
                                    productId, machineId, nextOperation});
                }
                else
                {
                    MachineId nextMachineId = selectBestMachine(nextOperation);
                    size_t queueSizeBefore = machines[nextMachineId].getQueueSize();
                    
                    machines[nextMachineId].addToQueue(productId);
                    eventQueue.push({currentTime, EventType::WAIT,
                                    productId, nextMachineId, nextOperation, queueSizeBefore});

                    // Maybe can start immediately
                    tryStartMachine(nextMachineId, currentTime, eventQueue);
                }

                // Current machine may process next product
                tryStartMachine(machineId, currentTime, eventQueue);
                break;
            }
            case EventType::WAIT:
            {
                std::cout << "wait " << currentTime << " " << productId << " "
                          << operationId << " " << machineId << " " << currentEvent.sizeQueueWait << "\n";
                break;
            }
            case EventType::READY:
            {
                std::cout << "ready " << currentTime << " " << productId << " " << machineId << "\n";
                ++completedProducts;
                if (completedProducts == totalProductsCount)
                {
                    std::cout << "stop " << currentTime << "\n";
                    return;
                }
                break;
            }
        }
    }
}

int Simulator::selectBestMachine(int operationId) const
{
    TimePoint minWaitTime = std::numeric_limits<TimePoint>::max();
    int bestMachineIndex = -1;

    for (int id = 0; id < workshopConfig.countMachines; ++id)
    {
        TimePoint currentMachineWaitTime = machines[id].calculateWorkload(workshopConfig, products);
        if (bestMachineIndex == -1 || currentMachineWaitTime < minWaitTime)
        {
            minWaitTime = currentMachineWaitTime;
            bestMachineIndex = static_cast<MachineId>(id);
        }
        else if (currentMachineWaitTime == minWaitTime)
        {
            if (id < bestMachineIndex)
            {
                bestMachineIndex = static_cast<MachineId>(id);
            }
        }
    }
    return bestMachineIndex;
}

void Simulator::tryStartMachine(MachineId machineId, TimePoint currentTime,
                                std::priority_queue<Event, std::vector<Event>, EventComparator>& eventQueue)
{
    if (!machines[machineId].isFree())
    {
        return;
    }

    if (machines[machineId].getQueueSize() == 0)
    {
        return;
    }

    ProductId productId = machines[machineId].startProcessingProduct();

    eventQueue.push({currentTime, EventType::START, productId, machineId, products[productId].getCurrentType()});
}