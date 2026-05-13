#pragma once
#ifndef PRODUCT_H
#define PRODUCT_H

class Product
{
public:
    int getId() const;
    int getCurrentType() const;

    void setId(int newId);
    void setCurrentType(int newType);

private:
    int id;
    int currentType;
};

inline int Product::getId() const
{
    return id;
}

inline int Product::getCurrentType() const
{
    return currentType;
}

#endif