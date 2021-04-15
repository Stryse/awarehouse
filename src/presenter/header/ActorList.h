#ifndef ACTOR_LIST__H
#define ACTOR_LIST__H

#include <QObject>
#include <QList>

//Presenter
#include "ActorPresenter.h"

class ActorList : public QObject
{
    Q_OBJECT

public:
    explicit ActorList(QObject* parent = nullptr);

    QList<ActorPresenter*>* actors();

    bool setActorAt(int index, ActorPresenter& actor);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendActor(ActorPresenter& actor);
    void removeActor(int index);
    void removeActor(int row, int column);

    void clear();

public:
    QList<ActorPresenter*> m_actors;
};

#endif /* ACTOR_LIST__H */
