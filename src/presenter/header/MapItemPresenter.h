#ifndef MAP_ITEM__PRESENTER__H
#define MAP_ITEM__PRESENTER__H

#include <QObject>
#include <QString>

class MapItemPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int     row   READ rowCoord  WRITE setRowCoord  NOTIFY rowCoordChanged  )
    Q_PROPERTY( int     col   READ colCoord  WRITE setColCoord  NOTIFY colCoordChanged  )
    Q_PROPERTY( QString image READ imagePath WRITE setImagePath NOTIFY imagePathChanged )

public:
    explicit MapItemPresenter(           int rowCoord,
                                         int colCoord,
                              const QString& imagePath,
                                    QObject* parent = nullptr);

    //Getter
    int     rowCoord()  const;
    int     colCoord()  const;
    QString imagePath() const;

    //Setter
    void setRowCoord(int value);
    void setColCoord(int value);
    void setImagePath(const QString &imagePath);

signals:
    void rowCoordChanged();
    void colCoordChanged();
    void imagePathChanged();

private:
    int     m_rowCoord;
    int     m_colCoord;
    QString m_imagePath;
};

#endif /* MAP_ITEM__PRESENTER__H */
