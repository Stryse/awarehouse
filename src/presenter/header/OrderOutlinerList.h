#ifndef ORDER_OUTLINER_LIST__H
#define ORDER_OUTLINER_LIST__H

#include "Order.h"
#include <QList>
#include <QObject>

class OrderOutlinerList : public QObject
{
    Q_OBJECT

public:
    explicit OrderOutlinerList(QList<const Order*>* orders,QObject *parent = nullptr);

    QList<const Order*> orders() const;
    bool setOrderAt(int index, const Order &order);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendOrder(const Order &order);
    void removeOrder(int index);
    void clear();

public:
    QList<const Order*>* mOrders;
};

#endif /* OUTLINER_LIST__H */
