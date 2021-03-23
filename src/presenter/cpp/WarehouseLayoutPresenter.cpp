#include "WarehouseLayoutPresenter.h"
#include "ChargingStationPresenter.h"
#include "PodDockPresenter.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

WarehouseLayoutPresenter::WarehouseLayoutPresenter(QObject* parent)
    : QObject(parent)
{
}

WarehouseLayoutPresenter::WarehouseLayoutPresenter(QJsonObject& source, QObject* parent)
    : QObject(parent)
{
    if(source.contains("WarehouseLayoutData") && source["WarehouseLayoutData"].isObject())
    {
        QJsonObject WarehouseLayoutData = source["WarehouseLayoutData"].toObject();

        // Read Row and Column
        if(WarehouseLayoutData.contains("RowCount") && WarehouseLayoutData["RowCount"].isDouble() &&
           WarehouseLayoutData.contains("ColCount") && WarehouseLayoutData["ColCount"].isDouble())
        {
            m_rowCount = WarehouseLayoutData["RowCount"].toInt();
            m_colCount = WarehouseLayoutData["ColCount"].toInt();
        }

        // Read Charging Stations
        if(WarehouseLayoutData.contains("ChargingStations") && WarehouseLayoutData["ChargingStations"].isArray())
        {
            QJsonArray chargingStationsJson = WarehouseLayoutData["ChargingStations"].toArray();
            chargingStations.reserve(chargingStationsJson.size());

            for (int i = 0; i < chargingStationsJson.size(); ++i) {

                QJsonObject chargingStationObj = chargingStationsJson[i].toObject();
                chargingStations.append(ChargingStationPresenter::read(chargingStationObj,this));
            }
        }

        // Read Pods
        if(WarehouseLayoutData.contains("PodDocks") && WarehouseLayoutData["PodDocks"].isArray())
        {
            QJsonArray PodsJSon = WarehouseLayoutData["Pods"].toArray();
            podDocks.reserve(PodsJSon.size());

            for(int i = 0; i < PodsJSon.size(); ++i)
            {
                QJsonObject podObj = PodsJSon[i].toObject();
                podDocks.append(PodDockPresenter::read(podObj,this));
            }
        }

    }
    else
    {
        qDebug() << "Jaj jaj nagy a baj";
    }
}

int WarehouseLayoutPresenter::rowCount() const
{
    return m_rowCount;
}

void WarehouseLayoutPresenter::setRowCount(int value)
{
    m_rowCount = value;
    emit rowCountChanged();
}

int WarehouseLayoutPresenter::colCount() const
{
    return m_colCount;
}

void WarehouseLayoutPresenter::setColCount(int colCount)
{
    m_colCount = colCount;
    emit colCountChanged();
}
