#ifndef MAP_ITEM__PRESENTER__H
#define MAP_ITEM__PRESENTER__H

#include <QObject>
#include <QString>

class MapItemPresenter : public QObject
{
  Q_OBJECT
  Q_PROPERTY (QString image READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
  Q_PROPERTY (int row READ rowCoord WRITE setRowCoord NOTIFY rowCoordChanged)
  Q_PROPERTY (int col READ colCoord WRITE setColCoord NOTIFY colCoordChanged)

public:

    QString imagePath() const;
    void setImagePath(const QString& imagePath);

    int rowCoord() const;
    void setRowCoord(int value);

    int colCoord() const;
    void setColCoord(int value);

    explicit MapItemPresenter(const QString& imagePath, int rowCoord, int colCoord,QObject *parent = nullptr);
signals:
    void imagePathChanged();
    void rowCoordChanged();
    void colCoordChanged();

private:
    QString m_imagePath;
    int m_rowCoord;
    int m_colCoord;
};

#endif /* MAP_ITEM__PRESENTER__H */
