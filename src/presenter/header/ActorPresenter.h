#ifndef ACTOR_PRESENTER_H
#define ACTOR_PRESENTER_H

//Presenter
#include "MapItemPresenter.h"

template <typename E, typename EN> class DeliveryRobot;
template <typename E>              class PodDock;
template <typename V>              class ObservableNavEnvironment;
                                   class Tile;

class ActorPresenter : public MapItemPresenter
{
    Q_OBJECT

    Q_PROPERTY( QString name     READ name     WRITE setName NOTIFY nameChanged     )
    Q_PROPERTY( QString action   READ action                 NOTIFY actionChanged   )
    Q_PROPERTY(     int battery  READ battery                NOTIFY batteryChanged  )
    Q_PROPERTY(     int rotation READ rotation               NOTIFY rotationChanged )
    Q_PROPERTY(     int moves    READ moves                  NOTIFY movesChanged    )
    Q_PROPERTY( int     row2   READ getRow2  WRITE setRow2  NOTIFY rowCoordChanged  )
    Q_PROPERTY( int     col2   READ getCol2  WRITE setCol2  NOTIFY colCoordChanged  )

public:
    explicit ActorPresenter(const DeliveryRobot<ObservableNavEnvironment<Tile>,int>* model,
                                                                            QObject* parent);

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

    int getRow2() const { return rowCoord(); }
    void setRow2(int row2) {}

    int getCol2() const { return 5; }
    void setCol2(int col2) {}

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
    const  DeliveryRobot<ObservableNavEnvironment<Tile>,int>* model;

    //Static
    static QString imagePath;
};

#endif // ACTOR_PRESENTER_H
