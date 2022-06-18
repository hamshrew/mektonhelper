#include "specmodel.h"
#include "weapon.h"

SpecModel::SpecModel(QList<Weapon>& data, QObject *parent) :
        QAbstractTableModel(parent),
        weaponList(data)
{
}

int SpecModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return weaponList.size();
}

int SpecModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 9;
}

QVariant SpecModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags SpecModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant SpecModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "WA";
    case 2:
        return "Range";
    case 3:
        return "Damage";
    case 4:
        return "Shots";
    case 5:
        return "Kills";
    case 6:
        return "CP";
    case 7:
        return "Space";
    case 8:
        return "Notes";
    }
    return 0;
}

QVariant SpecModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return weaponList[row].getName();
    case 1:
        return weaponList[row].getAcc();
    case 2:
        if (weaponList[row].getRange() <= 0)
            return "Melee";
        return weaponList[row].getRange();
    case 3:
        return weaponList[row].getDamage();
    case 4:
        {
            QString result(weaponList[row].getShotPrepend());
            result+=weaponList[row].getShots();
            return result;
        }
    case 5:
        return weaponList[row].getKills();
    case 6:
        return weaponList[row].getCost();
    case 7:
        return weaponList[row].getSpace();
    case 8:
        return weaponList[row].getAllNotes();
    }
    return 0;
}

bool SpecModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool SpecModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}

QString SpecModel::getToolTip(int row, int column) const
{
    switch(column)
    {
    case 0:
        return weaponList[row].getFormattedComments();
    case 8:
        return weaponList[row].getAllNotes();
    default:
        return "No Help Yet.";
    }
}
