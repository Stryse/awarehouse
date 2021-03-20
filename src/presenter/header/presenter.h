#ifndef PRESENTER_H
#define PRESENTER_H

#include <QObject>
#include <QVector>
#include "Actors.h"
#include "Order.h"

class Presenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY ( int speed READ speed WRITE setSpeed NOTIFY speedChanged )
    Q_PROPERTY ( QVector<Actors> actors READ actors NOTIFY actorsChanged )

public:
    explicit Presenter(QObject *parent = nullptr);

    int speed() const;
    void setSpeed(int speed);

    QVector<Actors> actorItems() const; 
    void changeActorItems(const QVector<Actors>* newActors);
    void addItemToActor(const Actors& newItem);

    QVector<Order> orderItems() const;
    void changeOrderItems(const QVector<Order>* newActors);
    void addItemToOrder(const Order& newItem);
    void deleteItemFromOrder(const Order& item);

signals:
    void speedChanged();
    void itemAppendedInActors();
    void itemAppendedInOrders();
    void itemDeletedFromOrders();

private:
    QVector<Actors> mActor;
    QVector<Order> mOrders;
    int mAnimationSpeed;

};

#endif // PRESENTER_H
