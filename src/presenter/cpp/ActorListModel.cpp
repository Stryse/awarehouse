#include "ActorListModel.h"

//Presenter
#include "ActorPresenter.h"

ActorListModel::ActorListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_actors(nullptr)
{}

int ActorListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || m_actors == nullptr)
        return 0;

    return m_actors->actors()->size();
}

QVariant ActorListModel::data(const QModelIndex& index,
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
        case RowRole:
            return QVariant(actor.row());
        case ColumnRole:
            return QVariant(actor.column());
        case ImageRole:
            return QVariant(actor.imagePath());
    }

    return QVariant();
}

bool ActorListModel::setData(const QModelIndex& index,
                             const    QVariant& value,
                                            int role)
{
    if (data(index, role) != value)
    {
        ActorPresenter& actor = *m_actors->actors()->at(index.row());

        switch(role)
        {
            case NameRole:
                actor.setName     (value.toString());
                break;
            case ActionRole:
                actor.setAction   (value.toString());
                break;
            case BatteryRole:
                actor.setBattery  (value.toInt());
                break;
            case RotationRole:
                actor.setRotation (value.toInt());
                break;
            case MovesRole:
                actor.setMoves    (value.toInt());
                break;
            case RowRole:
                actor.setRow      (value.toInt());
                break;
            case ColumnRole:
                actor.setColumn   (value.toInt());
                break;
            case ImageRole:
                actor.setImagePath(value.toString());
                break;
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags ActorListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ActorListModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[NameRole]     = "name";
    names[ActionRole]   = "action";
    names[BatteryRole]  = "battery";
    names[RotationRole] = "rotation";
    names[MovesRole]    = "moves";
    names[RowRole]      = "row";
    names[ColumnRole]   = "column";
    names[ImageRole]    = "image";

    return names;
}

ActorList* ActorListModel::actors() const { return m_actors; }

void ActorListModel::setActors(ActorList* actors)
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
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_actors, &ActorList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_actors, &ActorList::preItemRemoved,   this, [=](int index)
        {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_actors, &ActorList::postItemRemoved,  this, [=]()
        {
            endRemoveRows();
        });
    }

    endResetModel();
}
