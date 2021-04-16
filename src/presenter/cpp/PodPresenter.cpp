#include "PodPresenter.h"

//Model
#include "DeliveryStation.h"
#include "Body.h"

PodPresenter::PodPresenter(const Pod* model,
                             QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       PodPresenter::m_static_imagePath,
                       parent)
    , model(model)
{
    //TODO
    int rotateY = model->getBody().getPosition().getPose().getOrientation().getY();
    int rotateX = model->getBody().getPosition().getPose().getOrientation().getX();
    m_rotation = (((std::atan2(rotateY, rotateX)*180)/M_PI));

    model->onBodyMoved.connect([=](const Body& body){
        int row = body.getPose().getPosition().getPosY();
        int column = body.getPose().getPosition().getPosX();
        //TODO
        int rotateY = model->getPosition->getPose().getOrientation().getY();
        int rotateX = model->getPosition->getPose().getOrientation().getX();
        m_rotation = (((std::atan2(rotateY, rotateX)*180)/M_PI));
        setRow(row);
        setColumn(column);
        setRotation(rotation);
    });
}

QString PodPresenter::m_static_imagePath = "qrc:/placeholder_amogus.png";

int PodPresenter::rotation() const { return m_rotation; }

void PodPresenter::setRotation(int rotation)
{
    if(m_rotation == rotation)
        return;

    m_rotation = rotation;
    emit rotationChanged();
}

