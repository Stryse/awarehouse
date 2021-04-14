#ifndef CHARGING_STATION_LIST_MODEL__H
#define CHARGING_STATION_LIST_MODEL__H

#include <QObject>
#include <QAbstractListModel>

//Presenter
#include "ChargingStationList.h"

class ChargingStationListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( ChargingStationList* chargingStations READ chargingStations WRITE setChargingStations )

public:
    explicit ChargingStationListModel(QObject* parent = nullptr);

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
    ChargingStationList* chargingStations() const;

    //Setter
    void setChargingStations(ChargingStationList* chargingStations);

private:
    ChargingStationList* m_chargingStations;
};

#endif /* CHARGING_STATION_LIST_MODEL__H */
