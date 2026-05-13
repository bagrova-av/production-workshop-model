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
        Machine machine;
        machine.id = id;
        
        for (int productType : workshopConfig.initialQueues[id])
        {
            Product newProduct;
            newProduct.setId(currentId);
            newProduct.setCurrentType(productType);
            
            products.push_back(newProduct);
            machine.queueProductsIds.push_back(newProduct.getId());
            
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
        if (!machines[id].queueProductsIds.empty())
        {
            int productId = machines[id].queueProductsIds.front();
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
                std::cout << "start " << currentTime << " " << productId << " " << operationType << " " << machineId << "\n";
                machines[machineId].processingProductId = productId;
                machines[machineId].queueProductsIds.pop_front();
                
                long long duration = workshopConfig.getTime(operationType, machineId);
                eventQueue.push({currentTime + duration, EventType::FINISH, productId, machineId, operationType});
                break;
            }
            case EventType::FINISH:
            {
                std::cout << "finish " << currentTime << " " << productId << " " << operationType << " " << machineId << "\n";
                machines[machineId].processingProductId = -1;
                if (!machines[machineId].queueProductsIds.empty())
                {
                    int nextProductId = machines[machineId].queueProductsIds.front();
                    eventQueue.push({currentTime, EventType::START, nextProductId, machineId, products[nextProductId].getCurrentType()});
                }

                products[productId].setCurrentType(products[productId].getCurrentType() + 1);
                int nextOperationId = products[productId].getCurrentType();

                if (nextOperationId == workshopConfig.M - 1)
                {
                    eventQueue.push({currentTime, EventType::READY, productId, machineId, nextOperationId});
                }
                else
                {
                    int bestMachineForNextOperation = selectBestMachine(nextOperationId);
                    int currentQueueSize = machines[bestMachineForNextOperation].queueProductsIds.size();
                    
                    eventQueue.push({currentTime, EventType::WAIT, productId, bestMachineForNextOperation, nextOperationId, currentQueueSize});
                    machines[bestMachineForNextOperation].queueProductsIds.push_back(productId);
                    if (machines[bestMachineForNextOperation].isFree())
                    {
                        eventQueue.push({currentTime, EventType::START, productId, bestMachineForNextOperation, nextOperationId});
                    }
                }
                break;
            }
            case EventType::WAIT:
            {
                std::cout << "wait " << currentTime << " " << productId << " " << operationType << " " << machineId << " " << currentEvent.sizeQueueWait << "\n";
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
        long long currentMachineWaitTime = 0;
        for (int productId : machines[id].queueProductsIds)
        {
            int productType = products[productId].getCurrentType();
            currentMachineWaitTime += workshopConfig.getTime(productType, id);
        }

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