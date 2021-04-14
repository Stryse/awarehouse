#include "RoadPresenter.h"

RoadPresenter::RoadPresenter(     int rowCoord,
                                  int colCoord,
                             QObject* parent)
    : MapItemPresenter(rowCoord,
                       colCoord,
                       RoadPresenter::imagePath,
                       parent)
{}

QString RoadPresenter::imagePath = "qrc:/images/tile.png";
