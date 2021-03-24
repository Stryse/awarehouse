#ifndef MAP_TABLE_PRESENTER_MODEL__H
#define MAP_TABLE_PRESENTER_MODEL__H

#include <QAbstractTableModel>
#include <QList>
#include "MapItemPresenter.h"
#include "MapTableList.h"

class MapTablePresenterModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_ENUMS(Roles);
    Q_PROPERTY(QList<const MapItemPresenter*>* items READ items WRITE setItems)

public:
    enum Roles
    {
      ImageRole = Qt::UserRole + 1
    };

    QHash<int, QByteArray> roleNames() const override;

    explicit MapTablePresenterModel(QList<const MapItemPresenter*>* items = nullptr, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;

    int columnCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    size_t getIndexByPoint(int row, int col) const;

    void setItems(QList<const MapItemPresenter*>* items);
    QList<const MapItemPresenter*>* items() const;

private:

    int m_rowCount;
    int m_colCount;
    int m_size;

    MapTableList mapTable;
};

#endif /* MAP_TABLE_PRESENTER_MODEL__H */
