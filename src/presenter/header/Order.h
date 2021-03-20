#ifndef ORDER_H
#define ORDER_H

#include <QObject>

//class Point;

class Order : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Point position READ position NOTIFY positionChanged)
    Q_PROPERTY(int orderNumber READ orderNumber NOTIFY orderNumberChanged)
    Q_PROPERTY(QString assignedRobotName READ assignedRobotName NOTIFY robotNameChanged)

public:
    explicit Order(/*Point position,*/ int orderNumber=0, QString assignedRobotName="");
    Order(Order const &order);
    //Point position() const;
    //void setPosition(const Point &position);

    int orderNumber() const;
    void setOrderNumber(int orderNumber);

    QString assignedRobotName() const;
    void setAssignedRobotName(const QString &name);

signals:
    void positionChanged();
    void nameChanged();
    void orderNumberChanged();

private:
    //Point mPosition;
    int mOrderNumber;
    QString mAssignedRobotName;
};

#endif // ORDER_H
