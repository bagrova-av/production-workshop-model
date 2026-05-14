#pragma once
#ifndef MACHINE_H
#define MACHINE_H

#include <deque>

#include "../common/Config.h"
#include "Product.h"

class Machine
{
public:
    explicit Machine(int id);

    void addToQueue(int productId);
    ProductId startProcessingProduct();
    void finishProcessingProduct();

    bool isFree() const;
    MachineId getId() const;
    ProductId getProcessingProductId() const;
    size_t getQueueSize() const;

    TimePoint calculateWorkload(const Config& config,
                                const std::vector<Product>& allProducts) const;

private:
    MachineId id;
    std::deque<ProductId> queueProductsIds;
    ProductId processingProductId = -1;
};

inline MachineId Machine::getId() const
{
    return id;
}
inline ProductId Machine::getProcessingProductId() const
{
    return processingProductId;
}

inline size_t Machine::getQueueSize() const
{
    return queueProductsIds.size();
}

#endif