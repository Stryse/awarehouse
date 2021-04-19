#include "Settings.h"

Settings::Settings()
    : m_batteryLevel(100)
{ }

Settings::Settings(int batteryLevel)
    : m_batteryLevel(batteryLevel)
{ }

void Settings::setBatteryLevel(int batteryLevel)
{
    if(m_batteryLevel == batteryLevel)
        return;

    m_batteryLevel = batteryLevel;
    emit batteryChanged();
}

int Settings::batteryLevel() const { return m_batteryLevel; }
