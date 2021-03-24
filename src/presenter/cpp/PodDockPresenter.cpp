#include "PodDockPresenter.h"
#include "PodDock.h"

PodDockPresenter::PodDockPresenter(const PodDock<ObservableNavEnvironment<Tile>>* model, QObject *parent)
    : MapItemPresenter(PodDockPresenter::imagePath,
                       model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       parent)
    ,model(model)
{

}

QString PodDockPresenter::imagePath = "qrc:/images/pod.png";
