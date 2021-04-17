#ifndef SETTINGS__H
#define SETTINGS__H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int batterylevel READ batteryLevel WRITE setBatteryLevel NOTIFY batteryChanged)
public:
    Settings();
    Settings(int batteryLevel);

    //Getter
    int batteryLevel() const;

    //Setter
    void setBatteryLevel(int batteryLevel);

signals:
    void batteryChanged();

private:
    int m_batteryLevel;
};

#endif /* SETTINGS__H*/
