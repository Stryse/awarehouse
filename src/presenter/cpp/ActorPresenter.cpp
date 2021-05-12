#include "ActorPresenter.h"

#include <cmath>
#include <QDebug>

//Model
#include "DeliveryRobot.h"
#include "ObservableEnvironment.h"
#include "Body.h"
#include "Tile.h"

QString ActorPresenter::m_static_imagePath = "qrc:/actorImg.png";

ActorPresenter::ActorPresenter(const DeliveryRobot* model,
                                           QObject* parent)
                                     
    : MapItemPresenter(model->getBody()->getPose().getPosition().getPosY(),
                       model->getBody()->getPose().getPosition().getPosX(),
                       ActorPresenter::m_static_imagePath,
                       parent)
    , m_name    (QString::fromStdString(model->getId()))
    , m_battery (model->getEnergySource()->getCharge())
    , m_rotation(static_cast<int>(std::atan2(model->getBody()->getPose().getOrientation().getY(),
                                             model->getBody()->getPose().getOrientation().getX())
                                             *180/M_PI) + 90)
    , m_moves(0)
    , m_energyUsed(0)
    , model(model)
{
    mapStatus(model->getMCU()->getStatus());
    model->getMoveMechanism()->onBodyMoved.connect([=](const Body& body){
        int row      = body.getPose().getPosition().getPosY();
        int column   = body.getPose().getPosition().getPosX();
        
        int rotateY  = body.getPose().getOrientation().getY();
        int rotateX  = body.getPose().getOrientation().getX();
        int rotation = (std::atan2(rotateY, rotateX)*180/M_PI) + 90;

        setRow(row);
        setColumn(column);
        setRotation(rotation);
    });

    model->getEnergySource()->onChargeChanged.connect([=](int energy){
        setBattery(energy);
        setEnergyUsed(1);
    });

    model->getMCU()->onStatusChanged.connect([=](const Status& status){
        mapStatus(status);
    });
}

ActorPresenter::ActorPresenter(int row, int column, QObject* parent)
    : MapItemPresenter(row,
                       column,
                       ActorPresenter::m_static_imagePath,
                       parent)
    , m_name      ("Anon")
    , m_action    ("\xc2\xaf\x5c\x5f\x28\xe3\x83\x84\x29\x5f\x2f\xc2\xaf")
    , m_battery   (100)
    , m_rotation  (0)
    , m_moves     (0)
    , m_energyUsed(0)
{}

bool ActorPresenter::operator==(const ActorPresenter& other) const
{
    return MapItemPresenter::operator==(other)     &&
           this->name()       == other.name()      &&
           this->action()     == other.action()    &&
           this->battery()    == other.battery()   &&
           this->rotation()   == other.rotation()  &&
           this->moves()      == other.moves()     &&
           this->energyUsed() == other.energyUsed();
}

ActorPresenter* ActorPresenter::loadJsonObject(const QJsonObject& actorObj,
                                                         QObject* parent)
{
    if (actorObj.contains("RowCoord")     && actorObj["RowCoord"].isDouble() &&
        actorObj.contains("ColCoord")     && actorObj["ColCoord"].isDouble() &&
        actorObj.contains("OrientationX") && actorObj["OrientationX"].isDouble() &&
        actorObj.contains("OrientationY") && actorObj["OrientationY"].isDouble())
    {
        int row      = actorObj["ColCoord"].toInt();
        int column   = actorObj["RowCoord"].toInt();
        int rotation = (std::atan2(actorObj["OrientationY"].toInt(), actorObj["OrientationX"].toInt())*180/M_PI) + 90;

        ActorPresenter* actor = new ActorPresenter(column, row, parent);
        actor->setRotation(rotation);

        return actor;
    }

    return nullptr;
}

QJsonObject ActorPresenter::saveJsonObject() const
{
    QJsonObject actorJsonObj;

    actorJsonObj.insert("RowCoord",     row());
    actorJsonObj.insert("ColCoord",     column());
    actorJsonObj.insert("OrientationX", std::round(std::cos((rotation()-90)*M_PI/180)));
    actorJsonObj.insert("OrientationY", std::round(std::sin((rotation()-90)*M_PI/180)));

    return actorJsonObj;
}

//Getter
QString ActorPresenter::name()       const { return m_name;       }
QString ActorPresenter::action()     const { return m_action;     }
int     ActorPresenter::battery()    const { return m_battery;    }
int     ActorPresenter::rotation()   const { return m_rotation;   }
int     ActorPresenter::moves()      const { return m_moves;      }
int     ActorPresenter::energyUsed() const { return m_energyUsed; }

//Setter
void ActorPresenter::setName(const QString& name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged();
    emit mapItemChanged();
}

void ActorPresenter::setAction(const QString& action)
{
    if (m_action == action)
        return;

    m_action = action;
    emit actionChanged();
    emit mapItemChanged();;
}

void ActorPresenter::setBattery(int level)
{
    if (m_battery == level)
        return;

    m_battery = level;
    emit batteryChanged();
    emit mapItemChanged();
}

void ActorPresenter::setRotation(int rotation)
{
    if (m_rotation == rotation)
        return;

    m_rotation = rotation;
    emit rotationChanged();
    emit mapItemChanged();
}

void ActorPresenter::setMoves(int moves)
{
    if (m_moves == moves)
        return;

    m_moves = moves;
    emit movesChanged();
    emit mapItemChanged();
}

void ActorPresenter::setEnergyUsed(int energy)
{
    m_energyUsed += energy;
    emit energyUsedChanged();
    emit mapItemChanged();
}

void ActorPresenter::mapStatus(const Status& status)
{
    switch (status)
    {
    case Status::IDLE:
        setAction("\xc2\xaf\x5c\x5f\x28\xe3\x83\x84\x29\x5f\x2f\xc2\xaf");
        break;

    case Status::RUNNING:
        setAction("RUNNING");
        break;

    case Status::CHARGING:
        setAction("CHARGING");
        break;

    case Status::ERROR:
        setAction("ERROR");
        break;    
    }
}