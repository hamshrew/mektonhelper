#include "poolshieldmodel.h"
#include "shield.h"
#include "mecha.h"

PoolShieldModel::PoolShieldModel(QList<Shield*>& data, QObject *parent) :
    QAbstractTableModel(parent),
    source(data)
{
}

int PoolShieldModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return source.size();
}

int PoolShieldModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8;
}

QVariant PoolShieldModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags PoolShieldModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 2 || index.column() == 4)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant PoolShieldModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Type";
    case 2:
        return "Class";
    case 3:
        return "SP";
    case 4:
        return "Armor";
    case 5:
        return "DC";
    case 6:
        return "Cost";
    case 7:
        return "Notes";
    }
    return 0;
}

QVariant PoolShieldModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return source[row]->getName();
    case 1:
        return source[row]->getTypeString();
    case 2:
        return source[row]->getSizeString();
    case 3:
        return source[row]->getSP();
    case 4:
        return source[row]->getArmorString();
    case 5:
        return source[row]->getDC();
    case 6:
        return source[row]->getTotalCost();
    case 7:
        return source[row]->getAllNotes();
    }
    return 0;
}

bool PoolShieldModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool PoolShieldModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 2:
        source[row]->setSize((ServoSizes)value.toInt());
        return true;
    case 4:
        if (source[row]->getType() == REACTIVE)
            return true;
        source[row]->setArmor((ArmorTypes)value.toInt());
        return true;
    }
    return false;
}

bool PoolShieldModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool PoolShieldModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
