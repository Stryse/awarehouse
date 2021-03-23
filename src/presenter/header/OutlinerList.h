#ifndef OUTLINER_LIST__H
#define OUTLINER_LIST__H

#include <QObject>
#include <QList>
#include "Actors.h"

class ActorOutlinerList : public QObject
{
    Q_OBJECT

public:
    explicit ActorOutlinerList(QObject *parent = nullptr);

    QList<Actors> actors() const;
    bool setActorAt(int index, const Actors& actor);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendActor(const Actors& actor);
    void removeActor(int index);
    void clear();

public:
    QList<Actors> mActors;
};

#endif /* OUTLINER_LIST__H */