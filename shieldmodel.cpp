#include "shieldmodel.h"
#include "shield.h"
#include "mecha.h"

ShieldModel::ShieldModel(Mecha* mecha, QObject *parent) :
    QAbstractTableModel(parent),
    mecha(mecha),
    source(mecha->getShieldRef())
{
}

int ShieldModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return source.size();
}

int ShieldModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 12;
}

QVariant ShieldModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags ShieldModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 2 || index.column() == 3 || index.column() == 5 || index.column() == 10)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ShieldModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Loc";
    case 4:
        return "SP";
    case 5:
        return "Armor";
    case 6:
        return "DC";
    case 7:
        return "B. Space";
    case 8:
        return "Cost";
    case 9:
        return "Space";
    case 10:
        return "Eff";
    case 11:
        return "Notes";
    }
    return 0;
}

QVariant ShieldModel::getDataByPosition(int row, int column) const
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
        return source[row]->getLoc();
    case 4:
        return source[row]->getSP();
    case 5:
        return source[row]->getArmorString();
    case 6:
        return source[row]->getDC();
    case 7:
        return source[row]->getBinderSpace();
    case 8:
        return source[row]->getTotalCost();
    case 9:
        return source[row]->getSpace();
    case 10:
        return source[row]->getEff();
    case 11:
        return source[row]->getAllNotes();
    }
    return 0;
}

bool ShieldModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool ShieldModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 2:
        source[row]->setSize((ServoSizes)value.toInt());
        return true;
    case 3:
        if ((!mecha->getServoNames().contains(value.toString()) && source[row]->getType())
            && !(value.toString() == "Split" && source[row]->getType() == REACTIVE) )
        {
            source[row]->setLoc("None");
            return true;
        }
        source[row]->setLoc(value.toString());
        return true;
    case 5:
        if (source[row]->getType() == REACTIVE)
            return true;
        source[row]->setArmor((ArmorTypes)value.toInt());
        return true;
    case 10:
        source[row]->setEff(value.toDouble());
        return true;
    }
    return false;
}

bool ShieldModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool ShieldModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
