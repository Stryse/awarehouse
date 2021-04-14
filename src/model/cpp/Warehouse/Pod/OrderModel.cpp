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

int OrderModel::getCategory() const { return category; }

const std::string &OrderModel::getOrderId() const { return orderId; }

bool operator<(const OrderModel &lhs, const OrderModel &rhs)
{
    return lhs.category < rhs.category;
}

int OrderModel::idCounter = 0;