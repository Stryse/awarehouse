#include "DeliveryStationListModel.h"

//Presenter
#include "DeliveryStationPresenter.h"

QVector<int> DeliveryStationListModel::m_roles = { RowRole,
                                                   ColumnRole,
                                                   ImageRole };

DeliveryStationListModel::DeliveryStationListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_deliveryStations(nullptr)
{}

int DeliveryStationListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || m_deliveryStations == nullptr)
        return 0;

    return m_deliveryStations->deliveryStations()->size();
}

QVariant DeliveryStationListModel::data(const QModelIndex& index,
                                                       int role) const
{
    if (!index.isValid() || m_deliveryStations == nullptr)
       return QVariant();

    const DeliveryStationPresenter& deliveryStation = *m_deliveryStations->deliveryStations()->at(index.row());

    switch(role)
    {
        case RowRole:
            return QVariant(deliveryStation.row());
        case ColumnRole:
            return QVariant(deliveryStation.column());
        case ImageRole:
            return QVariant(deliveryStation.imagePath());
    }

    return QVariant();
}

bool DeliveryStationListModel::setData(const QModelIndex& index,
                                       const    QVariant& value,
                                                      int role)
{
    if (m_deliveryStations == nullptr)
        return false;

    DeliveryStationPresenter& deliveryStation = *m_deliveryStations->deliveryStations()->at(index.row());

    switch(role)
    {
        case RowRole:
            deliveryStation.setRow      (value.toInt());
            break;
        case ColumnRole:
            deliveryStation.setColumn   (value.toInt());
            break;
        case ImageRole:
            deliveryStation.setImagePath(value.toString());
            break;
    }

    if (m_deliveryStations->setDeliveryStationAt(index.row(), deliveryStation))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags DeliveryStationListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> DeliveryStationListModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[RowRole]    = "row";
    names[ColumnRole] = "column";
    names[ImageRole]  = "image";

    return names;
}

DeliveryStationList* DeliveryStationListModel::deliveryStations() const { return m_deliveryStations; }

void DeliveryStationListModel::setDeliveryStations(DeliveryStationList* deliveryStations)
{
    beginResetModel();

    if (m_deliveryStations)
        m_deliveryStations->disconnect(this);

    m_deliveryStations = deliveryStations;

    if (m_deliveryStations)
    {
        connect(m_deliveryStations, &DeliveryStationList::preItemAppended,  this, [=]()
        {
            const int index = m_deliveryStations->deliveryStations()->size();
            beginInsertRows(QModelIndex(),index,index);
        });

        connect(m_deliveryStations, &DeliveryStationList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_deliveryStations, &DeliveryStationList::preItemRemoved,   this, [=](int index)
        {
            beginRemoveRows(QModelIndex(),index, index);
        });

        connect(m_deliveryStations, &DeliveryStationList::postItemRemoved,  this, [=]()
        {
            endRemoveRows();
        });

        connect(m_deliveryStations, &DeliveryStationList::dataChanged,      this, [=](int index)
        {
            emit dataChanged(QAbstractListModel::index(index), QAbstractListModel::index(index), m_roles);
        });
    }

    endResetModel();
}
