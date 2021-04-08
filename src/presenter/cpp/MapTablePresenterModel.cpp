#include "MapTablePresenterModel.h"

MapTablePresenterModel::MapTablePresenterModel(QList<const MapItemPresenter*>* items, QObject* parent)
    : QAbstractTableModel(parent), mapTable(items,this)
{}

int MapTablePresenterModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return 12;
}

int MapTablePresenterModel::columnCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return 12;
}

QVariant MapTablePresenterModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch (role) {
        case ImageRole:
            // TODO use decorator query
            return QVariant((*mapTable.m_items)[getIndexByPoint(index.column(),index.row())]->imagePath());
    }
    return QVariant();
}

bool MapTablePresenterModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(role != ImageRole || data(index,role) == value)
        return false;

    //emit dataChanged(index,index,{role});
    return true;
}

void MapTablePresenterModel::setItems(QList<const MapItemPresenter*>* items)
{
    mapTable.m_items = items;
}

QList<const MapItemPresenter*>* MapTablePresenterModel::items() const
{
    return mapTable.m_items;
}

Qt::ItemFlags MapTablePresenterModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

size_t MapTablePresenterModel::getIndexByPoint(int row, int col) const
{
    return row * 12 + col;
}

QHash<int, QByteArray> MapTablePresenterModel::roleNames() const
{
    return {
        { ImageRole, "image" }
    };
}
