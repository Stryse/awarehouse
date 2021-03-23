#ifndef ORDEROUTLINER_MODEL__H
#define ORDEROUTLINER_MODEL__H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "Order.h"

class OrderOutlinerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(const QList<Order>* orders READ orders WRITE setOrders)

public:
    explicit OrderOutlinerModel(QObject *parent = nullptr);

    enum {
        OrderNameRole = Qt::UserRole,
        PositionRole,
        RobotNameRole,
        OrderNumberRole
    };

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Editable:
    QHash<int, QByteArray> roleNames() const override;

    const QList<Order>* orders() const;
    void setOrders(const QList<Order>* orders);

private:
    const QList<Order>* mOrders;
};

#endif //ORDEROUTLINER_MODEL__H
