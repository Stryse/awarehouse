#ifndef WAREHOUSE__H
#define WAREHOUSE__H

#include "State.h"
#include "IWarehousePersistence.h"
#include <memory>

// ################# FORWARD DECLARATIONS #######################
class QString;
class AScheduler;
class AController;
class Warehouse;
class QString;
// ##############################################################

class Warehouse
{
public:
    Warehouse(std::unique_ptr<IWarehousePersistence<QString>>&& persistence = nullptr);
    virtual ~Warehouse();

public:
    void tick();
    void loadState(const QString &srcPath);
    void saveState(const QString &destPath);

private:
    int timeStamp;
    std::unique_ptr<AScheduler> scheduler;
    std::unique_ptr<AController> controller;
    std::unique_ptr<IWarehousePersistence<QString>> persistence;
    std::unique_ptr<State> state;
};

#endif /* WAREHOUSE__H */
