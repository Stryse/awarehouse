#include "TaskPresenter.h"
#include "Task.h"

TaskPresenter::TaskPresenter(   Task* model,
                             QObject* parent)
    : QObject(parent)
    , m_assignedRobotName(QString::fromStdString(model->getAssignedAgentID()))
    , m_orders           ({})
    , m_podRow           (model->getWayPoints().at(0).getPosX())
    , m_podColumn        (model->getWayPoints().at(0).getPosY())
    , model              (model)
{
    //Orders???
}

TaskPresenter::TaskPresenter(const TaskPresenter& task)
    : m_assignedRobotName(task.m_assignedRobotName)
    , m_orders           (task.m_orders           )
    , m_podRow           (task.m_podRow           )
    , m_podColumn        (task.m_podColumn        )
{}

TaskPresenter& TaskPresenter::operator=(const TaskPresenter& other)
{
    this->m_assignedRobotName = other.m_assignedRobotName;
    this->m_orders            = other.m_orders;
    this->m_podRow            = other.m_podRow;
    this->m_podColumn         = other.m_podColumn;

    return *this;
}

bool TaskPresenter::operator==(const TaskPresenter& other) const
{
    return this->m_assignedRobotName == other.m_assignedRobotName &&
           this->m_orders            == other.m_orders            &&
           this->m_podRow            == other.m_podRow            &&
           this->m_podColumn         == other.m_podColumn;
}

//Getter
QString      TaskPresenter::assignedRobotName() const { return m_assignedRobotName; }
QVector<int> TaskPresenter::orders()            const { return m_orders;            }
int          TaskPresenter::podRow()            const { return m_podRow;            }
int          TaskPresenter::podColumn()         const { return m_podColumn;         }

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

void TaskPresenter::setPodRow(int podRow)
{
    if(m_podRow == podRow)
        return;

    m_podRow = podRow;
    emit podRowChanged();
    emit taskChanged();
}

void TaskPresenter::setPodColumn(int podColumn)
{
    if(m_podColumn == podColumn)
        return;

    m_podColumn = podColumn;
    emit podColumnChanged();
    emit taskChanged();
}
