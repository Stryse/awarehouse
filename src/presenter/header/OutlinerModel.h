#ifndef OUTLINER_MODEL__H
#define OUTLINER_MODEL__H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "Actors.h"

class OutlinerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(const QList<Actors>* actors READ actors WRITE setActors)

public:

    explicit OutlinerModel(QObject *parent = nullptr);

    enum {
            NameRole = Qt::UserRole,
            ActionRole,
            BatteryRole,
            OrientationRole
    };

    virtual ~OutlinerModel();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Editable:
    QHash<int, QByteArray> roleNames() const override;
    void setActors(const QList<Actors>* actors);
    const QList<Actors>* actors() const;

private:
    const QList<Actors>* m_actors;
};

#endif
