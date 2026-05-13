#include "Machine.h"

Machine::Machine(int id) :
    id(id)
{ }

void Machine::addToQueue(int productId)
{
    queueProductsIds.push_back(productId);
}

int Machine::startProcessingProduct()
{
    if (queueProductsIds.empty())
    {
        return -1;
    }

    processingProductId = queueProductsIds.front();
    queueProductsIds.pop_front();
    return processingProductId;
}

void Machine::finishProcessingProduct()
{
    processingProductId = -1;
}

bool Machine::isFree() const
{
    return processingProductId == -1;
}

long long Machine::calculateWorkload(const Config& config, 
                                     const std::vector<Product>& allProducts) const
{
    long long totalWaitTime = 0;
    for (int productId : queueProductsIds)
    {
        int productType = allProducts[productId].getCurrentType();
        totalWaitTime += config.getTime(productType, id);
    }
    return totalWaitTime;
}