#include "PodDockPresenter.h"

PodDockPresenter::PodDockPresenter(int rowCoord, int colCoord, QObject *parent)
    : MapItemPresenter(PodDockPresenter::imagePath, rowCoord, colCoord, parent)
{
}

PodDockPresenter* PodDockPresenter::read(QJsonObject &jsonObject, QObject *parent)
{
    if(jsonObject.contains("RowCoord") && jsonObject["RowCoord"].isDouble()
       && jsonObject.contains("ColCoord") && jsonObject["ColCoords"].isDouble())
    {
        int rowCoord = jsonObject["RowCoord"].toInt();
        int colCoord = jsonObject["ColCoord"].toInt();
        return new PodDockPresenter(rowCoord, colCoord, parent);
    }
}

QString PodDockPresenter::imagePath = "asd2.png";
