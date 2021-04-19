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
    emit dataChanged(index);
    return true;
}

void TaskList::appendTask(TaskPresenter& task)
{
    emit preItemAppended();

    int last = m_tasks.size();
    m_tasks.append(&task);
    connect(&task, &TaskPresenter::taskChanged, this, [=]()
    {
        emit dataChanged(last);
    });

    emit postItemAppended();
}

void TaskList::removeTask(int index)
{
    if (index < 0 ||
        index >= m_tasks.size())
        return;

    emit preItemRemoved(index);

    m_tasks[index]->disconnect(this);
    m_tasks.removeAt(index);
    for (int i = index; i < m_tasks.size(); ++i)
    {
        m_tasks[i]->disconnect(this);
        connect(m_tasks[i], &TaskPresenter::taskChanged, this, [=]()
        {
            emit dataChanged(i);
        });
    }

    emit postItemRemoved();
}

void TaskList::clear()
{
    if (m_tasks.size() == 0)
        return;

    for (int i = m_tasks.size() - 1; i >= 0; --i)
        removeTask(i);
}
