#ifndef POD_DOCK_PRESENTER_H
#define POD_DOCK_PRESENTER_H

#include <QObject>

//Presenter
#include "MapItemPresenter.h"

template <typename E> class PodDock;
                      class ObservableNavEnvironment;
                      class Tile;

class PodDockPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit PodDockPresenter(const PodDock<ObservableNavEnvironment>* model,
                                                                    QObject* parent = nullptr);
private:
    //Model
    const  PodDock<ObservableNavEnvironment>* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* POD_DOCK_PRESENTER_H */
