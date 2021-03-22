#include "OutlinerModel.h"

OutlinerModel::OutlinerModel(QObject* parent)
    : QAbstractListModel(parent), m_actors(nullptr)
{}

OutlinerModel::~OutlinerModel(){}

int OutlinerModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid() || !m_actors)
        return 0;

    return m_actors->size();
}

QVariant OutlinerModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !m_actors)
       return QVariant();

    const Actors& actor = m_actors->at(index.row());
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



QHash<int, QByteArray> OutlinerModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[ActionRole] = "action";
    names[BatteryRole] = "battery";
    names[OrientationRole] = "orientation";
    return names;
}

void OutlinerModel::setActors(const QList<Actors>* actors)
{
    beginResetModel();
    this->m_actors = actors;
    endResetModel();
}

const QList<Actors>* OutlinerModel::actors() const
{
    return m_actors;
}
