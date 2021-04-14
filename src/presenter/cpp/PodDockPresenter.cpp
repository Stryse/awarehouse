#include "PodDockPresenter.h"

//Model
#include "PodDock.h"

PodDockPresenter::PodDockPresenter(const PodDock<ObservableNavEnvironment<Tile>>* model,
                                                                         QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       PodDockPresenter::imagePath,
                       parent)
    , model(model)
{}

QString PodDockPresenter::imagePath = "qrc:/images/pod.png";
