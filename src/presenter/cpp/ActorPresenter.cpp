#include "ActorPresenter.h"

//Model
#include "DeliveryRobot.h"
#include "ObservableEnvironment.h"

ActorPresenter::ActorPresenter(const DeliveryRobot<ObservableNavEnvironment<Tile>, int>* model,
                                                                                QObject* parent)
    : MapItemPresenter(model->getBody()->getPose().getPosition().getPosY(),
                       model->getBody()->getPose().getPosition().getPosX(),
                       ActorPresenter::m_static_imagePath,
                       parent)
    , m_name    (QString::fromStdString(model->getId()))
    , m_action  ("\xc2\xaf\x5c\x5f\x28\xe3\x83\x84\x29\x5f\x2f\xc2\xaf")
    , m_battery (model->getEnergySource()->getCharge())
    , m_rotation(0)
    , m_moves   (0)
    , model     (model)
{}

ActorPresenter::ActorPresenter(int row, int column, QObject* parent)
    : MapItemPresenter(row,
                       column,
                       ActorPresenter::m_static_imagePath,
                       parent)
    , m_name    ("Jojo")
    , m_action  ("\xc2\xaf\x5c\x5f\x28\xe3\x83\x84\x29\x5f\x2f\xc2\xaf")
    , m_battery (100)
    , m_rotation(0)
    , m_moves   (0)
{}

QString ActorPresenter::m_static_imagePath = "qrc:/images/robot.png";

//Getter
QString ActorPresenter::name()     const { return m_name;     }
QString ActorPresenter::action()   const { return m_action;   }
int     ActorPresenter::battery()  const { return m_battery;  }
int     ActorPresenter::rotation() const { return m_rotation; }
int     ActorPresenter::moves()    const { return m_moves;    }

//Setter
void ActorPresenter::setName(const QString& name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged();
}

void ActorPresenter::setAction(const QString& action)
{
    if (m_action == action)
        return;

    m_action = action;
    emit actionChanged();
}

void ActorPresenter::setBattery(int level)
{
    if (m_battery == level)
        return;

    m_battery = level;
    emit batteryChanged();
}

void ActorPresenter::setRotation(int rotation)
{
    if (m_rotation == rotation)
        return;

    m_rotation = rotation;
    emit rotationChanged();
}

void ActorPresenter::setMoves(int moves)
{
    if (m_moves == moves)
        return;

    m_moves = moves;
    emit movesChanged();
}
