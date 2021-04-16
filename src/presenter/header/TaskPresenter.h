#ifndef TASK_PRESENTER_H
#define TASK_PRESENTER_H

#include <QObject>
#include <QPoint>
#include <QDateTime>

class TaskPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(      QString assignedRobotName READ assignedRobotName NOTIFY assignedRobotNameChanged )
    Q_PROPERTY( QVector<int> orders            READ orders            NOTIFY ordersChanged            )
    Q_PROPERTY(          int destinationX      READ destinationX      NOTIFY destinationXChanged      )
    Q_PROPERTY(          int destinationY      READ destinationY      NOTIFY destinationYChanged      )

public:
    explicit TaskPresenter(     QString assignedRobotName = "N/A",
                           QVector<int> orders            = QVector<int>(),
                                    int destinationX      = 0,
                                    int destinationY      = 0,
                               QObject* parent            = nullptr);
    explicit TaskPresenter(const TaskPresenter& task);

    TaskPresenter& operator=(const TaskPresenter& other);
    bool operator==(const TaskPresenter& other) const;

    //Getter
    QString      assignedRobotName() const;
    QVector<int> orders()            const;
    int          destinationX()      const;
    int          destinationY()      const;

    //Setter
    void setAssignedRobotName(const QString& robotName);
    void setOrders(const QVector<int>& orders);
    void setDestinationX(int destinationX);
    void setDestinationY(int destinationY);

signals:
    void assignedRobotNameChanged();
    void ordersChanged();
    void destinationXChanged();
    void destinationYChanged();

private:
    QString      m_assignedRobotName;
    QVector<int> m_orders;
    int          m_destinationX;
    int          m_destinationY;
};

#endif // TASK_PRESENTER_H
