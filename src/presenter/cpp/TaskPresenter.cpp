#include "TaskPresenter.h"
#include "Task.h"

TaskPresenter::TaskPresenter(Task* task,
                             QObject* parent)
    : QObject(parent)
    , model(task)
    , m_assignedRobotName("Jani")
    , m_orders           ({})
    , m_destinationX     (0)
    , m_destinationY     (0)
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

bool TaskPresenter::operator==(const TaskPresenter& other) const
{
    return this->m_assignedRobotName == other.m_assignedRobotName &&
           this->m_orders            == other.m_orders            &&
           this->m_destinationX      == other.m_destinationX      &&
           this->m_destinationY      == other.m_destinationY;
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
    emit taskChanged();
}

void TaskPresenter::setOrders(const QVector<int>& orders)
{
    if (m_orders == orders)
        return;

    m_orders = orders;
    emit ordersChanged();
    emit taskChanged();
}

void TaskPresenter::setDestinationX(int destinationX)
{
    if(m_destinationX == destinationX)
        return;

    m_destinationX = destinationX;
    emit destinationXChanged();
    emit taskChanged();
}

void TaskPresenter::setDestinationY(int destinationY)
{
    if(m_destinationY == destinationY)
        return;

    m_destinationY = destinationY;
    emit destinationYChanged();
    emit taskChanged();
}
