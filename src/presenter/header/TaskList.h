#ifndef TASK_LIST__H
#define TASK_LIST__H

#include <QList>
#include <QObject>

//Presenter
#include "TaskPresenter.h"

class TaskList : public QObject
{
    Q_OBJECT

public:
    explicit TaskList(QObject* parent = nullptr);

    QList<const TaskPresenter*>* tasks();

    bool setTaskAt(int index, const TaskPresenter& task);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendOrder(const TaskPresenter& task);
    void removeOrder(int index);

    void clear();

public:
    QList<const TaskPresenter*> m_tasks;
};

#endif /* TASK_LIST__H */
