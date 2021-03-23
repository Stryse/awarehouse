#ifndef PODDOCKPRESENTER_H
#define PODDOCKPRESENTER_H

#include <MapItemPresenter.h>
#include <QJsonObject>
#include <QObject>

class PodDockPresenter : public MapItemPresenter
{
    Q_OBJECT
public:
    explicit PodDockPresenter(int rowCoord, int colCoord, QObject *parent=nullptr);

    static PodDockPresenter* read(QJsonObject &jsonObject, QObject *parent=nullptr);

private:
    static QString imagePath;
};

#endif // PODDOCKPRESENTER_H
