#include "ammomodel.h"
#include "ammo.h"
#include "weapon.h"

AmmoModel::AmmoModel(QList<Ammo*>& data, QHash<QString,Weapon>& wpHash,QObject *parent) :
    QAbstractTableModel(parent),
    ammoList(data),
    wpHash(wpHash)
{
}

int AmmoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ammoList.size();
}

int AmmoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 11;
}

QVariant AmmoModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags AmmoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 6 || index.column() == 9)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant AmmoModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Ammo Type";
    case 1:
        return "WA";
    case 2:
        return "Damage";
    case 3:
        return "Shots";
    case 4:
        return "Clips";
    case 5:
        return "Kills";
    case 6:
        return "Loc";
    case 7:
        return "Space";
    case 8:
        return "Cost";
    case 9:
        return "Eff";
    case 10:
        return "Notes";
    }
    return 0;
}

QVariant AmmoModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return ammoList[row]->getWeaponName();
    case 1:
        return ammoList[row]->getAcc();
    case 2:
        return ammoList[row]->getDamage();
    case 3:
        return ammoList[row]->getShots();
    case 4:
        return ammoList[row]->getClips();
    case 5:
        return ammoList[row]->getKills();
    case 6:
        return ammoList[row]->getLoc();
    case 7: {
        QRegExp re("\\d+H");
        if (re.exactMatch(wpHash[ammoList[row]->getWeaponName()].getLoc()) && !re.exactMatch(ammoList[row]->getLoc()))
            return 0;
        return ammoList[row]->getSpace();}
    case 8:
        return ammoList[row]->getCost();
    case 9:
        return ammoList[row]->getEff();
    case 10:
        return ammoList[row]->getAllNotes();
    }
    return 0;
}

bool AmmoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || index.row() >= ammoList.size())
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

bool AmmoModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 6:
        ammoList[row]->setLoc(value.toString());
        return true;
    case 9:
        ammoList[row]->setEff(value.toDouble());
        return true;
    }
    return false;
}

bool AmmoModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool AmmoModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}

void AmmoModel::update()
{
    emit dataChanged(QModelIndex(),QModelIndex());
}
