#include "optionmodel.h"
#include "mecha.h"
#include "option.h"

OptionModel::OptionModel(Mecha* mecha, QObject *parent) :
    QAbstractTableModel(parent),
    mecha(mecha),
    source(mecha->getOptionRef())
{
}

int OptionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return source.size();
}

int OptionModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant OptionModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags OptionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 1 || index.column() == 2 || index.column() == 3 || index.column() == 6)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant OptionModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "System";
    case 1:
        return "Notes";
    case 2:
        return "Number";
    case 3:
        return "Location";
    case 4:
        return "Cost";
    case 5:
        return "Space";
    case 6:
        return "Eff";
    }
    return 0;
}

QVariant OptionModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return source[row]->getName();
    case 1:
        return source[row]->getNotes();
    case 2:
        return source[row]->getNumber();
    case 3:
        return source[row]->getLoc();
    case 4:
        return source[row]->getCost(mecha->getWeight());
    case 5:
        return source[row]->getSpace(mecha->getWeight());
    case 6:
        return source[row]->getEff();
    }
    return 0;
}

bool OptionModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool OptionModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 1:
        source[row]->setNotes(value.toString());
        return true;
    case 2:
        source[row]->setNumber(value.toInt());
        return true;
    case 3:
        source[row]->setLoc(value.toString());
        return true;
    case 6:
        source[row]->setEff(value.toDouble());
        return true;
    }
    return false;
}

bool OptionModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool OptionModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
