#ifndef ACTOR_LIST__H
#define ACTOR_LIST__H

#include <QObject>
#include <QList>
#include <QJsonArray>

//Presenter
#include "ActorPresenter.h"

/******************************************************************
 * @brief Decorator of a QList which broadcast manipulation events.
 ******************************************************************/
class ActorList : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int count     READ count     NOTIFY countChanged     )
    Q_PROPERTY( int sumMoves  READ sumMoves  NOTIFY sumMovesChanged  )
    Q_PROPERTY( int sumEnergy READ sumEnergy NOTIFY sumEnergyChanged )

public:
    explicit ActorList(QObject* parent = nullptr);

    QList<ActorPresenter*>* actors();
    int                     count();
    int                     sumMoves();
    int                     sumEnergy();

    bool setActorAt(int index, ActorPresenter& actor);

    void       loadJsonArray(const QJsonArray& actorsJson);
    QJsonArray saveJsonArray() const;

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void dataChanged(int index);

    void countChanged(int count);
    void sumMovesChanged();
    void sumEnergyChanged();

public slots:
    void appendActor(ActorPresenter& actor);
    void removeActor(int index);
    void removeActor(int row, int column);

    void clear();

public:
    QList<ActorPresenter*> m_actors;
};

#endif /* ACTOR_LIST__H */
