#include "Simulator.h"
#include "../domainEntities/Event.h"

#include <iostream>
#include <queue>
#include <vector>

Simulator::Simulator(const Config& config) :
    workshopConfig(config)
{
    int currentId = 0;
    for (int id = 0; id < workshopConfig.N; ++id)
    {
        Machine machine(id);
        
        for (int productType : workshopConfig.initialQueues[id])
        {
            Product newProduct;
            newProduct.setId(currentId);
            newProduct.setCurrentType(productType);
            
            products.push_back(newProduct);
            machine.addToQueue(newProduct.getId());
            
            ++currentId;
        }
        machines.push_back(machine);
    }
    totalProductsCount = products.size();
}

void Simulator::runSimulation()
{
    std::priority_queue<Event> eventQueue;
    for (int id = 0; id < workshopConfig.N; ++id)
    {
        if (machines[id].getQueueSize() > 0)
        {
            int productId = machines[id].startProcessingProduct();
            eventQueue.push({0, EventType::START, productId, id, products[productId].getCurrentType()});
        }
    }

    int completedItems = 0;
    long long currentTime = 0;

    while (!eventQueue.empty())
    {
        Event currentEvent = eventQueue.top();
        eventQueue.pop();
        
        currentTime = currentEvent.time;
        int productId = currentEvent.productId;
        int machineId = currentEvent.machineId;
        int operationType = currentEvent.operationType;

        switch (currentEvent.type)
        {
            case EventType::START:
            {
                std::cout << "start " << currentTime << " " << productId << " "
                          << operationType << " " << machineId << "\n";
                
                long long duration = workshopConfig.getTime(operationType, machineId);
                eventQueue.push({currentTime + duration, EventType::FINISH, productId, machineId,
                                 operationType});
                break;
            }
            case EventType::FINISH:
            {
                std::cout << "finish " << currentTime << " " << productId << " "
                          << operationType << " " << machineId << "\n";
                machines[machineId].finishProcessingProduct();
                // if (machines[machineId].getQueueSize() > 0)
                // {
                //     int nextProductId = machines[machineId].startProcessingProduct();
                //     eventQueue.push({currentTime, EventType::START, nextProductId, machineId,
                //                      products[nextProductId].getCurrentType()});
                // }

                products[productId].setCurrentType(products[productId].getCurrentType() + 1);
                int nextOperationId = products[productId].getCurrentType();

                if (nextOperationId == workshopConfig.M - 1)
                {
                    eventQueue.push({currentTime, EventType::READY, productId, machineId, nextOperationId});
                }
                else
                {
                    int bestMachineForNextOperation = selectBestMachine(nextOperationId);
                    int queueSizeBefore = machines[bestMachineForNextOperation].getQueueSize();
                    
                    eventQueue.push({currentTime, EventType::WAIT, productId,
                                     bestMachineForNextOperation, nextOperationId, queueSizeBefore});
                    machines[bestMachineForNextOperation].addToQueue(productId);
                    if (machines[bestMachineForNextOperation].isFree())
                    {
                        int nextProductId = machines[bestMachineForNextOperation].startProcessingProduct();
                        eventQueue.push({currentTime, EventType::START, nextProductId,
                                         bestMachineForNextOperation, nextOperationId});
                    }
                }

                if (machines[machineId].getQueueSize() > 0)
                {
                    int nextProductId = machines[machineId].startProcessingProduct();
                    eventQueue.push({currentTime, EventType::START, nextProductId, machineId,
                                     products[nextProductId].getCurrentType()});
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
                ++completedItems;
                if (completedItems == totalProductsCount)
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
    long long minWaitTime = -1;
    int bestMachineIndex = -1;

    for (int id = 0; id < workshopConfig.N; ++id)
    {
        long long currentMachineWaitTime = machines[id].calculateWorkload(workshopConfig, products);
        if (bestMachineIndex == -1 || currentMachineWaitTime < minWaitTime)
        {
            minWaitTime = currentMachineWaitTime;
            bestMachineIndex = id;
        }
        else if (currentMachineWaitTime == minWaitTime)
        {
            if (id < bestMachineIndex)
            {
                bestMachineIndex = id;
            }
        }
    }
    return bestMachineIndex;
}