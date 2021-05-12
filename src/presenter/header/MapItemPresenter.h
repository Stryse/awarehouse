#ifndef MAP_ITEM__PRESENTER__H
#define MAP_ITEM__PRESENTER__H

#include <QObject>
#include <QString>

class MapItemPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(     int row   READ row       WRITE setRow       NOTIFY rowChanged       )
    Q_PROPERTY(     int col   READ column    WRITE setColumn    NOTIFY columnChanged    )
    Q_PROPERTY( QString image READ imagePath WRITE setImagePath NOTIFY imagePathChanged )

public:
    explicit MapItemPresenter(           int row,
                                         int column,
                              const QString& imagePath,
                                    QObject* parent = nullptr);

    bool operator==(const MapItemPresenter& other) const;

    //Getter
    int     row()       const;
    int     column()    const;
    QString imagePath() const;

    //Setter
    void setRow(int row);
    void setColumn(int column);
    void setImagePath(const QString& imagePath);

signals:
    void rowChanged();
    void columnChanged();
    void imagePathChanged();

    void mapItemChanged();

protected:
    int     m_row;
    int     m_column;
    QString m_imagePath;
};

#endif /* MAP_ITEM__PRESENTER__H */
