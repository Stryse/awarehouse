#ifndef ACTOR_PRESENTER_H
#define ACTOR_PRESENTER_H

#include <QJsonObject>

//Presenter
#include "MapItemPresenter.h"

class DeliveryRobot;
class PodDock;
class ObservableNavEnvironment;
class Tile;

class ActorPresenter : public MapItemPresenter
{
    Q_OBJECT

    Q_PROPERTY( QString name       READ name       WRITE setName       NOTIFY nameChanged     )
    Q_PROPERTY( QString action     READ action     WRITE setAction     NOTIFY actionChanged   )
    Q_PROPERTY(     int battery    READ battery    WRITE setBattery    NOTIFY batteryChanged  )
    Q_PROPERTY(     int rotation   READ rotation   WRITE setRotation   NOTIFY rotationChanged )
    Q_PROPERTY(     int moves      READ moves      WRITE setMoves      NOTIFY movesChanged    )
    Q_PROPERTY(     int energyUsed READ energyUsed WRITE setEnergyUsed NOTIFY energyUsedChanged)
public:
    explicit ActorPresenter(const DeliveryRobot* model,
                                        QObject* parent = nullptr);
    explicit ActorPresenter(int row, int column, QObject* parent = nullptr);

    bool operator==(const ActorPresenter& other) const;

    static ActorPresenter* loadJsonObject(const QJsonObject& actorObj,
                                                    QObject* parent = nullptr);
    QJsonObject saveJsonObject() const;

    //Getter
    QString name()       const;
    QString action()     const;
    int     battery()    const;
    int     rotation()   const;
    int     moves()      const;
    int     energyUsed() const;

    //Setter
    void setName(const QString& name);
    void setAction(const QString& action);
    void setBattery(int level);
    void setRotation(int rotation);
    void setMoves(int moves);
    void setEnergyUsed(int energy);

signals:
    void nameChanged();
    void actionChanged();
    void batteryChanged();
    void rotationChanged();
    void movesChanged();
    void energyUsedChanged();

private:
    QString m_name;
    QString m_action;
    int     m_battery;
    int     m_rotation;
    int     m_moves;
    int     m_energyUsed;

    //Model
    const  DeliveryRobot* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* ACTOR_PRESENTER_H */
