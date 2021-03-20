#include "OutlinerListModel.h"
#include "OutlinerListActor.h"

OutlinerListModel::OutlinerListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

/*QVariant OutlinerListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}*/

int OutlinerListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return mList->actorItems().size();
}

QVariant OutlinerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const Actors item = mList->actorItems().at(index.row());
    switch(role) {
        case NameRole:
            return QVariant(item.name());
        case ActionRole:
            return QVariant(item.action());
        case BatteryRole:
            return QVariant(item.battery());
        case OrientationRole:
            return QVariant(item.orientation());
    }

    return QVariant();
}

QHash<int, QByteArray> OutlinerListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[ActionRole] = "action";
    names[BatteryRole] = "batterlevel";
    names[OrientationRole] = "direction";
    return names;
}

OutlinerListActor *OutlinerListModel::list() const
{
    return mList;
}

void OutlinerListModel::setList(OutlinerListActor *list)
{
    beginResetModel();

    if(mList)
        mList->disconnect(this);

    mList = list;

    if(mList)
    {
        connect(mList, &OutlinerListActor::itemAppendedInActors, this, [=](){endInsertRows();});
    }

    endResetModel();
}
