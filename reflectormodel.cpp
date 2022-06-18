#include "reflectormodel.h"
#include "reflector.h"
#include "mecha.h"

ReflectorModel::ReflectorModel(Mecha* mecha,QObject *parent) :
    QAbstractTableModel(parent),
    mecha(mecha),
    source(mecha->getReflectRef())
{
}

int ReflectorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return source.size();
}

int ReflectorModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant ReflectorModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags ReflectorModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 0 || index.column() == 1 || index.column() == 2 || index.column() == 5)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ReflectorModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Value";
    case 2:
        return "Location";
    case 3:
        return "Cost";
    case 4:
        return "Space";
    case 5:
        return "Eff";
    }
    return 0;
}

QVariant ReflectorModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return source[row]->getName();
    case 1:
        return source[row]->getValue();
    case 2:
        return source[row]->getLoc();
    case 3:
        return source[row]->getCost();
    case 4:
        return source[row]->getSpace();
    case 5:
        return source[row]->getEff();
    }
    return 0;
}

bool ReflectorModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool ReflectorModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 0:
        source[row]->setName(value.toString());
        return true;
    case 1:
        source[row]->setValue(value.toUInt());
        return true;
    case 2:
        source[row]->setLoc(value.toString());
        return true;
    case 5:
        source[row]->setEff(value.toDouble());
        return true;
    }
    return false;
}

bool ReflectorModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool ReflectorModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
