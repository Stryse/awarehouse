#ifndef ROBOT_LOADER__H
#define ROBOT_LOADER__H

#include "DeliveryRobot.h"
#include "ObservableEnvironment.h"
#include "Tile.h"
#include <QJsonObject>
#include <memory>

class Settings;

/****************************************************************
 * @brief Loads and saves a Robot from a Json Object.
 ****************************************************************/
class RobotLoader
{
public:
    static std::shared_ptr<DeliveryRobot> load(const QJsonObject &robotObj, std::shared_ptr<ObservableNavEnvironment> &env, const Settings* = nullptr);
    static QJsonObject save(const DeliveryRobot &robot);
};

#endif /* ROBOT_LOADER__H */
