#include "poolmodel.h"
#include "energypool.h"
#include "mecha.h"

PoolModel::PoolModel(Mecha* mecha,QObject *parent) :
    QAbstractTableModel(parent),
    mecha(mecha),
    source(mecha->getPoolRef())
{
}

int PoolModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return source.size();
}

int PoolModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 10;
}

QVariant PoolModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:
        return this->getDataByPosition(index.row(),index.column());
    case Qt::ToolTipRole:
        return getToolTip(index.row(),index.column());
    default:
        return QVariant();
    }
}

QString PoolModel::getToolTip(int row, int column) const
{
    Q_UNUSED(row);
    switch(column)
    {
    default:
        return "No Help Yet.";
    }
}

Qt::ItemFlags PoolModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 1 || index.column() == 9)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant PoolModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Name";
    case 1:
        return "Loc";
    case 2:
        return "Available\nPower";
    case 3:
        return "Max Power";
    case 4:
        return "Portfolio";
    case 5:
        return "Kills";
    case 6:
        return "Morphable";
    case 7:
        return "Space";
    case 8:
        return "Cost";
    case 9:
        return "Eff";
    }
    return 0;
}

QVariant PoolModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return source[row]->getName();
    case 1:
        return source[row]->getLoc();
    case 2:
        return source[row]->getPowerAvailable();
    case 3:
        return source[row]->getMaxPower();
    case 4:
        return source[row]->getPortfolio();
    case 5:
        return source[row]->getKills();
    case 6:
        return source[row]->isMorphable();
    case 7:
        return source[row]->getSpace();
    case 8:
        return source[row]->getCost();
    case 9:
        return source[row]->getEff();
    }
    return 0;
}

bool PoolModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || index.row() >= source.size())
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

bool PoolModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 1:
        source[row]->setLoc(value.toString());
        return true;
    case 9:
        source[row]->setEff(value.toDouble());
        return true;
    }
    return false;
}

bool PoolModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool PoolModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}

void PoolModel::update()
{
    emit dataChanged(QModelIndex(),QModelIndex());
}
