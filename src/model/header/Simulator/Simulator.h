#ifndef SIMULATOR__H
#define SIMULATOR__H

#include "ISimulator.h"
#include <QObject>
#include <QTimer>

// ###################### Forward declarations ##########################
class Warehouse;
// ######################################################################

/**************************************************************
 * @brief An implementation of the ISimulator interface
 * for our Warehouse type.
 **************************************************************/
class Simulator : public QObject, public ISimulator<Warehouse>
{
    Q_OBJECT

public:
    Simulator(QObject* parent = nullptr);
    virtual ~Simulator();

public slots:
    virtual bool isAvailable() const override;
    virtual void bindSimTypeData(Warehouse *warehouse) override;
    virtual void startSimulation() override;
    virtual void stopSimulation() override;
    virtual void setTickRate(int tickRate) override;
    virtual void setWarehouseStateAt(int timeStamp) override;

private:
    void makeAvailable();
    void makeUnAvailable();

private:
    bool isRunning;
    Warehouse *warehouse;
    QTimer timer;
    int tickRate;
};

#endif /* SIMULATOR__H */
