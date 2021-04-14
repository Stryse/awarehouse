#include "MapItemPresenter.h"

MapItemPresenter::MapItemPresenter(           int rowCoord,
                                              int colCoord,
                                   const QString& imagePath,
                                         QObject* parent)
    : QObject(parent)
    , m_rowCoord (rowCoord )
    , m_colCoord (colCoord )
    , m_imagePath(imagePath)
{}

int     MapItemPresenter::rowCoord()  const { return m_rowCoord;  }
int     MapItemPresenter::colCoord()  const { return m_colCoord;  }
QString MapItemPresenter::imagePath() const { return m_imagePath; }

void MapItemPresenter::setRowCoord(int value)                 { m_rowCoord  = value;
                                                                emit rowCoordChanged();  }
void MapItemPresenter::setColCoord(int value)                 { m_colCoord  = value;
                                                                emit colCoordChanged();  }
void MapItemPresenter::setImagePath(const QString& imagePath) { m_imagePath = imagePath;
                                                                emit imagePathChanged(); }
