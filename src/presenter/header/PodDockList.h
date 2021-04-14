#ifndef POD_DOCK_LIST__H
#define POD_DOCK_LIST__H

#include <QObject>
#include <QList>

//Presenter
#include "PodDockPresenter.h"

class PodDockList : public QObject
{
    Q_OBJECT

public:
    explicit PodDockList(QObject* parent = nullptr);

    QList<const PodDockPresenter*>* podDocks();

    bool setPodDockAt(int index, const PodDockPresenter& podDock);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendPodDock(const PodDockPresenter& podDock);
    void removePodDock(int index);

    void clear();

public:
    QList<const PodDockPresenter*> m_podDocks;
};

#endif /* POD_DOCK_LIST__H */
