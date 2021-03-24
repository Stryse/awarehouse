#include "MapTableList.h"

// TODO IMPLEMENT EDITING FUNCTIONALITY!!
MapTableList::MapTableList(QList<const MapItemPresenter*>* items, QObject* parent)
    : QObject(parent) ,m_items(items)
{
}

QList<const MapItemPresenter*>* MapTableList::items() const { return m_items; }

bool MapTableList::setMapItemAt(int row, int col, const MapItemPresenter& item) { return false; }

void MapTableList::removeMapItem(int row, int col){}

void MapTableList::clear() { m_items->clear(); }

void MapTableList::appendMapItem(const MapItemPresenter& item) {}
