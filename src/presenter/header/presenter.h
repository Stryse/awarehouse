#ifndef PRESENTER_H
#define PRESENTER_H

#include <QObject>
#include <QVector>
#include "Actors.h"
#include "Order.h"

class Presenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int speed READ speed WRITE setSpeed NOTIFY speedChanged)

public:
    explicit Presenter(QObject *parent = nullptr);

    int speed() const;
    void setSpeed(int speed);

signals:
    void speedChanged();

private:
    QVector<Actors> mActor;
    QVector<Order> mOrders;
    int mAnimationSpeed;

};

#endif // PRESENTER_H
