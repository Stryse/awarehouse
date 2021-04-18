#ifndef POD_DOCK_LIST__H
#define POD_DOCK_LIST__H

#include <QObject>
#include <QList>
#include <QJsonArray>

//Presenter
#include "PodDockPresenter.h"

class PodDockList : public QObject
{
    Q_OBJECT

public:
    explicit PodDockList(QObject* parent = nullptr);

    QList<PodDockPresenter*>* podDocks();

    bool setPodDockAt(int index, PodDockPresenter& podDock);

    void loadJsonArray(const QJsonArray& podDocksJSon);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void dataChanged(int index);

public slots:
    void appendPodDock(PodDockPresenter& podDock);
    void removePodDock(int index);
    void removePodDock(int row, int column);

    void clear();

public:
    QList<PodDockPresenter*> m_podDocks;
};

#endif /* POD_DOCK_LIST__H */
