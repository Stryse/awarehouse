#include "Actors.h"

Actors::Actors(QString name, QString action, int battery, Actors::Role role) : mName(name), mAction(action), mBatteryLevel(battery), mRole(role)
{
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

