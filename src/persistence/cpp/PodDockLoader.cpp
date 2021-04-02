#include "PodDockLoader.h"
#include "Point.h"

std::shared_ptr<PodDock<>> PodDockLoader::load(const QJsonObject &podObj)
{
    if (podObj.contains("RowCoord") && podObj["RowCoord"].isDouble() &&
        podObj.contains("ColCoord") && podObj["ColCoord"].isDouble())
    {
        Point<> position(podObj["ColCoord"].toInt(),
                         podObj["RowCoord"].toInt(),
                         0);

        return std::make_shared<PodDock<>>(position);
    }
    return nullptr;
}
QJsonObject PodDockLoader::save(const PodDock<> &station)
{
    return QJsonObject();
}