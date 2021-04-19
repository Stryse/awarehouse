#include "PersistencePresenter.h"

#include <QDir>
#include <QStringList>

const QString PersistencePresenter::defaultWarehouseName   = "Default";

const QString PersistencePresenter::m_defaultWarehousePath = ":/maps/Map01.json";
const QString PersistencePresenter::m_warehouseDirPath     = "../maps";

PersistencePresenter::PersistencePresenter(QObject* parent)
    : QObject(parent)
    , m_warehouses(this)
{
    readWarehouses(m_warehouseDirPath);
}

QStringList PersistencePresenter::warehouses()           const { return m_warehouses.stringList(); }
QString     PersistencePresenter::defaultWarehousePath() const { return m_defaultWarehousePath;    }
QString     PersistencePresenter::warehouseDirPath()     const { return m_warehouseDirPath;        }

QString PersistencePresenter::getWarehousePath(const QString& warehouseName)
{
    if (warehouseName == "Default")
        return m_defaultWarehousePath;
    else
        return m_warehouseDirPath + "\\" + warehouseName + ".json";
}

int PersistencePresenter::getWarehouseIndex(const QString& warehouseName) const
{
    return m_warehouses.stringList().indexOf(warehouseName);
}

void PersistencePresenter::readWarehouses(QString path)
{
    QDir warehousesDir(path);

    //If doesn't exist -> create directory
    if(!warehousesDir.exists())
        warehousesDir.mkdir(path);

    //Scan directory for .json files
    QStringList warehouses = warehousesDir.entryList(QStringList() << "*.json" << "*.JSON", QDir::Files);
    for(int i = 0; i < warehouses.size(); ++i )
        warehouses[i].chop(5);

    //Fill warehouselist + add Default warehouse
    warehouses.push_front(defaultWarehouseName);
    m_warehouses.setStringList(warehouses);

    emit warehousesChanged();
}

void PersistencePresenter::appendWarehouse(QString warehouse)
{
    QStringList warehouses = m_warehouses.stringList();
    warehouses.append(warehouse);
    m_warehouses.setStringList(warehouses);

    emit warehousesChanged();
}

void PersistencePresenter::removeWarehouse(int index)
{
    QStringList warehouses = m_warehouses.stringList();
    warehouses.removeAt(index);
    m_warehouses.setStringList(warehouses);

    emit warehousesChanged();
}

void PersistencePresenter::reloadWarehouses() { readWarehouses(m_warehouseDirPath); }
