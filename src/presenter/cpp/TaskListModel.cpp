#include "TaskListModel.h"

//Presenter
#include "TaskPresenter.h"

QVector<int> TaskListModel::m_roles = { AssignedRobotNameRole,
                                        OrdersRole,
                                        PodRowRole,
                                        PodColumnRole };

TaskListModel::TaskListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_tasks(nullptr)
{}

int TaskListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()|| m_tasks == nullptr)
        return 0;

    return m_tasks->tasks()->size();
}

QVariant TaskListModel::data(const QModelIndex& index,
                                            int role) const
{
    if (!index.isValid() || m_tasks == nullptr)
       return QVariant();

    const TaskPresenter& task = *m_tasks->tasks()->at(index.row());

    switch(role) {
        case AssignedRobotNameRole:
            return QVariant(task.assignedRobotName());
        case OrdersRole:
            return QVariant::fromValue(task.orders());
        case PodRowRole:
            return QVariant(task.podRow());
        case PodColumnRole:
            return QVariant(task.podColumn());
    }

    return QVariant();
}

bool TaskListModel::setData(const QModelIndex& index,
                            const    QVariant& value,
                                           int role)
{
    if (m_tasks == nullptr)
        return false;

    TaskPresenter& task = *m_tasks->tasks()->at(index.row());

    switch(role) {
        case AssignedRobotNameRole:
            task.setAssignedRobotName(value.toString());
            break;
        case OrdersRole:
            {
                QList<QVariant> tempList = value.toList();
                QVector<int>    orders   = QVector<int>(tempList.size());

                for (int i = 0; i < orders.size(); ++i)
                    orders[i] = tempList[i].toInt();

                task.setOrders(orders);
                break;
            }
        case PodRowRole:
            task.setPodRow(value.toInt());
            break;
        case PodColumnRole:
            task.setPodColumn(value.toInt());
            break;
    }

    if (m_tasks->setTaskAt(index.row(), task))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TaskListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}


QHash<int, QByteArray> TaskListModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[AssignedRobotNameRole] = "assignedRobotName";
    names[OrdersRole]            = "orders";
    names[PodRowRole]            = "podRow";
    names[PodColumnRole]         = "podColumn";

    return names;
}

TaskList *TaskListModel::tasks() const { return m_tasks; }

void TaskListModel::setTasks(TaskList* tasks)
{
    beginResetModel();

    if (m_tasks)
        m_tasks->disconnect(this);

    m_tasks = tasks;

    if (m_tasks)
    {
        connect(m_tasks, &TaskList::preItemAppended,  this, [=]()
        {
            const int index = m_tasks->tasks()->size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_tasks, &TaskList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_tasks, &TaskList::preItemRemoved,   this, [=](int index)
        {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_tasks, &TaskList::postItemRemoved,  this, [=]()
        {
            endRemoveRows();
        });

        connect(m_tasks, &TaskList::dataChanged,      this, [=](int index)
        {
            emit dataChanged(QAbstractListModel::index(index), QAbstractListModel::index(index), m_roles);
        });
    }

    endResetModel();
}
