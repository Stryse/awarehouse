#ifndef ACTOR_LIST_MODEL__H
#define ACTOR_LIST_MODEL__H

#include <QObject>
#include <QAbstractListModel>

//Presenter
#include "ActorPresenter.h"

class ActorListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ActorListModel(QObject* parent = nullptr){}
    virtual ~ActorListModel() {}

//QAbstractItemModel interface
public:
    int      rowCount(const QModelIndex& parent)                            const override {}
    QVariant data    (const QModelIndex& index, int role = Qt::DisplayRole) const override {}


    //Getter


private:
    QList<const ActorPresenter*> m_actors;
};

#endif //ACTOR_LIST_MODEL__H
