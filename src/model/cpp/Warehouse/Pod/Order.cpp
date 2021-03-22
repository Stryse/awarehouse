#include "Order.h"

Order::Order(int category)
    : category(category), orderId(makeId(category))
{
}

std::string Order::makeId(int category)
{
    return std::string("[ORDER]{Category: ") +
           std::to_string(category) +
           std::string("}_ID_") +
           std::to_string(idCounter++);
}

// bool operator<(const Order &lhs, const Order &rhs)
// {
//     return lhs.category < rhs.category;
// }

int Order::idCounter = 0;