#include "PodListModel.h"

//Presenter
#include "PodPresenter.h"

QVector<int> PodListModel::m_roles = { RotationRole,
                                       OrdersRole,
                                       RowRole,
                                       ColumnRole,
                                       ImageRole };


PodListModel::PodListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_pods(nullptr)
{}

int PodListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || m_pods == nullptr)
        return 0;

    return m_pods->pods()->size();
}

QVariant PodListModel::data(const QModelIndex& index,
                                           int role) const
{
    if (!index.isValid() || m_pods == nullptr)
       return QVariant();

    const PodPresenter& pod = *m_pods->pods()->at(index.row());

    switch(role)
    {
        case RotationRole:
            return QVariant(pod.rotation());
        case OrdersRole:
            return QVariant(pod.orders());
        case RowRole:
            return QVariant(pod.row());
        case ColumnRole:
            return QVariant(pod.column());
        case ImageRole:
            return QVariant(pod.imagePath());
    }

    return QVariant();
}

bool PodListModel::setData(const QModelIndex& index,
                           const    QVariant& value,
                                          int role)
{
    if (m_pods == nullptr)
        return false;

    PodPresenter& pod = *m_pods->pods()->at(index.row());

    switch(role)
    {
        case RotationRole:
            pod.setRotation (value.toInt());
            break;
        case OrdersRole:
            pod.setOrders   (value.toStringList());
            break;
        case RowRole:
            pod.setRow      (value.toInt());
            break;
        case ColumnRole:
            pod.setColumn   (value.toInt());
            break;
        case ImageRole:
            pod.setImagePath(value.toString());
            break;
    }

    if (m_pods->setPodAt(index.row(), pod))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags PodListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> PodListModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[RotationRole] = "rotation";
    names[OrdersRole]   = "orders";
    names[RowRole]      = "row";
    names[ColumnRole]   = "column";
    names[ImageRole]    = "image";

    return names;
}

PodList* PodListModel::pods() const { return m_pods; }

void PodListModel::setPods(PodList* pods)
{
    beginResetModel();

    if (m_pods)
        m_pods->disconnect(this);

    m_pods = pods;

    if (m_pods)
    {
        connect(m_pods, &PodList::preItemAppended,  this, [=]()
        {
            const int index = m_pods->pods()->size();
            beginInsertRows(QModelIndex(),index,index);
        });

        connect(m_pods, &PodList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_pods, &PodList::preItemRemoved,   this, [=](int index)
        {
            beginRemoveRows(QModelIndex(),index, index);
        });

        connect(m_pods, &PodList::postItemRemoved,  this, [=]()
        {
            endRemoveRows();
        });

        connect(m_pods, &PodList::dataChanged,      this, [=](int index)
        {
            emit dataChanged(QAbstractListModel::index(index), QAbstractListModel::index(index), m_roles);
        });
    }

    endResetModel();
}
