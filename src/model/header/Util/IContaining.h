#ifndef I_CONTAINING__H
#define I_CONTAINING__H

#include <optional>

template <typename ItemType>
class IContaining
{
public:
    virtual void push(const ItemType &item) = 0;
    virtual void push(ItemType &&item) = 0;
    virtual std::optional<ItemType> pop(const ItemType &item) = 0;
    virtual bool empty() const = 0;
};

#endif