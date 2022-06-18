#include "gtmodel.h"
#include "mecha.h"

GTModel::GTModel(Mecha* mecha, const QString& type, QObject *parent) :
    QAbstractTableModel(parent),
    mecha(mecha),
    locs(mecha->getServoNames()),
    type(type)
{
}

int GTModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return locs.size();
}

int GTModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant GTModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags GTModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 1)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant GTModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Kills";
    }
    return 0;
}

QVariant GTModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return locs[row];
    case 1:
        return mecha->getTransKills(type,locs[row]);
    }
    return 0;
}

bool GTModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool GTModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 1:
        mecha->setTransKills(type,locs[row],value.toInt());
        return true;
    }
    return false;
}

bool GTModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool GTModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
