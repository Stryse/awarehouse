#ifndef POD_DOCK_PRESENTER_H
#define POD_DOCK_PRESENTER_H

#include <QObject>
//#include <QJsonObject>

//Presenter
#include "MapItemPresenter.h"

class PodDock;
class ObservableNavEnvironment;
class Tile;

class PodDockPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit PodDockPresenter(const PodDock* model,
                                    QObject* parent = nullptr);
    explicit PodDockPresenter(int row, int column, QObject* parent);

    bool operator==(const PodDockPresenter& other) const;

//    static PodDockPresenter* loadJsonObject(const QJsonObject& podDockObj,
//                                                      QObject* parent = nullptr);
//    QJsonObject saveJsonObject() const;

private:
    //Model
    const  PodDock* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* POD_DOCK_PRESENTER_H */
