#ifndef DELIVERY_STATION_LIST_MODEL__H
#define DELIVERY_STATION_LIST_MODEL__H

#include <QObject>
#include <QAbstractListModel>

//Presenter
#include "DeliveryStationList.h"

class DeliveryStationListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( DeliveryStationList* deliveryStations READ deliveryStations WRITE setDeliveryStations )

public:
    explicit DeliveryStationListModel(QObject* parent = nullptr);

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
    DeliveryStationList* deliveryStations() const;

    //Setter
    void setDeliveryStations(DeliveryStationList* deliveryStations);

private:
    DeliveryStationList* m_deliveryStations;
};

#endif /* DELIVERY_STATION_LIST_MODEL__H */
