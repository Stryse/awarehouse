#ifndef TASK_LIST_MODEL__H
#define TASK_LIST_MODEL__H

#include <QObject>
#include <QAbstractListModel>
#include <QList>

//Presenter
#include "TaskList.h"

class TaskListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( TaskList* tasks READ tasks WRITE setTasks )

public:
    explicit TaskListModel(QObject* parent = nullptr);

    enum
    {
        AssignedRobotNameRole = Qt::UserRole,
        OrdersRole,
        DestinationXRole,
        DestinationYRole
    };

//QAbstractItemModel interface
public:
    int      rowCount(const QModelIndex& parent)                        const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    //Editable
    bool setData(const QModelIndex& index,
                 const    QVariant& value,
                                int role = Qt::EditRole) override;

    Qt::ItemFlags          flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames()                     const override;

    //Getter
    TaskList* tasks() const;

    //Setter
    void setTasks(TaskList* tasks);

private:
    TaskList* m_tasks;

    static QVector<int> m_roles;
};

#endif /* TASK_LIST_MODEL__H */
