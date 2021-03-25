#include "Actors.h"
#include "DeliveryRobot.h"
#include "ObservableEnvironment.h"

Actors::Actors(const DeliveryRobot<ObservableNavEnvironment<Tile>,int>* model, QObject* parent)
    : MapItemPresenter(Actors::imagePath,
                       model->getBody()->getPose().getPosition().getPosY(),
                       model->getBody()->getPose().getPosition().getPosX(),
                       parent),
      mName(QString::fromStdString(model->getId())),
      mAction("IDLE"),
      mBatteryLevel(model->getBattery().getCharge()),
      mMoveCount(0),
      mOrientation(Direction::Up),
      model(model)
{
}

QString Actors::imagePath = "asd3.png";

QString Actors::name() const { return mName; }

void Actors::setName(const QString &name)
{
    if(mName == name)
        return;

    mName = name;
    emit nameChanged();
}

QString Actors::action() const { return mAction; }

void Actors::setAction(const QString &action)
{
    if(mAction == action)
        return;

    mAction = action;
    emit actionChanged();
}

int Actors::battery() const { return mBatteryLevel; }

void Actors::setBattery(int level)
{
    if(mBatteryLevel == level)
        return;

    mBatteryLevel = level;
    emit batteryChanged();
}

int Actors::moveCount() const { return mMoveCount; }

void Actors::MoveCountInc()
{
    ++mMoveCount;
    emit moveCountChanged();
}

Actors::Direction Actors::orientation() const { return mOrientation; }

void Actors::setOrientation(Actors::Direction dir)
{
    if(mOrientation == dir)
        return;

    mOrientation = dir;
    emit orientationChanged();
}
