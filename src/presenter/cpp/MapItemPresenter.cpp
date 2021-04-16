#include "MapItemPresenter.h"

MapItemPresenter::MapItemPresenter(           int row,
                                              int column,
                                   const QString& imagePath,
                                         QObject* parent)
    : QObject(parent)
    , m_row      (row      )
    , m_column   (column   )
    , m_imagePath(imagePath)
{}

bool MapItemPresenter::operator==(const MapItemPresenter& other) const
{
    return this->row()       == other.row()       &&
           this->column()    == other.column()    &&
           this->imagePath() == other.imagePath();
}

int     MapItemPresenter::row()       const { return m_row;       }
int     MapItemPresenter::column()    const { return m_column;    }
QString MapItemPresenter::imagePath() const { return m_imagePath; }

void MapItemPresenter::setRow(int row)
{
    if(m_row == row)
        return;

    m_row = row;
    emit rowChanged();
    emit mapItemChanged();
}

void MapItemPresenter::setColumn(int column)
{
    if(m_column == column)
        return;

    m_column    = column;
    emit columnChanged();
    emit mapItemChanged();
}

void MapItemPresenter::setImagePath(const QString& imagePath)
{
    if(m_imagePath == imagePath)
        return;

    m_imagePath = imagePath;
    emit imagePathChanged();
    emit mapItemChanged();
}
