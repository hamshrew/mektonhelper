#include "splitmodel.h"
#include "mecha.h"
#include "splittablepart.h"

SplitModel::SplitModel(Mecha *mecha,SplittablePart* part,QObject *parent) :
    QAbstractTableModel(parent),
    mecha(mecha),
    locs(mecha->getServoNames()),
    part(part)
{
}

int SplitModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return locs.size();
}

int SplitModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant SplitModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:
        return this->getDataByPosition(index.row(),index.column());
    case Qt::ToolTipRole:
        return QString("No Help Yet.");
    default:
        return QVariant();
    }
}

Qt::ItemFlags SplitModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 1)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant SplitModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
    {
        return section +1;
    }

    switch (section)
    {
    case 0:
        return "Loc";
    case 1:
        return "Spaces";
    }
    return 0;
}

QVariant SplitModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return locs[row];
    case 1:
        return part ? part->getSpace(locs[row]) : 0;
    }
    return 0;
}

bool SplitModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || index.row() >= locs.size())
    {
        return false;
    }

    if (!setDataByPosition(index.row(),index.column(),value))
    {
        return false;
    }

    emit dataChanged(index,index);
    return true;
}

bool SplitModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 1:
        if (!part)
            return true;
        part->setAlloc(locs[row],value.toDouble());
        part->normalize(locs);
        return true;
    }
    return false;
}

bool SplitModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool SplitModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
