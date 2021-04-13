#include "TaskPresenter.h"

TaskPresenter::TaskPresenter(QString      assignedRobotName,
                             QVector<int> orders,
                             int          destinationX,
                             int          destinationY)
    : m_assignedRobotName(assignedRobotName)
    , m_orders           (orders           )
    , m_destinationX     (destinationX     )
    , m_destinationY     (destinationY     )
{}

TaskPresenter::TaskPresenter(const TaskPresenter& task)
    : m_assignedRobotName(task.m_assignedRobotName)
    , m_orders           (task.m_orders           )
    , m_destinationX     (task.m_destinationX     )
    , m_destinationY     (task.m_destinationY     )
{}

TaskPresenter& TaskPresenter::operator=(const TaskPresenter& other)
{
    this->m_assignedRobotName = other.m_assignedRobotName;
    this->m_orders            = other.m_orders;
    this->m_destinationX      = other.m_destinationX;
    this->m_destinationY      = other.m_destinationY;
    return *this;
}

bool operator==(TaskPresenter* const lhs, const TaskPresenter rhs)
{
    return lhs->m_assignedRobotName    == rhs.m_assignedRobotName &&
                   lhs->m_orders       == rhs.m_orders            &&
                   lhs->m_destinationX == rhs.m_destinationX      &&
                   lhs->m_destinationY == rhs.m_destinationY;
}

//Getter
QString      TaskPresenter::assignedRobotName() const { return m_assignedRobotName; }
QVector<int> TaskPresenter::orders()            const { return m_orders;            }
int          TaskPresenter::destinationX()      const { return m_destinationX;      }
int          TaskPresenter::destinationY()      const { return m_destinationY;      }

//Setter
void TaskPresenter::setAssignedRobotName(const QString& robotName)
{
    if(m_assignedRobotName == robotName)
        return;

    m_assignedRobotName = robotName;
    emit assignedRobotNameChanged();
}

void TaskPresenter::setOrders(const QVector<int>& orders)
{
    if (m_orders == orders)
        return;

    m_orders = orders;
    emit ordersChanged();
}

void TaskPresenter::setDestinationX(int destinationX)
{
    if(m_destinationX == destinationX)
        return;

    m_destinationX = destinationX;
    emit destinationXChanged();
}

void TaskPresenter::setDestinationY(int destinationY)
{
    if(m_destinationY == destinationY)
        return;

    m_destinationY = destinationY;
    emit destinationYChanged();
}
