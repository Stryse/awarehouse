#include "RobotLoader.h"
#include "Point.h"

std::shared_ptr<DeliveryRobot> RobotLoader::load(const QJsonObject &robotObj, std::shared_ptr<ObservableNavEnvironment> &env)
{
    if (robotObj.contains("RowCoord") && robotObj["RowCoord"].isDouble() &&
        robotObj.contains("ColCoord") && robotObj["ColCoord"].isDouble() &&
        robotObj.contains("OrientationY") && robotObj["OrientationY"].isDouble())
    {
        Point<> position(robotObj["ColCoord"].toInt(),
                         robotObj["RowCoord"].toInt(),
                         0);

        DirectionVector<> orientation(0, robotObj["OrientationY"].toInt(), 0);

        return std::make_shared<DeliveryRobot>(env, position, orientation, 100);
    }
    return nullptr;
}

QJsonObject RobotLoader::save(const DeliveryRobot &robot)
{
    QJsonObject robotObject;
    robotObject.insert("RowCoord", robot.getBody()->getPose().getPosition().getPosY());
    robotObject.insert("ColCoord", robot.getBody()->getPose().getPosition().getPosX());
    robotObject.insert("OrientationY", robot.getBody()->getPose().getOrientation().getY());
    return robotObject;
}
