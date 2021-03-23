#include "OutlinerModel.h"

OutlinerModel::OutlinerModel(QObject* parent)
    : QAbstractListModel(parent), m_actors(nullptr)
{}

OutlinerModel::~OutlinerModel(){}

int OutlinerModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid() || !m_actors)
        return 0;

    return m_actors->actors().size();
}

QVariant OutlinerModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !m_actors)
       return QVariant();

    const Actors& actor = m_actors->actors().at(index.row());
    switch(role) {
            case NameRole:
                return QVariant(actor.name());
            case ActionRole:
                return QVariant(actor.action());
            case BatteryRole:
                return QVariant(actor.battery());
            case OrientationRole:
                return QVariant(actor.orientation());
    }
    return QVariant();
}


bool OutlinerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(data(index,role) != value)
    {
        // TODO IMPLEMENT
        emit dataChanged(index,index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags OutlinerModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> OutlinerModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[ActionRole] = "action";
    names[BatteryRole] = "battery";
    names[OrientationRole] = "orientation";
    return names;
}



void OutlinerModel::setActors(OutlinerList* actors)
{
    beginResetModel();

    if(m_actors)
        m_actors->disconnect(this);

    m_actors = actors;

    if(m_actors)
    {
        connect(m_actors, &OutlinerList::preItemAppended,this,[=](){
            const int index = m_actors->actors().size();
            beginInsertRows(QModelIndex(),index,index);
        });

        connect(m_actors, &OutlinerList::postItemAppended,this,[=](){
            endInsertRows();
        });

        connect(m_actors, &OutlinerList::preItemRemoved, this,[=](int index){
            beginRemoveRows(QModelIndex(),index, index);
        });

        connect(m_actors, &OutlinerList::postItemRemoved, this, [=](){
            endRemoveRows();
        });
    }

    endResetModel();
}

OutlinerList* OutlinerModel::actors() const
{
    return m_actors;
}
