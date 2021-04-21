#ifndef PERSISTENCE_PRESENTER__H
#define PERSISTENCE_PRESENTER__H

#include <QObject>
#include <QStringListModel>

class PersistencePresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(     QString defaultWarehousePath READ defaultWarehousePath CONSTANT                   )
    Q_PROPERTY(     QString warehouseDirPath     READ warehouseDirPath     CONSTANT                   )

    Q_PROPERTY( QStringList warehouses           READ warehouses           NOTIFY   warehousesChanged )

public:
    explicit PersistencePresenter(QObject* parent = nullptr);

    QStringList warehouses()           const;
    QString     defaultWarehousePath() const;
    QString     warehouseDirPath()     const;

    static QString getWarehousePath(const QString& warehouseName);

private:
    void readWarehouses(QString path);

signals:
    void warehousesChanged();

public slots:
    void appendWarehouse(QString warehouse);
    void removeWarehouse(int index);

    int getWarehouseIndex(const QString& warehouseName) const;

    void reloadWarehouses();

public:
    static const QString defaultWarehouseName;

private:
    static const QString m_defaultWarehousePath;
    static const QString m_warehouseDirPath;

    QStringListModel m_warehouses; //Why StringListModel???
};

#endif /* PERSISTENCE_PRESENTER__H */
