#include "weaponmodel.h"
#include "weapon.h"
#include "mecha.h"
#include "servo.h"

#include <QComboBox>

WeaponModel::WeaponModel(Mecha* mecha, QList<Weapon*>& data, QObject *parent) :
        QAbstractTableModel(parent),
        mecha(mecha),
        weaponList(data)
{
}

int WeaponModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return weaponList.size();
}

int WeaponModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 12;
}

QVariant WeaponModel::data(const QModelIndex &index, int role) const
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

QString WeaponModel::getToolTip(int row, int column) const
{
    switch(column)
    {
    case 0:
        return weaponList[row]->getFormattedComments();
    case 9:
        return weaponList[row]->getAllNotes();
    case 11:
        return weaponList[row]->getLinks();
    default:
        return "No Help Yet.";
    }
}

Qt::ItemFlags WeaponModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 6)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant WeaponModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Loc";
    case 7:
        return "Space";
    case 8:
        return "Cost";
    case 9:
        return "Notes";
    case 10:
        return "Link CP";
    case 11:
        return "Linkage";
    }
    return 0;
}

QVariant WeaponModel::getDataByPosition(int row, int column) const
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
        return weaponList[row]->getKills();
    case 6:
        return weaponList[row]->getLoc();
    case 7:
        return weaponList[row]->getSpace();
    case 8:
        return weaponList[row]->getCost();
    case 9:
        return weaponList[row]->getAllNotes();
    case 10:
        return weaponList[row]->getLinkCost();
    case 11:
        return weaponList[row]->getLinks();
    }
    return 0;
}

bool WeaponModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool WeaponModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 6:
        weaponList[row]->setLoc(value.toString());
        return true;
    }
    return false;
}

bool WeaponModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool WeaponModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}

void WeaponModel::update()
{
    emit dataChanged(QModelIndex(),QModelIndex());
}
