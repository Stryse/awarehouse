#ifndef I_WAREHOUSE_PERSISTENCE__H
#define I_WAREHOUSE_PERSISTENCE__H

class State;

template <typename ResourceType>
class IWarehousePersistence
{
public:
    virtual State* load(ResourceType &resource) = 0;
    virtual void save(const State &state, ResourceType &resource) const = 0;
};

#endif /* I_WAREHOUSE_PERSISTENCE__H */
