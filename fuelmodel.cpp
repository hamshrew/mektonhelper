#include "fuelmodel.h"
#include "fuel.h"

FuelModel::FuelModel(QList<Fuel*>& data, QObject *parent) :
    QAbstractTableModel(parent),
    source(data)
{
}

int FuelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return source.size();
}

int FuelModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant FuelModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags FuelModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if ((index.column() < 3) || (index.column() == 5))
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant FuelModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Type";
    case 1:
        return "Loc";
    case 2:
        return "#";
    case 3:
        return "CP";
    case 4:
        return "Sp";
    case 5:
        return "Eff";
    }
    return 0;
}

QVariant FuelModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return source[row]->isExternal() ? "External":"Internal";
    case 1:
        return source[row]->getLoc();
    case 2:
        return source[row]->getNumTanks();
    case 3:
        return source[row]->getCost();
    case 4:
        return source[row]->getSpace();
    case 5:
        return source[row]->getEff();
    }
    return 0;
}

bool FuelModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool FuelModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 0:
        source[row]->setExternal((value.toString() == "External"));
        return true;
    case 1:
        source[row]->setLoc(value.toString());
        return true;
    case 2:
        source[row]->setNumTanks(value.toInt());
        return true;
    case 5:
        source[row]->setEff(value.toDouble());
        return true;
    }
    return false;
}

bool FuelModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool FuelModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
