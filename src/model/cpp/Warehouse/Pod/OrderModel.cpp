#include "OrderModel.h"

OrderModel::OrderModel(int category)
    : category(category), orderId(makeId(category))
{
}

std::string OrderModel::makeId(int category)
{
    return std::string("[ORDER]{Category: ") +
           std::to_string(category) +
           std::string("}_ID_") +
           std::to_string(idCounter++);
}

int OrderModel::idCounter = 0;