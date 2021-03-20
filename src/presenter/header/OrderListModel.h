#ifndef ORDERLISTMODEL_H
#define ORDERLISTMODEL_H

#include <QAbstractItemModel>

class Presenter;
class Order;

class OrderListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit OrderListModel(QObject *parent = nullptr);

    enum {
        NameRole,
        TaskRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    QVector<Order*> list() const;
    void setOrderList(QVector<Order*>* orders);

private:
    Presenter* mOrderList;
};

#endif // ORDERLISTMODEL_H
