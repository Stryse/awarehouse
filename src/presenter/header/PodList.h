#ifndef POD_LIST__H
#define POD_LIST__H

#include <QObject>
#include <QList>

//Presenter
#include "PodPresenter.h"

class PodList : public QObject
{
    Q_OBJECT

public:
    explicit PodList(QObject* parent = nullptr);

    QList<const PodPresenter*>* pods();

    bool setPodAt(int index, const PodPresenter& pod);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendPod(const PodPresenter& pod);
    void removePod(int index);

    void clear();

public:
    QList<const PodPresenter*> m_pods;
};

#endif /* POD_LIST__H*/
