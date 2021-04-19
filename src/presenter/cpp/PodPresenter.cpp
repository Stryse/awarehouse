#include "PodPresenter.h"

//Model
#include "Pod.h"
#include "Body.h"

PodPresenter::PodPresenter(const Pod<OrderModel>* model,
                             QObject* parent)
    : MapItemPresenter(model->getBody()->getPose().getPosition().getPosY(),
                       model->getBody()->getPose().getPosition().getPosX(),
                       PodPresenter::m_static_imagePath,
                       parent)
    , model(model)
    , m_rotation(static_cast<int>(std::atan2(model->getBody()->getPose().getOrientation().getY(),
                                             model->getBody()->getPose().getOrientation().getX())
                                             *180/M_PI) + 90)
{
    //TODO
    int rotateY = model->getBody()->getPose().getOrientation().getY();
    int rotateX = model->getBody()->getPose().getOrientation().getX();

    model->onBodyMoved.connect([=](const Body& body){
        int row = body.getPose().getPosition().getPosY();
        int column = body.getPose().getPosition().getPosX();
        //TODO
        int rotateY = model->getBody()->getPose().getOrientation().getY();
        int rotateX = model->getBody()->getPose().getOrientation().getX();
        int rotation = (std::atan2(rotateY, rotateX)*180/M_PI) + 90;
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

