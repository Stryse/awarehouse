#ifndef ROAD_PRESENTER__H
#define ROAD_PRESENTER__H

#include <MapItemPresenter.h>
#include <QObject>
#include <QString>

class RoadPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit RoadPresenter(int rowCoord, int colCoord, QObject *parent = nullptr);
private:
    static QString imagePath;
};

#endif // ROAD_PRESENTER__H
