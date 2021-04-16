#include "ChargingStationListModel.h"

//Presenter
#include "ChargingStationPresenter.h"

QVector<int> ChargingStationListModel::m_roles = { RowRole,
                                                   ColumnRole,
                                                   ImageRole };

ChargingStationListModel::ChargingStationListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_chargingStations(nullptr)
{}

int ChargingStationListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || m_chargingStations == nullptr)
        return 0;

    return m_chargingStations->chargingStations()->size();
}

QVariant ChargingStationListModel::data(const QModelIndex& index,
                                                       int role) const
{
    if (!index.isValid() || m_chargingStations == nullptr)
       return QVariant();

    const ChargingStationPresenter& chargingStation = *m_chargingStations->chargingStations()->at(index.row());

    switch(role)
    {
        case RowRole:
            return QVariant(chargingStation.row());
        case ColumnRole:
            return QVariant(chargingStation.column());
        case ImageRole:
            return QVariant(chargingStation.imagePath());
    }

    return QVariant();
}

bool ChargingStationListModel::setData(const QModelIndex& index,
                                       const    QVariant& value,
                                                      int role)
{
    if (m_chargingStations == nullptr)
        return false;

    ChargingStationPresenter& chargingStation = *m_chargingStations->chargingStations()->at(index.row());

    switch(role)
    {
        case RowRole:
            chargingStation.setRow      (value.toInt());
            break;
        case ColumnRole:
            chargingStation.setColumn   (value.toInt());
            break;
        case ImageRole:
            chargingStation.setImagePath(value.toString());
            break;
    }

    if (m_chargingStations->setChargingStationAt(index.row(), chargingStation))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ChargingStationListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ChargingStationListModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[RowRole]    = "row";
    names[ColumnRole] = "column";
    names[ImageRole]  = "image";

    return names;
}

ChargingStationList* ChargingStationListModel::chargingStations() const { return m_chargingStations; }

void ChargingStationListModel::setChargingStations(ChargingStationList* actors)
{
    beginResetModel();

    if (m_chargingStations)
        m_chargingStations->disconnect(this);

    m_chargingStations = actors;

    if (m_chargingStations)
    {
        connect(m_chargingStations, &ChargingStationList::preItemAppended,  this, [=]()
        {
            const int index = m_chargingStations->chargingStations()->size();
            beginInsertRows(QModelIndex(),index,index);
        });

        connect(m_chargingStations, &ChargingStationList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_chargingStations, &ChargingStationList::preItemRemoved,   this, [=](int index)
        {
            beginRemoveRows(QModelIndex(),index, index);
        });

        connect(m_chargingStations, &ChargingStationList::postItemRemoved,  this, [=]()
        {
            endRemoveRows();
        });

        connect(m_chargingStations, &ChargingStationList::dataChanged,      this, [=](int index)
        {
            emit dataChanged(QAbstractListModel::index(index), QAbstractListModel::index(index), m_roles);
        });
    }

    endResetModel();
}
