#pragma once
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../Parsing/InputParser.h"
#include "../DomainEntities/Machine.h"
#include "../DomainEntities/Product.h"

class Simulator
{
public:
    explicit Simulator(const Config& config);

    void runSimulation();

private:
    int selectBestMachine(int operationType) const;
    void initializeProducts();

private:
    Config workshopConfig;
    std::vector<Machine> machines;
    std::vector<Product> products;
    int totalProductsCount = 0;
};

#endif