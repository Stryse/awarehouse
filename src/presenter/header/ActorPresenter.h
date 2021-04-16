#ifndef ACTOR_PRESENTER_H
#define ACTOR_PRESENTER_H

//Presenter
#include "MapItemPresenter.h"

class DeliveryRobot;
class PodDock;
class ObservableNavEnvironment;
class Tile;

class ActorPresenter : public MapItemPresenter
{
    Q_OBJECT

    Q_PROPERTY( QString name     READ name     WRITE setName NOTIFY nameChanged     )
    Q_PROPERTY( QString action   READ action                 NOTIFY actionChanged   )
    Q_PROPERTY(     int battery  READ battery                NOTIFY batteryChanged  )
    Q_PROPERTY(     int rotation READ rotation               NOTIFY rotationChanged )
    Q_PROPERTY(     int moves    READ moves                  NOTIFY movesChanged    )

public:
    explicit ActorPresenter(const DeliveryRobot* model,
                                  QObject* parent = nullptr);

    //Getter
    QString name()      const;
    QString action()    const;
    int     battery()   const;
    int     rotation()  const;
    int     moves()     const;

    //Setter
    void setName(const QString& name);
    void setAction(const QString& action);
    void setBattery(int level);
    void setRotation(int rotation);
    void movesInc();

signals:
    void nameChanged();
    void actionChanged();
    void batteryChanged();
    void rotationChanged();
    void movesChanged();

private:
    QString m_name;
    QString m_action;
    int     m_battery;
    int     m_rotation;
    int     m_moves;

    //Model
    const  DeliveryRobot* model;

    //Static
    static QString m_static_imagePath;
};

#endif // ACTOR_PRESENTER_H
