#pragma once
#ifndef VALIDATION_UTILS_H
#define VALIDATION_UTILS_H

#include "Config.h"

bool checkCorrectNameFile(const std::string& fileName);

bool validateCountProductsTypes(int countProductsTypes);
bool validateCountMachines(int countMachines);
bool validateOperationTime(TimePoint operationTime);
bool validateProductType(OperationId productType, const Config& config);
bool validateTotalCountProducts(int countProducts);

#endif