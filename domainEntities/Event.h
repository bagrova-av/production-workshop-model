#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "../common/types.h"

enum class EventType
{
    FINISH = 0,
    START = 1,
    WAIT = 2,
    READY = 3
};

struct Event
{
    TimePoint time;
    EventType type;
    ProductId productId;
    MachineId machineId;
    OperationId operationId;
    size_t sizeQueueWait = 0;

    Event(TimePoint time, EventType type, ProductId productId, MachineId machineId,
          OperationId operationId, size_t sizeQueueWait = 0) :
        time(time),
        type(type),
        productId(productId),
        machineId(machineId),
        operationId(operationId),
        sizeQueueWait(sizeQueueWait)
    {}
};

struct EventComparator
{
    // The element is smaller than the other element if its time is longer
    bool operator()(const Event& lhs, const Event& rhs) const
    {
        if (lhs.time != rhs.time)
        {
            return lhs.time > rhs.time;
        }
        return static_cast<int>(lhs.type) > static_cast<int>(rhs.type);
    }
};

#endif