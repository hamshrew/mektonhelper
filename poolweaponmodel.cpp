#include "poolweaponmodel.h"
#include "weapon.h"
#include "mecha.h"
#include "servo.h"

PoolWeaponModel::PoolWeaponModel(Mecha* mecha, QList<Weapon*>& data, QObject *parent) :
        QAbstractTableModel(parent),
        mecha(mecha),
        weaponList(data)
{
}

int PoolWeaponModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return weaponList.size();
}

int PoolWeaponModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant PoolWeaponModel::data(const QModelIndex &index, int role) const
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

QString PoolWeaponModel::getToolTip(int row, int column) const
{
    return this->getDataByPosition(row,column).toString();
}

Qt::ItemFlags PoolWeaponModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant PoolWeaponModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Cost";
    case 6:
        return "Notes";
    }
    return 0;
}

QVariant PoolWeaponModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return weaponList[row]->getName();
    case 1:
        return weaponList[row]->getAcc();
    case 2:
        if (weaponList[row]->getRange() <= 0)
            return "Melee";
        return weaponList[row]->getRange();
    case 3:{
            if (weaponList[row]->getLoc().at(0).isDigit() && weaponList[row]->isMelee())
                return QString::number(weaponList[row]->getDamage()) + "+" + QString::number(mecha->getBestDamageBonus()
                                                                                             + mecha->getHydraulicsBonus());

            Servo* s = mecha->getServoByLoc(weaponList[row]->getLoc());
            if (s == NULL)
                return weaponList[row]->getDamage();

            if ((s->getType() == LEG || s->getType() == ARM) && weaponList[row]->isMelee())
                return QString::number(weaponList[row]->getDamage()) + "+" + QString::number(s->getDamageBonus()
                                                                                             + mecha->getHydraulicsBonus());

            return weaponList[row]->getDamage();
            }
    case 4:
        {
            QString result(weaponList[row]->getShotPrepend());
            result+=weaponList[row]->getShots();
            return result;
        }
    case 5:
        return weaponList[row]->getCost();
    case 6:
        return weaponList[row]->getAllNotes();
    }
    return 0;
}

bool PoolWeaponModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || index.row() >= weaponList.size())
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

bool PoolWeaponModel::setDataByPosition(int row, int column, const QVariant &value)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(value);
    return false;
}

bool PoolWeaponModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool PoolWeaponModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}

void PoolWeaponModel::update()
{
    emit dataChanged(QModelIndex(),QModelIndex());
}
