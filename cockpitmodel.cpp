#include "cockpitmodel.h"
#include "cockpit.h"

CockpitModel::CockpitModel(QList<Cockpit*>& data, QObject *parent) :
    QAbstractTableModel(parent),
    cpList(data)
{
}

int CockpitModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return cpList.size();
}

int CockpitModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8;
}

QVariant CockpitModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags CockpitModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if ((index.column() < 4) || (index.column() > 5))
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant CockpitModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Designation";
    case 1:
        return "Location";
    case 2:
        return "Crew";
    case 3:
        return "Passengers";
    case 4:
        return "Cost";
    case 5:
        return "Space";
    case 6:
        return "Enclosure";
    case 7:
        return "Eff";
    }
    return 0;
}

QVariant CockpitModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return cpList[row]->getName();
    case 1:
        return cpList[row]->getLoc();
    case 2:
        return cpList[row]->getCrew();
    case 3:
        return cpList[row]->getPassengers();
    case 4:
        return cpList[row]->getCost();
    case 5:
        return cpList[row]->getSpace();
    case 6:
        return cpList[row]->getEnc();
    case 7:
        return cpList[row]->getEff();
    }
    return 0;
}

bool CockpitModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || index.row() >= cpList.size())
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

bool CockpitModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 0:
        cpList[row]->setName(value.toString());
        return true;
    case 1:
        cpList[row]->setLoc(value.toString());
        return true;
    case 2:
        cpList[row]->setCrew(value.toUInt());
        return true;
    case 3:
        cpList[row]->setPassengers(value.toUInt());
        return true;
    case 6:
        cpList[row]->setEnc(value.toString());
        return true;
    case 7:
        cpList[row]->setEff(value.toDouble());
        return true;
    }
    return false;
}

bool CockpitModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool CockpitModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
