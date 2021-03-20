#include "Actors.h"

Actors::Actors(QString name, QString action, int battery, Actors::Role role, Actors::Direction orientation) :
    mName(name), mAction(action), mBatteryLevel(battery), mRole(role), mMoveCount(0), mOrientation(orientation)
{
}

Actors::Actors(const Actors &newActor)
{
    mName = newActor.mName;
    mAction = newActor.mAction;
    mBatteryLevel = newActor.mBatteryLevel;
    mRole = newActor.mRole;
    mMoveCount = newActor.mMoveCount;
    mOrientation = newActor.mOrientation;
}

QString Actors::name() const
{
    return mName;
}

void Actors::setName(const QString &name)
{
    if(mName == name)
        return;

    mName = name;
    emit nameChanged();
}

QString Actors::action() const
{
    return mAction;
}

void Actors::setAction(const QString &action)
{
    if(mAction == action)
        return;

    mAction = action;
    emit actionChanged();
}

int Actors::battery() const
{
    return mBatteryLevel;
}

void Actors::setBattery(int level)
{
    if(mBatteryLevel == level)
        return;

    mBatteryLevel = level;
    emit batteryChanged();
}

Actors::Role Actors::role() const
{
    return mRole;
}

void Actors::setRole(Actors::Role role)
{
    if(mRole == role)
        return;

    mRole = role;
    emit roleChanged();
}

int Actors::moveCount() const
{
    return mMoveCount;
}

void Actors::MoveCountInc()
{
    ++mMoveCount;
    emit moveCountChanged();
}

Actors::Direction Actors::orientation() const
{
    return mOrientation;
}

void Actors::setOrientation(Actors::Direction dir)
{
    if(mOrientation == dir)
        return;

    mOrientation = dir;
    emit orientationChanged();
}

