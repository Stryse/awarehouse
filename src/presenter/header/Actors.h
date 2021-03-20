#ifndef ACTORS_H
#define ACTORS_H

#include <QObject>

class Actors : public QObject
{
    Q_OBJECT

    Q_PROPERTY ( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY ( QString action READ action NOTIFY actionChanged )
    Q_PROPERTY ( int battery READ battery NOTIFY batteryChanged )
    Q_PROPERTY ( Role role READ role NOTIFY roleChanged )
    Q_PROPERTY ( int moveCount READ moveCount NOTIFY moveChanged )
    Q_PROPERTY ( Direction orientation READ orientation NOTIFY orientationChanged )
public:
    enum Role { Robot, Task };
    enum Direction { Up, Down, Left, Right };
    explicit Actors(QString name = "", QString action = "?\_(?)_/?", int battery = 100, Role role = Robot, Direction orientation = Up);
    Actors(Actors newActor);
    Q_ENUM( Role )
    Q_ENUM( Direction )

    QString name() const;
    void setName(const QString& name);

    QString action() const;
    void setAction(const QString& action);

    int battery() const;
    void setBattery(int level);

    Role role() const;
    void setRole(Role role);

    int moveCount() const;
    void MoveCountInc();

    Direction orientation() const;
    void setOrientation(Direction dir);

signals:
    void nameChanged();
    void actionChanged();
    void batteryChanged();
    void roleChanged();
    void moveCountChanged();
    void orientationChanged();

private:
    QString mName;
    QString mAction;
    int mBatteryLevel;
    Role mRole;
    int mMoveCount;
    Direction mOrientation;
};

#endif // ACTORS_H
