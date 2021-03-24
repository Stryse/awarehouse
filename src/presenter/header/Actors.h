#ifndef ACTORS_H
#define ACTORS_H

#include "MapItemPresenter.h"

template <typename E, typename EN> class DeliveryRobot;
template <typename E> class PodDock;
template <typename V> class ObservableNavEnvironment;
class Tile;

class Actors : public MapItemPresenter
{
    Q_OBJECT

    Q_PROPERTY ( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY ( QString action READ action NOTIFY actionChanged )
    Q_PROPERTY ( int battery READ battery NOTIFY batteryChanged )
    Q_PROPERTY ( int moveCount READ moveCount NOTIFY moveCountChanged )
    Q_PROPERTY ( Direction orientation READ orientation NOTIFY orientationChanged )
public:
    explicit Actors(const DeliveryRobot<ObservableNavEnvironment<Tile>,int>* model,
                    QObject* parent);

    //Enum
    enum Direction { Up, Down, Left, Right };
    Q_ENUM( Direction )

    //Getter setter
    QString name() const;
    void setName(const QString& name);

    QString action() const;
    void setAction(const QString& action);

    int battery() const;
    void setBattery(int level);

    int moveCount() const;
    void MoveCountInc();

    Direction orientation() const;
    void setOrientation(Direction dir);

    // Operators
    friend bool operator==(Actors* const lhs, const Actors rhs)
    {
        return lhs->mName == rhs.mName &&
               lhs->mAction == rhs.mAction &&
               lhs->mBatteryLevel == rhs.mBatteryLevel &&
               lhs->mMoveCount == rhs.mMoveCount &&
               lhs->mOrientation == rhs.mOrientation;
    }

    Actors& operator=(const Actors& other);

signals:
    void nameChanged();
    void actionChanged();
    void batteryChanged();
    void moveCountChanged();
    void orientationChanged();

private:
    QString mName;
    QString mAction;
    int mBatteryLevel;
    int mMoveCount;
    Direction mOrientation;

    // Static
    static QString imagePath;
};

#endif // ACTORS_H
