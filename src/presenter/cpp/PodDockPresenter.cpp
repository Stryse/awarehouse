#include "PodDockPresenter.h"

//Model
#include "PodDock.h"

PodDockPresenter::PodDockPresenter(const PodDock<ObservableNavEnvironment>* model,
                                                                         QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       PodDockPresenter::m_static_imagePath,
                       parent)
    , model(model)
{}

QString PodDockPresenter::m_static_imagePath = "qrc:/images/pod.png";
