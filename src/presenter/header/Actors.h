#ifndef ACTORS_H
#define ACTORS_H

#include <QObject>

class Actors : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )
    Q_PROPERTY( QString action READ action NOTIFY actionChanged )
    Q_PROPERTY( int battery READ battery NOTIFY batteryChanged )
    Q_PROPERTY( Role role READ role NOTIFY roleChanged )

public:
    enum Role { Robot, Pod, Station, Charger };
    explicit Actors(QString name = "", QString action = "?\_(?)_/?", int battery = 100, Role role = Robot);
    Q_ENUM( Role )

    QString name() const;
    void setName(const QString& name);

    QString action() const;
    void setAction(const QString& action);

    int battery() const;
    void setBattery(int level);

    Role role() const;
    void setRole(Role role);

signals:
    void nameChanged();
    void actionChanged();
    void batteryChanged();
    void roleChanged();

private:
    QString mName;
    QString mAction;
    int mBatteryLevel;
    Role mRole;
};

#endif // ACTORS_H
