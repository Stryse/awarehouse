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

int     MapItemPresenter::row()       const { return m_row;       }
int     MapItemPresenter::column()    const { return m_column;    }
QString MapItemPresenter::imagePath() const { return m_imagePath; }

void MapItemPresenter::setRow(int row)                        { m_row       = row;
                                                                emit rowChanged();       }
void MapItemPresenter::setColumn(int column)                  { m_column    = column;
                                                                emit columnChanged();    }
void MapItemPresenter::setImagePath(const QString& imagePath) { m_imagePath = imagePath;
                                                                emit imagePathChanged(); }
