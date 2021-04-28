#include "WarehouseEditorPresenter.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDebug>

//Presenter
#include "WarehouseLayoutPresenter.h"

const QString                      WarehouseEditorPresenter::newWarehouseName = "New";
WarehouseEditorPresenter::TileType WarehouseEditorPresenter::baseTile         = ROAD;

WarehouseEditorPresenter::WarehouseEditorPresenter(PersistencePresenter* persistence,
                                                                QObject* parent)
    : QObject(parent)
    , m_currentWarehouseName(newWarehouseName)
    , m_layout(new WarehouseLayoutPresenter(this))
    , m_tileTypeTable(QVector<QVector<TileType>>())
    , m_persistence(persistence == nullptr ? new PersistencePresenter(this) : persistence)
{   
    createTileTypeTable(m_layout->rows(), m_layout->columns());

    connect(m_layout, &WarehouseLayoutPresenter::rowsChanged,    this, [=](int rows)    { updateRowsInTable(rows);       });
    connect(m_layout, &WarehouseLayoutPresenter::columnsChanged, this, [=](int columns) { updateColumnsInTable(columns); });

    connect(m_layout->deliveryStations(), &DeliveryStationList::preItemRemoved, this, [=](int index)
    {
        //Pls look away...
        for (auto& pod : *m_layout->pods()->pods())
        {
            QStringList orders = pod->orders();

            orders.removeOne(QString::number(index + 1));
            for (int i = 0; i < orders.size(); ++i)
            {
                int orderInt = orders[i].toInt();

                if (orderInt > index + 1)
                    orders[i] = QString::number(orderInt - 1);
            }

            pod->setOrders(orders);
        }
    });
}

QString                   WarehouseEditorPresenter::currentWarehouseName() const { return m_currentWarehouseName; }
WarehouseLayoutPresenter* WarehouseEditorPresenter::layout()               const { return m_layout;               }
PersistencePresenter*     WarehouseEditorPresenter::persistence()          const { return m_persistence;          }

void WarehouseEditorPresenter::updateRowsInTable(int newRows)
{
    int rowDelta = newRows - m_tileTypeTable.size();

    if (rowDelta > 0)
        for (int i = 0; i < rowDelta; ++i)
            m_tileTypeTable.emplace_back(QVector<TileType>(m_layout->columns(), baseTile));
    else if (rowDelta < 0)
    {
        for (int i = 0; i < rowDelta * -1; ++i)
            m_tileTypeTable.pop_back();

        //Actors
        ActorList& actors = *m_layout->actors();
        for (int i = actors.actors()->size()-1; i >= 0; --i)
        {
            ActorPresenter& actor = *actors.actors()->at(i);
            if (actor.row() >= newRows)
                actors.removeActor(i);
        }

        //ChargingStations
        ChargingStationList& chargingStations = *m_layout->chargingStations();
        for (int i = chargingStations.chargingStations()->size()-1; i >= 0; --i)
        {
            ChargingStationPresenter& chargingStation = *chargingStations.chargingStations()->at(i);
            if (chargingStation.row() >= newRows)
                chargingStations.removeChargingStation(i);
        }

        //PodDocks
        PodList& pods = *m_layout->pods();
        for (int i = pods.pods()->size()-1; i >= 0; --i)
        {
            PodPresenter& pod = *pods.pods()->at(i);
            if (pod.row() >= newRows)
                pods.removePod(i);
        }

        //ChargingStations
        DeliveryStationList& deliveryStations = *m_layout->deliveryStations();
        for (int i = deliveryStations.deliveryStations()->size()-1; i >= 0; --i)
        {
            DeliveryStationPresenter& deliveryStation = *deliveryStations.deliveryStations()->at(i);
            if (deliveryStation.row() >= newRows)
                deliveryStations.removeDeliveryStation(i);
        }
    }
}

void WarehouseEditorPresenter::updateColumnsInTable(int newColumns)
{
    int columnDelta = newColumns - m_tileTypeTable[0].size();

    if (columnDelta > 0)
        for (int i = 0; i < m_layout->rows(); ++i)
            for (int j = 0; j < columnDelta; ++j)
                m_tileTypeTable[i].emplace_back(baseTile);
    else
    {
        for (int i = 0; i < m_layout->rows(); ++i)
            for (int j = 0; j < columnDelta * -1; ++j)
                m_tileTypeTable[i].pop_back();

        //Actors
        ActorList& actors = *m_layout->actors();
        for (int i = actors.actors()->size()-1; i >= 0; --i)
        {
            ActorPresenter& actor = *actors.actors()->at(i);
            if (actor.column() >= newColumns)
                actors.removeActor(i);
        }

        //ChargingStations
        ChargingStationList& chargingStations = *m_layout->chargingStations();
        for (int i = chargingStations.chargingStations()->size()-1; i >= 0; --i)
        {
            ChargingStationPresenter& chargingStation = *chargingStations.chargingStations()->at(i);
            if (chargingStation.column() >= newColumns)
                chargingStations.removeChargingStation(i);
        }

        //Pods
        PodList& pods = *m_layout->pods();
        for (int i = pods.pods()->size()-1; i >= 0; --i)
        {
            PodPresenter& pod = *pods.pods()->at(i);
            if (pod.column() >= newColumns)
                pods.removePod(i);
        }

        //ChargingStations
        DeliveryStationList& deliveryStations = *m_layout->deliveryStations();
        for (int i = deliveryStations.deliveryStations()->size()-1; i >= 0; --i)
        {
            DeliveryStationPresenter& deliveryStation = *deliveryStations.deliveryStations()->at(i);
            if (deliveryStation.column() >= newColumns)
                deliveryStations.removeDeliveryStation(i);
        }
    }
}

void WarehouseEditorPresenter::setTile(int row, int column, TileType type)
{
    removeTile(row, column);

    switch (type)
    {
        case ACTOR:
            m_layout->actors()->appendActor(*new ActorPresenter(row, column, m_layout));
            break;
        case CHARGING_STATION:
            m_layout->chargingStations()->appendChargingStation(*new ChargingStationPresenter(row, column, m_layout));
            break;
        case POD:
            m_layout->pods()->appendPod(*new PodPresenter(row, column, m_layout));
            break;
        case DELIVERY_STATION:
            m_layout->deliveryStations()->appendDeliveryStation(*new DeliveryStationPresenter(row, column, m_layout));
            break;
        default: break;
    }

    m_tileTypeTable[row][column] = type;
}

void WarehouseEditorPresenter::removeTile(int row, int column)
{
    switch (m_tileTypeTable[row][column])
    {
        case ACTOR:
            m_layout->actors()->removeActor(row, column);
            break;
        case CHARGING_STATION:
            m_layout->chargingStations()->removeChargingStation(row, column);
            break;
        case POD:
            m_layout->pods()->removePod(row, column);
            break;
        case DELIVERY_STATION:
            m_layout->deliveryStations()->removeDeliveryStation(row, column);
            break;
        default: return;
    }

    m_tileTypeTable[row][column] = baseTile;
}

bool WarehouseEditorPresenter::isInBounds(int row, int column) const
{
    return row    >= 0 && row    < m_layout->rows() &&
           column >= 0 && column < m_layout->columns();
}

bool WarehouseEditorPresenter::canMoveToTile(int row, int column) const
{
    return isInBounds(row, column) && m_tileTypeTable[row][column] == baseTile;
}

bool WarehouseEditorPresenter::moveTile(WarehouseEditorPresenter::TileType type, int index, int row, int column)
{
    if (!canMoveToTile(row, column))
        return false;

    switch (type)
    {
        case ACTOR:
        {
            ActorPresenter* actor = m_layout->actors()->actors()->at(index);

            m_tileTypeTable[actor->row()][actor->column()] = baseTile;
            actor->setRow(row);
            actor->setColumn(column);
            break;
        }
        case CHARGING_STATION:
        {
            ChargingStationPresenter* chargingStation = m_layout->chargingStations()->chargingStations()->at(index);

            m_tileTypeTable[chargingStation->row()][chargingStation->column()] = baseTile;
            chargingStation->setRow(row);
            chargingStation->setColumn(column);
            break;
        }
        case POD:
        {
            PodPresenter* pod = m_layout->pods()->pods()->at(index);

            m_tileTypeTable[pod->row()][pod->column()] = baseTile;
            pod->setRow(row);
            pod->setColumn(column);
            break;
        }
        case DELIVERY_STATION:
        {
            DeliveryStationPresenter* deliveryStation = m_layout->deliveryStations()->deliveryStations()->at(index);

            m_tileTypeTable[deliveryStation->row()][deliveryStation->column()] = baseTile;
            deliveryStation->setRow(row);
            deliveryStation->setColumn(column);
            break;
        }
        default: return false;
    }

    m_tileTypeTable[row][column] = type;

    return true;
}

bool WarehouseEditorPresenter::moveMultipleTile(TileType type, QList<int> indices, int deltaRow, int deltaColumn)
{
    switch (type) {
        case ACTOR:
        {
            for (int i = 0; i < indices.size(); ++i)
            {
                ActorPresenter* actor = m_layout->actors()->actors()->at(indices[i]);

                int newRow    = actor->row()    + deltaRow;
                int newColumn = actor->column() + deltaColumn;

                // Indices out of bound
                if (!isInBounds(newRow, newColumn))
                    return false;

                // If tile at the new position is in selected list => OK
                if (m_tileTypeTable[newRow][newColumn] == ACTOR)
                {
                    int j = 0;
                    for (; j < indices.size(); ++j)
                    {
                        if (i == j) continue;

                        ActorPresenter* otherActor = m_layout->actors()->actors()->at(indices[j]);
                        if (otherActor->row() == newRow && otherActor->column() == newColumn)
                            break;
                    }
                    // Reached end of selected list and tile at new position is ACTOR => NOT OK
                    if (j == indices.size())
                        return false;
                }
                // Tile at new position is not empty
                else if (m_tileTypeTable[newRow][newColumn] != baseTile)
                    return false;
            }
            // Move tiles
            for (int i = 0; i < indices.size(); ++i)
            {
                ActorPresenter* actor = m_layout->actors()->actors()->at(indices[i]);

                m_tileTypeTable[actor->row()][actor->column()] = baseTile;
                actor->setRow(actor->row() + deltaRow);
                actor->setColumn(actor->column() + deltaColumn);
            }
            for (int i = 0; i < indices.size(); ++i)
            {
                ActorPresenter* actor = m_layout->actors()->actors()->at(indices[i]);
                m_tileTypeTable[actor->row()][actor->column()] = ACTOR;
            }
            break;
        }
        case CHARGING_STATION:
        {
            for (int i = 0; i < indices.size(); ++i)
            {
                ChargingStationPresenter* chargingStation = m_layout->chargingStations()->chargingStations()->at(indices[i]);

                int newRow    = chargingStation->row()    + deltaRow;
                int newColumn = chargingStation->column() + deltaColumn;

                // Indices out of bound
                if (!isInBounds(newRow, newColumn))
                    return false;

                // If tile at the new position is in selected list => OK
                if (m_tileTypeTable[newRow][newColumn] == CHARGING_STATION)
                {
                    int j = 0;
                    for (; j < indices.size(); ++j)
                    {
                        if (i == j) continue;

                        ChargingStationPresenter* otherChargingStation = m_layout->chargingStations()->chargingStations()->at(indices[j]);
                        if (otherChargingStation->row() == newRow && otherChargingStation->column() == newColumn)
                            break;
                    }
                    // Reached end of selected list and tile at new position is CHARGING_STATION => NOT OK
                    if (j == indices.size())
                        return false;
                }
                // Tile at new position is not empty
                else if (m_tileTypeTable[newRow][newColumn] != baseTile)
                    return false;
            }
            // Move tiles
            for (int i = 0; i < indices.size(); ++i)
            {
                ChargingStationPresenter* chargingStation = m_layout->chargingStations()->chargingStations()->at(indices[i]);

                m_tileTypeTable[chargingStation->row()][chargingStation->column()] = baseTile;
                chargingStation->setRow(chargingStation->row() + deltaRow);
                chargingStation->setColumn(chargingStation->column() + deltaColumn);
            }
            for (int i = 0; i < indices.size(); ++i)
            {
                ChargingStationPresenter* chargingStation = m_layout->chargingStations()->chargingStations()->at(indices[i]);
                m_tileTypeTable[chargingStation->row()][chargingStation->column()] = CHARGING_STATION;
            }
            break;
        }
        case POD:
        {
            for (int i = 0; i < indices.size(); ++i)
            {
                PodPresenter* pod = m_layout->pods()->pods()->at(indices[i]);

                int newRow    = pod->row()    + deltaRow;
                int newColumn = pod->column() + deltaColumn;

                // Indices out of bound
                if (!isInBounds(newRow, newColumn))
                    return false;

                // If tile at the new position is in selected list => OK
                if (m_tileTypeTable[newRow][newColumn] == POD)
                {
                    int j = 0;
                    for (; j < indices.size(); ++j)
                    {
                        if (i == j) continue;

                        PodPresenter* otherPod = m_layout->pods()->pods()->at(indices[j]);
                        if (otherPod->row() == newRow && otherPod->column() == newColumn)
                            break;
                    }
                    // Reached end of selected list and tile at new position is POD => NOT OK
                    if (j == indices.size())
                        return false;
                }
                // Tile at new position is not empty
                else if (m_tileTypeTable[newRow][newColumn] != baseTile)
                    return false;
            }
            // Move tiles
            for (int i = 0; i < indices.size(); ++i)
            {
                PodPresenter* pod = m_layout->pods()->pods()->at(indices[i]);

                m_tileTypeTable[pod->row()][pod->column()] = baseTile;
                pod->setRow(pod->row() + deltaRow);
                pod->setColumn(pod->column() + deltaColumn);
            }
            for (int i = 0; i < indices.size(); ++i)
            {
                PodPresenter* pod = m_layout->pods()->pods()->at(indices[i]);
                m_tileTypeTable[pod->row()][pod->column()] = POD;
            }
            break;
        }
        case DELIVERY_STATION:
        {
            for (int i = 0; i < indices.size(); ++i)
            {
                DeliveryStationPresenter* deliveryStation = m_layout->deliveryStations()->deliveryStations()->at(indices[i]);

                int newRow    = deliveryStation->row()    + deltaRow;
                int newColumn = deliveryStation->column() + deltaColumn;

                // Indices out of bound
                if (!isInBounds(newRow, newColumn))
                    return false;

                // If tile at the new position is in selected list => OK
                if (m_tileTypeTable[newRow][newColumn] == DELIVERY_STATION)
                {
                    int j = 0;
                    for (; j < indices.size(); ++j)
                    {
                        if (i == j) continue;

                        DeliveryStationPresenter* otherDeliveryStation = m_layout->deliveryStations()->deliveryStations()->at(indices[j]);
                        if (otherDeliveryStation->row() == newRow && otherDeliveryStation->column() == newColumn)
                            break;
                    }
                    // Reached end of selected list and tile at new position is DELIVERY_STATION => NOT OK
                    if (j == indices.size())
                        return false;
                }
                // Tile at new position is not empty
                else if (m_tileTypeTable[newRow][newColumn] != baseTile)
                    return false;
            }
            // Move tiles
            for (int i = 0; i < indices.size(); ++i)
            {
                DeliveryStationPresenter* deliveryStation = m_layout->deliveryStations()->deliveryStations()->at(indices[i]);

                m_tileTypeTable[deliveryStation->row()][deliveryStation->column()] = baseTile;
                deliveryStation->setRow(deliveryStation->row() + deltaRow);
                deliveryStation->setColumn(deliveryStation->column() + deltaColumn);
            }
            for (int i = 0; i < indices.size(); ++i)
            {
                DeliveryStationPresenter* deliveryStation = m_layout->deliveryStations()->deliveryStations()->at(indices[i]);
                m_tileTypeTable[deliveryStation->row()][deliveryStation->column()] = DELIVERY_STATION;
            }
            break;
        }
        default: return false;
    }

    return true;
}

void WarehouseEditorPresenter::clearWarehouse()
{
    m_layout->actors()->clear();
    m_layout->chargingStations()->clear();
    m_layout->pods()->clear();
    m_layout->deliveryStations()->clear();
}

void WarehouseEditorPresenter::loadWarehouse(const QString& warehouseName)
{
    QString warehousePath = PersistencePresenter::getWarehousePath(warehouseName);
    QFile   warehouseFile(warehousePath);
    QString jsonString;

    if (warehouseFile.exists())
        if (warehouseFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            jsonString = warehouseFile.readAll();
            warehouseFile.close();

            QJsonDocument warehouseDoc     = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject   warehouseJsonObj = warehouseDoc.object();

            loadJsonObject(warehouseJsonObj);

            m_currentWarehouseName         = warehouseName;
            emit currentWarehouseNameChanged(warehouseName);
        }
}

void WarehouseEditorPresenter::loadJsonObject(const QJsonObject& json)
{
    if (json.contains("WarehouseLayoutData") && json["WarehouseLayoutData"].isObject())
    {
        QJsonObject warehouseLayoutData = json["WarehouseLayoutData"].toObject();

        //Row and Column
        if (warehouseLayoutData.contains("RowCount") && warehouseLayoutData["RowCount"].isDouble() &&
            warehouseLayoutData.contains("ColCount") && warehouseLayoutData["ColCount"].isDouble())
        {
            int rows    = warehouseLayoutData["RowCount"].toInt();
            int columns = warehouseLayoutData["ColCount"].toInt();

            m_layout->setRows(rows);
            m_layout->setColumns(columns);

            createTileTypeTable(rows, columns);
        }

        //Charging Stations
        if (warehouseLayoutData.contains("ChargingStations") && warehouseLayoutData["ChargingStations"].isArray())
        {
            QJsonArray chargingStationsJson = warehouseLayoutData["ChargingStations"].toArray();
            m_layout->chargingStations()->loadJsonArray(chargingStationsJson);

            for (int i = 0; i < m_layout->chargingStations()->chargingStations()->size(); ++i)
            {
                ChargingStationPresenter& chargingStation = *m_layout->chargingStations()->chargingStations()->at(i);
                m_tileTypeTable[chargingStation.row()][chargingStation.column()] = CHARGING_STATION;
            }
        }

        //Delivery Stations
        if (warehouseLayoutData.contains("DeliveryStations") && warehouseLayoutData["DeliveryStations"].isArray())
        {
            QJsonArray deliveryStationsJson = warehouseLayoutData["DeliveryStations"].toArray();
            m_layout->deliveryStations()->loadJsonArray(deliveryStationsJson);

            for (int i = 0; i < m_layout->deliveryStations()->deliveryStations()->size(); ++i)
            {
                DeliveryStationPresenter& deliveryStation = *m_layout->deliveryStations()->deliveryStations()->at(i);
                m_tileTypeTable[deliveryStation.row()][deliveryStation.column()] = DELIVERY_STATION;
            }
        }

        //Pods - IMPORTANT: Not podDock, beacuse it doesn't have ORDERS
        if (warehouseLayoutData.contains("PodDocks") && warehouseLayoutData["PodDocks"].isArray())
        {
            QJsonArray podsJson = warehouseLayoutData["PodDocks"].toArray();
            m_layout->pods()->loadJsonArray(podsJson);

            for (int i = 0; i < m_layout->pods()->pods()->size(); ++i)
            {
                PodPresenter& pod = *m_layout->pods()->pods()->at(i);
                m_tileTypeTable[pod.row()][pod.column()] = POD;
            }
        }

        //Actors
        if (warehouseLayoutData.contains("DeliveryRobots") && warehouseLayoutData["DeliveryRobots"].isArray())
        {
            QJsonArray actorsJson = warehouseLayoutData["DeliveryRobots"].toArray();
            m_layout->actors()->loadJsonArray(actorsJson);

            for (int i = 0; i < m_layout->actors()->actors()->size(); ++i)
            {
                ActorPresenter& actor = *m_layout->actors()->actors()->at(i);
                m_tileTypeTable[actor.row()][actor.column()] = ACTOR;
            }
        }
    }
}

bool WarehouseEditorPresenter::saveWarehouse(const QString& warehouseName)
{
    if (warehouseName == PersistencePresenter::defaultWarehouseName)
        return false;

    QString warehousePath = PersistencePresenter::getWarehousePath(warehouseName);

    QFile warehouseFile(warehousePath);
    if (!warehouseFile.open(QIODevice::WriteOnly))
        return false;

    QJsonObject warehouseJsonObj = saveJsonObject();

    warehouseFile.write(QJsonDocument(warehouseJsonObj).toJson());

    m_persistence->reloadWarehouses();

    if (m_currentWarehouseName != warehouseName)
    {
        m_currentWarehouseName = warehouseName;
        emit currentWarehouseNameChanged(warehouseName);
    }

    return true;
}

bool WarehouseEditorPresenter::isDefaultWarehouse() const { return m_currentWarehouseName == PersistencePresenter::defaultWarehouseName ||
                                                                   m_currentWarehouseName == newWarehouseName; }

QJsonObject WarehouseEditorPresenter::saveJsonObject() const
{
    QJsonObject warehouseLayoutData;

    warehouseLayoutData.insert("RowCount", m_layout->rows());
    warehouseLayoutData.insert("ColCount", m_layout->columns());

    warehouseLayoutData.insert("ChargingStations", m_layout->chargingStations()->saveJsonArray());
    //TODO: No order ID
    warehouseLayoutData.insert("DeliveryStations", m_layout->deliveryStations()->saveJsonArray());
    //Pods - IMPORTANT: Not podDock, beacuse it doesn't have ORDERS
    warehouseLayoutData.insert("PodDocks",         m_layout->pods()->saveJsonArray());
    warehouseLayoutData.insert("DeliveryRobots",   m_layout->actors()->saveJsonArray());

    QJsonObject warehouseJsonObj;
    warehouseJsonObj.insert("WarehouseLayoutData", warehouseLayoutData);

    return warehouseJsonObj;
}

void WarehouseEditorPresenter::createTileTypeTable(int rows, int columns)
{
    m_tileTypeTable.clear();

    for (int i = 0; i < rows; ++i)
        m_tileTypeTable.emplace_back(QVector<TileType>(columns, baseTile));
}
