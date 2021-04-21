#ifndef TASK_PRESENTER_H
#define TASK_PRESENTER_H

#include <QObject>
#include <QPoint>
#include <QDateTime>

class Task;

class TaskPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(      QString assignedRobotName READ assignedRobotName NOTIFY assignedRobotNameChanged )
    Q_PROPERTY( QVector<int> orders            READ orders            NOTIFY ordersChanged            )
    Q_PROPERTY(          int podRow            READ podRow            NOTIFY podRowChanged            )
    Q_PROPERTY(          int podColumn         READ podColumn         NOTIFY podColumnChanged         )

public:
    explicit TaskPresenter(   Task* model,
                           QObject* parent = nullptr);

    explicit TaskPresenter(const TaskPresenter& task);

    TaskPresenter& operator=(const TaskPresenter& other);
    bool operator==(const TaskPresenter& other) const;

    //Getter
    QString      assignedRobotName() const;
    QVector<int> orders()            const;
    int          podRow()            const;
    int          podColumn()         const;

    //Setter
    void setAssignedRobotName(const QString& robotName);
    void setOrders(const QVector<int>& orders);
    void setPodRow(int podRow);
    void setPodColumn(int podColumn);

signals:
    void assignedRobotNameChanged();
    void ordersChanged();
    void podRowChanged();
    void podColumnChanged();

    void taskChanged();

private:
    QString      m_assignedRobotName;
    QVector<int> m_orders;
    int          m_podRow;
    int          m_podColumn;

    Task* model; //Const?
};

#endif // TASK_PRESENTER_H
