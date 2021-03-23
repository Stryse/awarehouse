#ifndef ORDEROUTLINER_MODEL__H
#define ORDEROUTLINER_MODEL__H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "Order.h"
#include "OrderOutlinerList.h"

class OrderOutlinerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(OrderOutlinerList* orders READ orders WRITE setOrders)

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
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Getter Setter
    OrderOutlinerList* orders() const;
    void setOrders(OrderOutlinerList* orders);

private:
    OrderOutlinerList* mOrders;
};

#endif //ORDEROUTLINER_MODEL__H
