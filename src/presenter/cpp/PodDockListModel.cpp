#include "PodDockListModel.h"

//Presenter
#include "PodDockPresenter.h"

PodDockListModel::PodDockListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_podDocks(nullptr)
{}

int PodDockListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || m_podDocks == nullptr)
        return 0;

    return m_podDocks->podDocks()->size();
}

QVariant PodDockListModel::data(const QModelIndex& index,
                                             int role) const
{
    if (!index.isValid() || m_podDocks == nullptr)
       return QVariant();

    const PodDockPresenter& actor = *m_podDocks->podDocks()->at(index.row());

    switch(role)
    {
        case RowRole:
            return QVariant(actor.row());
        case ColumnRole:
            return QVariant(actor.column());
        case ImageRole:
            return QVariant(actor.imagePath());
    }

    return QVariant();
}

bool PodDockListModel::setData(const QModelIndex& index,
                             const    QVariant& value,
                                            int role)
{
    if (data(index, role) != value)
    {
        // TODO IMPLEMENT
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags PodDockListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> PodDockListModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[RowRole]      = "row";
    names[ColumnRole]   = "column";
    names[ImageRole]    = "image";

    return names;
}

PodDockList* PodDockListModel::podDocks() const { return m_podDocks; }

void PodDockListModel::setPodDocks(PodDockList* actors)
{
    beginResetModel();

    if (m_podDocks)
        m_podDocks->disconnect(this);

    m_podDocks = actors;

    if (m_podDocks)
    {
        connect(m_podDocks, &PodDockList::preItemAppended,  this, [=]()
        {
            const int index = m_podDocks->podDocks()->size();
            beginInsertRows(QModelIndex(),index,index);
        });

        connect(m_podDocks, &PodDockList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_podDocks, &PodDockList::preItemRemoved,   this, [=](int index)
        {
            beginRemoveRows(QModelIndex(),index, index);
        });

        connect(m_podDocks, &PodDockList::postItemRemoved,  this, [=]()
        {
            endRemoveRows();
        });
    }

    endResetModel();
}
