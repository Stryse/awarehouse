#ifndef POD_LIST_MODEL__H
#define POD_LIST_MODEL__H

#include <QObject>
#include <QAbstractListModel>

//Presenter
#include "PodList.h"

class PodListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( PodList* pods READ pods WRITE setPods )

public:
    explicit PodListModel(QObject* parent = nullptr);

    enum
    {
        RotationRole = Qt::UserRole,
        OrdersRole,
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
    PodList* pods() const;

    //Setter
    void setPods(PodList* pods);

private:
    PodList* m_pods;

    static QVector<int> m_roles;
};

#endif /* POD_LIST_MODEL__H */
