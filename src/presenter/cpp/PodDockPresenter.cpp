#include "PodDockPresenter.h"

//Model
#include "PodDock.h"

QString PodDockPresenter::m_static_imagePath = "qrc:/images/pod.png";

PodDockPresenter::PodDockPresenter(const PodDock* model,
                                         QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       PodDockPresenter::m_static_imagePath,
                       parent),
      model(model)
{
}

PodDockPresenter::PodDockPresenter(int row, int column, QObject* parent)
    : MapItemPresenter(row,
                       column,
                       PodDockPresenter::m_static_imagePath,
                       parent)
{}

bool PodDockPresenter::operator==(const PodDockPresenter& other) const
{
    return MapItemPresenter::operator==(other);
}

PodDockPresenter* PodDockPresenter::loadJsonObject(const QJsonObject& podDockObj,
                                                             QObject* parent)
{
    if (podDockObj.contains("RowCoord") && podDockObj["RowCoord"].isDouble() &&
        podDockObj.contains("ColCoord") && podDockObj["ColCoord"].isDouble())
    {
        int row    = podDockObj["ColCoord"].toInt();
        int column = podDockObj["RowCoord"].toInt();

        return new PodDockPresenter(column, row, parent);
    }

    return nullptr;
}

QJsonObject PodDockPresenter::saveJsonObject() const
{
    QJsonObject podDockJsonObj;

    podDockJsonObj.insert("RowCoord", row());
    podDockJsonObj.insert("ColCoord", column());

    //TODO: Save orders

    return podDockJsonObj;
}
