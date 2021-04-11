#ifndef ORDER_MODEL__H
#define ORDER_MODEL__H

#include <string>

class OrderModel
{
public:
    explicit OrderModel(int category);

    friend inline bool operator<(const OrderModel &lhs, const OrderModel &rhs)
    {
        return lhs.category < rhs.category;
    }

private:
    int category;
    static int idCounter;
    std::string orderId;
    static std::string makeId(int category);
};

#endif /* ORDER_MODEL__H */
