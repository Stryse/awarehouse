#ifndef OUTLINERLISTMODEL_H
#define OUTLINERLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>

class Presenter;
class Actors;

class OutlinerListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY (OutlinerListActor* list READ list WRITE setList)

public:
    explicit OutlinerListModel(QObject *parent = nullptr);

    enum {
        NameRole,
        ActionRole,
        BatteryRole,
        OrientationRole
    };

    // Header:
    // QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    QVector<Actors> list() const;
    void setList(QVector<Actors> *list);

private:
    Presenter *mActorList;
};

#endif // OUTLINERLISTMODEL_H
