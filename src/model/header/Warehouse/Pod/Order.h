#ifndef ORDER__H
#define ORDER__H

#include <string>

class Order
{
public:
    Order(int category);

    friend inline bool operator<(const Order &lhs, const Order &rhs)
    {
        return lhs.category < rhs.category;
    }

private:
    int category;
    static int idCounter;
    std::string orderId;
    static std::string makeId(int category);
};

#endif /* ORDER__H */