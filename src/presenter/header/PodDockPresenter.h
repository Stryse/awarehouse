#ifndef PODDOCKPRESENTER_H
#define PODDOCKPRESENTER_H

#include <QObject>

//Presenter
#include "MapItemPresenter.h"

template <typename E> class PodDock;
template <typename V> class ObservableNavEnvironment;
class Tile;

class PodDockPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit PodDockPresenter(const PodDock<ObservableNavEnvironment<Tile>>* model,
                              QObject *parent=nullptr);
private:

    const PodDock<ObservableNavEnvironment<Tile>>* model;
    static QString imagePath;
};

#endif // PODDOCKPRESENTER_H
