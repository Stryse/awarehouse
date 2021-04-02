#ifndef POD_DOCK_LOADER__H
#define POD_DOCK_LOADER__H

#include "ObservableEnvironment.h"
#include "PodDock.h"
#include "Tile.h"
#include <QJsonObject>
#include <memory>

class PodDockLoader
{
public:
    static std::shared_ptr<PodDock<>> load(const QJsonObject &podObj);
    static QJsonObject save(const PodDock<> &podObj);
};

#endif /* POD_DOCK_LOADER__H */