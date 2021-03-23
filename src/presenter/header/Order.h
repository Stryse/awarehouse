#ifndef ORDER_H
#define ORDER_H

#include <QObject>
#include <QPoint>
#include <QDateTime>

class Order : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int positionX READ positionX NOTIFY positionXChanged)
    Q_PROPERTY(int positionY READ positionY NOTIFY positionYChanged)
    Q_PROPERTY(int orderNumber READ orderNumber NOTIFY orderNumberChanged)
    Q_PROPERTY(QString orderName READ orderName NOTIFY orderNameChanged)
    Q_PROPERTY(QString assignedRobotName READ assignedRobotName NOTIFY robotNameChanged)

public:
    explicit Order(int positionX = 0, int positionY = 0, int orderNumber = 0, QString orderName = "valami", QString assignedRobotName="N/A");
    Order(QString orderName);
    Order(Order const &order);

    int positionX() const;
    void setPositionX(int positionX);

    int positionY() const;
    void setPositionY(int positionY);

    int orderNumber() const;
    void setOrderNumber(int orderNumber);

    QString orderName() const;
    void setOrderName(const QString &name);

    QString assignedRobotName() const;
    void setAssignedRobotName(const QString &name);

    friend bool operator==(Order* const lhs, const Order rhs)
    {
        return lhs->mOrderNumber == rhs.mOrderNumber &&
               lhs->mAssignedRobotName == rhs.mAssignedRobotName &&
               lhs->mOrderPositionX == rhs.mOrderPositionX &&
               lhs->mOrderPositionY == rhs.mOrderPositionY &&
               lhs->mOrderName == rhs.mOrderName;
    }

    Order& operator=(const Order& other);
signals:
    void positionXChanged();
    void positionYChanged();
    void robotNameChanged();
    void orderNumberChanged();
    void orderNameChanged();

private:
    int mOrderPositionX;
    int mOrderPositionY;
    int mOrderNumber;
    QString mOrderName;
    QString mAssignedRobotName;
};

#endif // ORDER_H
