#include "validationUtils.h"
#include "../common/constants.h"

bool validateCountProductsTypes(int countProductsTypes)
{
    if (countProductsTypes < 1 || countProductsTypes > MAX_COUNT_PRODUCT_TYPES)
    {
        return false;
    }
    return true;
}

bool validateCountMachines(int countMachines)
{
    if (countMachines < 1 || countMachines > MAX_COUNT_MACHINES)
    {
        return false;
    }
    return true;
}

bool validateOperationTime(TimePoint operationTime)
{
    if (operationTime < 0 || operationTime > MAX_OPERATION_TIME)
    {
        return false;
    }
    return true;
}

bool validateProductType(OperationId productType, const Config& config)
{
    if (productType < 0 || productType > config.countProductsTypes - 2)
    {
        return false;
    }
    return true;
}

bool validateTotalCountProducts(int countProducts)
{
    if (countProducts > MAX_COUNT_PRODUCTS_IN_WORKSHOP)
    {
        return false;
    }
    return true;
}