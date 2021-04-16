#ifndef ACTOR_LIST_MODEL__H
#define ACTOR_LIST_MODEL__H

#include <QObject>
#include <QAbstractListModel>

//Presenter
#include "ActorList.h"

class ActorListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( ActorList* actors READ actors WRITE setActors )

public:
    explicit ActorListModel(QObject* parent = nullptr);

    enum
    {
        NameRole = Qt::UserRole,
        ActionRole,
        BatteryRole,
        RotationRole,
        MovesRole,
        RowRole,
        ColumnRole,
        ImageRole
    };


//QAbstractItemModel interface
public:
    int      rowCount(const QModelIndex& parent)                        const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Editable
    bool setData(const QModelIndex& index,
                 const    QVariant& value,
                                int role = Qt::EditRole) override;

    Qt::ItemFlags          flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames()                     const override;

    //Getter
    ActorList* actors() const;

    //Setter
    void setActors(ActorList* actors);

private:
    ActorList* m_actors;

    static QVector<int> m_roles;
};

#endif /* ACTOR_LIST_MODEL__H */
