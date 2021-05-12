#ifndef SETTINGS__H
#define SETTINGS__H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int batteryLevel READ batteryLevel WRITE setBatteryLevel NOTIFY batteryLevelChanged)

public:
    Settings();
    Settings(int batteryLevel);

    //Getter
    int batteryLevel() const;

    //Setter
    void setBatteryLevel(int batteryLevel);

signals:
    void batteryLevelChanged(int batteryLevel);

private:
    int m_batteryLevel;
};

#endif /* SETTINGS__H*/
