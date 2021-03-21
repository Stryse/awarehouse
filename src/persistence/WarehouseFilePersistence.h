#ifndef WAREHOUSE_FILE_PERSISTENCE__H
#define WAREHOUSE_FILE_PERSISTENCE__H

#include "IWarehousePersistence.h"
#include <stdexcept>
#include <string>

class WarehouseFilePersistence : public IWarehousePersistence<std::string>
{
public:
    virtual State load(std::string &resource) { throw std::runtime_error("Not implemented WarehouseFilePersistence::load()"); }
    virtual void save(const State &state, std::string &resource) {}
};

#endif /* WAREHOUSE_FILE_PERSISTENCE__H */