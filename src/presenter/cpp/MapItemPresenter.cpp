#include "MapItemPresenter.h"

MapItemPresenter::MapItemPresenter(const QString &imagePath, int rowCoord, int colCoord, QObject *parent)
    : QObject(parent), m_imagePath(imagePath), m_rowCoord(rowCoord), m_colCoord(colCoord)
{
}

QString MapItemPresenter::imagePath() const
{
    return m_imagePath;
}

void MapItemPresenter::setImagePath(const QString &imagePath)
{
    m_imagePath = imagePath;
    emit imagePathChanged();
}

int MapItemPresenter::rowCoord() const
{
    return m_rowCoord;
}

void MapItemPresenter::setRowCoord(int value)
{
    m_rowCoord = value;
    emit rowCoordChanged();
}

int MapItemPresenter::colCoord() const
{
    return m_colCoord;
}

void MapItemPresenter::setColCoord(int value)
{
    m_colCoord = value;
    emit colCoordChanged();
}
