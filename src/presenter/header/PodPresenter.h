#ifndef POD_PRESENTER_H
#define POD_PRESENTER_H

#include <QObject>
#include <QStringListModel>
#include <QJsonObject>

//Presenter
#include "MapItemPresenter.h"

                            class OrderModel;
template<typename ItemType> class Pod;

class PodPresenter : public MapItemPresenter
{
    Q_OBJECT

    Q_PROPERTY(         int rotation READ rotation WRITE setRotation NOTIFY rotationChanged )
    Q_PROPERTY( QStringList orders   READ orders   WRITE setOrders   NOTIFY ordersChanged   )

public:
    explicit PodPresenter(const Pod<OrderModel>* model,
                                        QObject* parent = nullptr);
    explicit PodPresenter(int      row,
                          int      column,
                          QObject* parent = nullptr);

    static PodPresenter* loadJsonObject(const QJsonObject& podObj,
                                                  QObject* parent = nullptr);
    QJsonObject saveJsonObject() const;

    //Getter
    int         rotation() const;
    QStringList orders()   const;

    //Setter
    void setRotation(int rotation);
    void setOrders(QStringList orders);

private:
    QStringList getOrders(const Pod<OrderModel>* model);

signals:
    void rotationChanged();
    void ordersChanged();

private:
    int              m_rotation;
    QStringListModel m_orders;

    //Model
    const  Pod<OrderModel>* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* POD_PRESENTER_H */
