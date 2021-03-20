#include "OutlinerListModel.h"
#include "Presenter.h"

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

    return mActorList->actorItems().size();
}

QVariant OutlinerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || mActorList->actorItems().size() == 0)
        return QVariant();

    const Actors item = mActorList->actorItems().at(index.row());
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

QVector<Actors> OutlinerListModel::list() const
{
    return mActorList->actorItems();
}

void OutlinerListModel::setList(QVector<Actors> *list)
{
    beginResetModel();

    /*if(mActorList->actorItems().size() != 0)
        mActorList->actorItems.disconnect(this);*/

    mActorList->changeActorItems(list);

    if(mActorList->actorItems().size() != 0)
    {
        connect(mActorList, &Presenter::itemAppendedInActors, this, [=](){endInsertRows();});
    }

    endResetModel();
}
