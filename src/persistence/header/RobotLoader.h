#ifndef ROBOT_LOADER__H
#define ROBOT_LOADER__H

#include "DeliveryRobot.h"
#include "ObservableEnvironment.h"
#include "Tile.h"
#include <QJsonObject>
#include <memory>

class RobotLoader
{
public:
    static std::shared_ptr<DeliveryRobot<>> load(const QJsonObject &robotObj, std::shared_ptr<ObservableNavEnvironment<>> &env);
    static QJsonObject save(const DeliveryRobot<> &robot);
};

#endif /* ROBOT_LOADER__H */
