#include "propmodel.h"
#include "propulsion.h"
#include "mecha.h"

#include <QStringList>

PropModel::PropModel(Mecha *mecha,const QString& propName,QObject *parent) :
    QAbstractTableModel(parent),
    mecha(mecha),
    locs(mecha->getServoNames()),
    prop(mecha->getPropMapRef().contains(propName) ? mecha->getPropMapRef()[propName] : 0)
{
}

int PropModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return locs.size();
}

int PropModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant PropModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags PropModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 1)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant PropModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Loc";
    case 1:
        return "Spaces";
    }
    return 0;
}

QVariant PropModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return locs[row];
    case 1:
        return prop ? prop->getSpaces(locs[row]) : 0;
    }
    return 0;
}

bool PropModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || index.row() >= locs.size())
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

bool PropModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 1:
        if (!prop)
            return true;
        prop->setSpaces(locs[row],value.toDouble());
        prop->normalize(mecha->getWeight());
        return true;
    }
    return false;
}

bool PropModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool PropModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
