#include "RobotLoader.h"
#include "Point.h"

std::shared_ptr<DeliveryRobot<>> RobotLoader::load(const QJsonObject &robotObj, std::shared_ptr<ObservableNavEnvironment<>> &env)
{
    if (robotObj.contains("RowCoord") && robotObj["RowCoord"].isDouble() &&
        robotObj.contains("ColCoord") && robotObj["ColCoord"].isDouble() &&
        robotObj.contains("OrientationY") && robotObj["OrientationY"].isDouble())
    {
        Point<> position(robotObj["ColCoord"].toInt(),
                         robotObj["RowCoord"].toInt(),
                         0);

        DirectionVector<> orientation(0, robotObj["OrientationY"].toInt(), 0);

        return std::make_shared<DeliveryRobot<>>(env, position, orientation);
    }
    return nullptr;
}

QJsonObject RobotLoader::save(const DeliveryRobot<> &robotObj)
{
    return QJsonObject();
}