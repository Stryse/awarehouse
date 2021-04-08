#include "RoadPresenter.h"

RoadPresenter::RoadPresenter(int rowCoord, int colCoord, QObject* parent)
    : MapItemPresenter(RoadPresenter::imagePath,rowCoord,colCoord,parent)
{}

QString RoadPresenter::imagePath = "qrc:/images/tile.png";
