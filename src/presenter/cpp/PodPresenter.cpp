#include "PodPresenter.h"

#include <QJsonArray>

//Model
#include "Pod.h"
#include "Body.h"

QString PodPresenter::m_static_imagePath = "qrc:/podImg.png";

PodPresenter::PodPresenter(const Pod<OrderModel>* model,
                                         QObject* parent)
    : MapItemPresenter(model->getBody()->getPose().getPosition().getPosY(),
                       model->getBody()->getPose().getPosition().getPosX(),
                       PodPresenter::m_static_imagePath,
                       parent)
    , m_rotation(static_cast<int>(std::atan2(model->getBody()->getPose().getOrientation().getY(),
                                             model->getBody()->getPose().getOrientation().getX())
                                             *180/M_PI) + 90)
    , m_orders(QStringListModel(getOrders(model)))
    , model(model)
{
    model->onBodyMoved.connect([=](const Body& body){
        int row      = body.getPose().getPosition().getPosY();
        int column   = body.getPose().getPosition().getPosX();
        //TODO
        int rotateY  = model->getBody()->getPose().getOrientation().getY();
        int rotateX  = model->getBody()->getPose().getOrientation().getX();
        int rotation = (std::atan2(rotateY, rotateX)*180/M_PI) + 90;

        QStringList orders = getOrders(model);

        setRow(row);
        setColumn(column);
        setOrders(orders);
        setRotation(rotation);
    });
}

PodPresenter::PodPresenter(int      row,
                           int      column,
                           QObject* parent)
    : MapItemPresenter(row,
                       column,
                       PodPresenter::m_static_imagePath,
                       parent)
    , m_rotation(0)
{}

PodPresenter* PodPresenter::loadJsonObject(const QJsonObject& podJsonObj,
                                                     QObject* parent)
{
    if (podJsonObj.contains("RowCoord") && podJsonObj["RowCoord"].isDouble() &&
        podJsonObj.contains("ColCoord") && podJsonObj["ColCoord"].isDouble())
    {
        int row    = podJsonObj["ColCoord"].toInt();
        int column = podJsonObj["RowCoord"].toInt();

        PodPresenter* pod = new PodPresenter(column, row, parent);

        if (podJsonObj.contains("Orders") && podJsonObj["Orders"].isArray())
        {
            QStringList orders;

            QJsonArray ordersJsonArray = podJsonObj["Orders"].toArray();
            for (int i = 0; i < ordersJsonArray.size(); ++i)
                if (ordersJsonArray[i].toObject().contains("Category") && ordersJsonArray[i].toObject()["Category"].isDouble())
                    orders.append(QString::number(ordersJsonArray[i].toObject()["Category"].toInt()));

            pod->m_orders.setStringList(orders);
        }

        return pod;
    }

    return nullptr;
}

QJsonObject PodPresenter::saveJsonObject() const
{
    QJsonObject podJsonObj;
    podJsonObj.insert("RowCoord", row());
    podJsonObj.insert("ColCoord", column());

    QStringList orders = m_orders.stringList();
    if (orders.size() > 0)
    {
        QJsonArray ordersJsonArray;
        for (auto& order : orders)
        {
            QJsonObject orderJsonObj;
            orderJsonObj.insert("Category", order.toInt());
            ordersJsonArray.append(orderJsonObj);
        }
        podJsonObj.insert("Orders", ordersJsonArray);
    }

    return podJsonObj;
}

int         PodPresenter::rotation() const { return m_rotation;            }
QStringList PodPresenter::orders()   const { return m_orders.stringList(); }

void PodPresenter::setRotation(int rotation)
{
    if(m_rotation == rotation)
        return;

    m_rotation = rotation;
    emit rotationChanged();
    emit mapItemChanged();
}

void PodPresenter::setOrders(QStringList orders)
{
    if (m_orders.stringList() == orders)
        return;

    m_orders.setStringList(orders);
    emit ordersChanged();
    emit mapItemChanged();
}

QStringList PodPresenter::getOrders(const Pod<OrderModel>* model)
{
    QStringList orders;

    const auto& inventory = model->getInventory();

    for (auto& order : inventory)
        orders.append(QString::number(order->getCategory()));

    return orders;
}

