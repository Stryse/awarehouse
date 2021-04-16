#include "PodDockPresenter.h"

//Model
#include "PodDock.h"

QString PodDockPresenter::m_static_imagePath = "qrc:/images/pod.png";

PodDockPresenter::PodDockPresenter(const PodDock<ObservableNavEnvironment<Tile>>* model,
                                                                         QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       PodDockPresenter::m_static_imagePath,
                       parent)
    , model(model)
{}

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
