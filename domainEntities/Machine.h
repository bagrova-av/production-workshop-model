#pragma once
#ifndef MACHINE_H
#define MACHINE_H

#include <deque>

#include "../parsing/Config.h"
#include "Product.h"

class Machine
{
public:
    explicit Machine(int id);

    void addToQueue(int productId);
    int startProcessingProduct();
    void finishProcessingProduct();

    bool isFree() const;
    int getId() const;
    int getProcessingProductId() const;
    size_t getQueueSize() const;

    long long calculateWorkload(const Config& config,
                                const std::vector<Product>& allProducts) const;

private:
    int id;
    std::deque<int> queueProductsIds;
    int processingProductId = -1;
};

inline int Machine::getId() const
{
    return id;
}
inline int Machine::getProcessingProductId() const
{
    return processingProductId;
}

inline size_t Machine::getQueueSize() const
{
    return queueProductsIds.size();
}

#endif