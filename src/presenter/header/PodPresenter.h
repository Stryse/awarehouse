#ifndef POD_PRESENTER_H
#define POD_PRESENTER_H

#include <QObject>

//Presenter
#include "MapItemPresenter.h"

class Pod;

class PodPresenter : public MapItemPresenter
{
    Q_OBJECT

    Q_PROPERTY(int rotation READ rotation NOTIFY rotationChanged)

public:
    explicit PodPresenter(const Pod* model,
                            QObject* parent = nullptr);
    //Getter
    int rotation() const;

    //Setter
    void setRotation(int rotation);

signals:
    void rotationChanged();

private:
    int m_rotation;

    //Model
    const  Pod* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* POD_PRESENTER_H */
