#ifndef OUTLINER_MODEL__H
#define OUTLINER_MODEL__H

#include <QObject>
#include <QAbstractListModel>
#include "Actors.h"
#include "OutlinerList.h"

class OutlinerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(OutlinerList* actors READ actors WRITE setActors)

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
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;


    // Getter setter
    void setActors(OutlinerList* actors);
    OutlinerList* actors() const;

private:
    OutlinerList *m_actors;
};

#endif
