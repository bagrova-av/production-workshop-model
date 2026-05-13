#pragma once
#ifndef EVENT_H
#define EVENT_H

enum class EventType
{
    FINISH = 0,
    START = 1,
    WAIT = 2,
    READY = 3
};

struct Event
{
    long long time;
    EventType type;
    int productId;
    int machineId;
    int operationType;
    int sizeQueueWait = 0;

    // This element is smaller than the other element if its time is longer
    bool operator<(const Event& other) const
    {
        if (time != other.time)
        {
            return time > other.time;
        }
        return type > other.type; 
    }
};

#endif