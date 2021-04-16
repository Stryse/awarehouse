#ifndef POD_DOCK_LIST_MODEL__H
#define POD_DOCK_LIST_MODEL__H

#include <QObject>
#include <QAbstractListModel>

//Presenter
#include "PodDockList.h"

class PodDockListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( PodDockList* podDocks READ podDocks WRITE setPodDocks )

public:
    explicit PodDockListModel(QObject* parent = nullptr);

    enum
    {
        RowRole = Qt::UserRole,
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
    PodDockList* podDocks() const;

    //Setter
    void setPodDocks(PodDockList* podDocks);

private:
    PodDockList* m_podDocks;

    static QVector<int> m_roles;
};

#endif /* POD_DOCK_LIST_MODEL__H */
