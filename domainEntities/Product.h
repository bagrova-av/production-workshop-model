#pragma once
#ifndef PRODUCT_H
#define PRODUCT_H

#include "../common/types.h"

class Product
{
public:
    ProductId getId() const;
    OperationId getCurrentType() const;

    void setId(ProductId newId);
    void setCurrentType(OperationId newType);

private:
    ProductId id;
    OperationId currentType;
};

inline ProductId Product::getId() const
{
    return id;
}

inline OperationId Product::getCurrentType() const
{
    return currentType;
}

#endif