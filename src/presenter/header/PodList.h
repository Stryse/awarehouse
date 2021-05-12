#ifndef POD_LIST__H
#define POD_LIST__H

#include <QObject>
#include <QList>
#include <QJsonArray>

//Presenter
#include "PodPresenter.h"

/******************************************************************
 * @brief Decorator of a QList which broadcast manipulation events.
 ******************************************************************/
class PodList : public QObject
{
    Q_OBJECT

public:
    explicit PodList(QObject* parent = nullptr);

    QList<PodPresenter*>* pods();

    bool setPodAt(int index, PodPresenter& pod);

    void       loadJsonArray(const QJsonArray& podsJSon);
    QJsonArray saveJsonArray() const;

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void dataChanged(int index);

public slots:
    void appendPod(PodPresenter& pod);
    void removePod(int index);
    void removePod(int row, int column);

    void clear();

public:
    QList<PodPresenter*> m_pods;
};

#endif /* POD_LIST__H*/
