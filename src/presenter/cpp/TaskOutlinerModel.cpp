#include "TaskOutlinerModel.h"

TaskOutlinerModel::TaskOutlinerModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_tasks(nullptr)
{}

int TaskOutlinerModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()|| m_tasks == nullptr)
        return 0;

    return m_tasks->tasks()->size();
}

QVariant TaskOutlinerModel::data(const QModelIndex& index,
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
        case DestinationXRole:
            return QVariant(task.destinationX());
        case DestinationYRole:
            return QVariant(task.destinationY());
    }

    return QVariant();
}

bool TaskOutlinerModel::setData(const QModelIndex& index,
                                const    QVariant& value,
                                               int role)
{
    if (data(index, role) != value)
    {
        // TODO IMPLEMENT
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags TaskOutlinerModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}


QHash<int, QByteArray> TaskOutlinerModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[AssignedRobotNameRole] = "assignedRobotName";
    names[OrdersRole]            = "orders";
    names[DestinationXRole]      = "destinationX";
    names[DestinationYRole]      = "destinationY";

    return names;
}

TaskList *TaskOutlinerModel::tasks() const { return m_tasks; }

void TaskOutlinerModel::setTasks(TaskList* tasks)
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
    }

    endResetModel();
}
