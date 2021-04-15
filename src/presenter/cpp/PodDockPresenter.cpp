#include "PodDockPresenter.h"

//Model
#include "PodDock.h"

PodDockPresenter::PodDockPresenter(const PodDock<ObservableNavEnvironment<Tile>>* model,
                                                                         QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       PodDockPresenter::m_static_imagePath,
                       parent)
    , model(model)
{}

PodDockPresenter::PodDockPresenter(QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       PodDockPresenter::m_static_imagePath,
                       parent)
{}

QString PodDockPresenter::m_static_imagePath = "qrc:/images/pod.png";
