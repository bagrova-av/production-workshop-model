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
    std::priority_queue<Event, std::vector<Event>, EventComparator> eventQueue;
    for (int id = 0; id < workshopConfig.countMachines; ++id)
    {
        if (machines[id].getQueueSize() > 0)
        {
            int productId = machines[id].startProcessingProduct();
            eventQueue.push({0, EventType::START,
                            productId, id, products[productId].getCurrentType()});
        }
    }

    int completedProducts = 0;
    TimePoint currentTime = 0;

    while (!eventQueue.empty())
    {
        Event currentEvent = eventQueue.top();
        eventQueue.pop();
        
        currentTime = currentEvent.time;
        ProductId productId = currentEvent.productId;
        MachineId machineId = currentEvent.machineId;
        OperationId operationType = currentEvent.operationType;

        switch (currentEvent.type)
        {
            case EventType::START:
            {
                std::cout << "start " << currentTime << " " << productId << " "
                          << operationType << " " << machineId << "\n";
                
                TimePoint duration = workshopConfig.getTime(operationType, machineId);
                eventQueue.push({currentTime + duration, EventType::FINISH,
                                productId, machineId, operationType});
                break;
            }
            case EventType::FINISH:
            {
                std::cout << "finish " << currentTime << " " << productId << " "
                          << operationType << " " << machineId << "\n";
                machines[machineId].finishProcessingProduct();

                products[productId].setCurrentType(products[productId].getCurrentType() + 1);
                OperationId nextOperationId = products[productId].getCurrentType();

                if (nextOperationId == workshopConfig.countProductsTypes - 1)
                {
                    eventQueue.push({currentTime, EventType::READY,
                                     productId, machineId, nextOperationId});
                }
                else
                {
                    MachineId bestMachineForNextOperation = selectBestMachine(nextOperationId);
                    size_t queueSizeBefore = machines[bestMachineForNextOperation].getQueueSize();
                    
                    eventQueue.push({currentTime, EventType::WAIT,
                                    productId, bestMachineForNextOperation, nextOperationId,
                                    queueSizeBefore});
                    machines[bestMachineForNextOperation].addToQueue(productId);
                    if (machines[bestMachineForNextOperation].isFree())
                    {
                        ProductId nextProductId = machines[bestMachineForNextOperation].startProcessingProduct();
                        eventQueue.push({currentTime, EventType::START,
                                        nextProductId, bestMachineForNextOperation, nextOperationId});
                    }
                }

                if (machines[machineId].isFree() && machines[machineId].getQueueSize() > 0)
                {
                    ProductId nextProductId = machines[machineId].startProcessingProduct();
                    eventQueue.push({currentTime, EventType::START,
                                    nextProductId, machineId, products[nextProductId].getCurrentType()});
                }
                break;
            }
            case EventType::WAIT:
            {
                std::cout << "wait " << currentTime << " " << productId << " "
                          << operationType << " " << machineId << " " << currentEvent.sizeQueueWait << "\n";
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

int Simulator::selectBestMachine(int operationType) const
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