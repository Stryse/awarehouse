#ifndef I_CONTAINING__H
#define I_CONTAINING__H

#include <optional>

/*****************************************************************
 * @brief An interface of an entity which can contain Items.
 * @tparam ItemType type of the item which the entity contains.
 * 
 * Contained item can be placed and taken through this interface.
 *****************************************************************/
template <typename ItemType>
class IContaining
{
public:

    /********************************************************
     * @brief Places the item inside the entity's inventory.
     ********************************************************/
    virtual void push(const ItemType &item) = 0;
    virtual void push(ItemType &&item) = 0;

    /*********************************************************
     * @brief Takes the item from the entity's inventory.
     ********************************************************/
    virtual std::optional<ItemType> pop(const ItemType &item) = 0;

    /*************************************************************
     * @brief Returns the whether the entity has at least one item
     * in its inventory.
     *************************************************************/
    virtual bool empty() const = 0;

public:
    virtual ~IContaining() = default;
};

#endif