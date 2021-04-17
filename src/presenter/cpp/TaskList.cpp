#include "TaskList.h"

TaskList::TaskList(QObject* parent)
    : QObject(parent)
{
    m_tasks.append(new TaskPresenter("amogus", {1,2,3}, 3, 5, this));
    m_tasks.append(new TaskPresenter("sus", {4}, 0, 7, this));
    m_tasks.append(new TaskPresenter("lajos", {2,4}, 2, 7, this));
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
    emit preItemRemoved(0);
    m_tasks.clear();
    emit postItemRemoved();
}
