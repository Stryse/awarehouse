#include "ActorOutlinerModel.h"

ActorOutlinerModel::ActorOutlinerModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_actors(nullptr)
{}

int ActorOutlinerModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || m_actors == nullptr)
        return 0;

    return m_actors->actors()->size();
}

QVariant ActorOutlinerModel::data(const QModelIndex& index,
                                                 int role) const
{
    if (!index.isValid() || m_actors == nullptr)
       return QVariant();

    const ActorPresenter& actor = *m_actors->actors()->at(index.row());

    switch(role)
    {
        case NameRole:
            return QVariant(actor.name());
        case ActionRole:
            return QVariant(actor.action());
        case BatteryRole:
            return QVariant(actor.battery());
        case RotationRole:
            return QVariant(actor.rotation());
        case MovesRole:
            return QVariant(actor.moves());
    }

    return QVariant();
}

bool ActorOutlinerModel::setData(const QModelIndex& index,
                                 const    QVariant& value,
                                                int role)
{
    if (data(index, role) != value)
    {
        // TODO IMPLEMENT
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags ActorOutlinerModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ActorOutlinerModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[NameRole]     = "name";
    names[ActionRole]   = "action";
    names[BatteryRole]  = "battery";
    names[RotationRole] = "rotation";
    names[MovesRole]    = "moves";

    return names;
}

ActorList* ActorOutlinerModel::actors() const { return m_actors; }

void ActorOutlinerModel::setActors(ActorList* actors)
{
    beginResetModel();

    if (m_actors)
        m_actors->disconnect(this);

    m_actors = actors;

    if (m_actors)
    {
        connect(m_actors, &ActorList::preItemAppended,  this, [=]()
        {
            const int index = m_actors->actors()->size();
            beginInsertRows(QModelIndex(),index,index);
        });

        connect(m_actors, &ActorList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_actors, &ActorList::preItemRemoved,   this, [=](int index)
        {
            beginRemoveRows(QModelIndex(),index, index);
        });

        connect(m_actors, &ActorList::postItemRemoved,  this, [=]()
        {
            endRemoveRows();
        });
    }

    endResetModel();
}
