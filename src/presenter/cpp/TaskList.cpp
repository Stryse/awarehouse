#include "TaskList.h"

TaskList::TaskList(QObject* parent)
    : QObject(parent)
{
}

QList<TaskPresenter*>* TaskList::tasks() { return &m_tasks; }

bool TaskList::setTaskAt(int index, TaskPresenter& task)
{
    if (index < 0 ||
        index >= m_tasks.size())
        return false;

    const TaskPresenter* oldTask = m_tasks.at(index);
    if (task == *oldTask)
        return false;

    m_tasks[index] = &task;
    return true;
}

void TaskList::appendOrder(TaskPresenter& task)
{
    emit preItemAppended();
    m_tasks.append(&task);
    emit postItemAppended();
}

void TaskList::removeOrder(int index)
{
    if (index < 0 ||
        index >= m_tasks.size())
        return;

    emit preItemRemoved(index);
    m_tasks.removeAt(index);
    emit postItemRemoved();
}

void TaskList::clear()
{
    emit preItemRemoved(0);
    m_tasks.clear();
    emit postItemRemoved();
}
