#ifndef MAP_TABLE_LIST__H
#define MAP_TABLE_LIST__H

#include <QObject>
#include <QList>
#include "Actors.h"

class MapTableList : public QObject
{
    Q_OBJECT

public:
    explicit MapTableList(QList<const MapItemPresenter*>* items, QObject *parent = nullptr);

    QList<const MapItemPresenter*>* items() const;
    bool setMapItemAt(int row, int col, const MapItemPresenter& item);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendMapItem(const MapItemPresenter& item);
    void removeMapItem(int row, int col);
    void clear();

public:
    QList<const MapItemPresenter*>* m_items;
};

#endif /* MAP_TABLE_LIST__H */
