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

    QList<const ActorPresenter*>* actors();

    bool setActorAt(int index, const ActorPresenter& actor);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendActor(const ActorPresenter& actor);
    void removeActor(int index);

    void clear();

public:
    QList<const ActorPresenter*> m_actors;
};

#endif /* ACTOR_LIST__H */
