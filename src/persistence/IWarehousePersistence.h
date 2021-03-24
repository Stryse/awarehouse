#ifndef I_WAREHOUSE_PERSISTENCE__H
#define I_WAREHOUSE_PERSISTENCE__H

class State;

template <typename ResourceType>
class IWarehousePersistence
{
public:
    virtual State* load(const ResourceType &resource) = 0;
    virtual bool save(const State &state, const ResourceType &resource) const = 0;
};

#endif /* I_WAREHOUSE_PERSISTENCE__H */
