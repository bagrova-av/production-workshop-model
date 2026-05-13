#pragma once
#ifndef MACHINE_H
#define MACHINE_H

#include <deque>

struct Machine
{
    int id;
    std::deque<int> queueProductsIds;
    int processingProductId = -1;
    
    bool isFree() const
    {
        return processingProductId == -1;
    }
};

#endif