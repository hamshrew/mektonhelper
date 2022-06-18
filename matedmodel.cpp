#include "matedmodel.h"
#include "matedsystem.h"
#include "mecha.h"

MatedModel::MatedModel(Mecha* mecha,QObject *parent) :
    QAbstractTableModel(parent),
    mecha(mecha),
    source(mecha->getMatedPartsRef())
{
}

int MatedModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return source.size();
}

int MatedModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant MatedModel::data(const QModelIndex &index, int role) const
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

QString MatedModel::getToolTip(int row, int column) const
{
    Q_UNUSED(row);
    switch(column)
    {
    default:
        return "No Help Yet.";
    }
}

Qt::ItemFlags MatedModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 1 || index.column() == 2 || index.column() == 3)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant MatedModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Loc";
    case 2:
        return "First System";
    case 3:
        return "Second System";
    case 4:
        return "Old Space";
    case 5:
        return "New Space";
    }
    return 0;
}

QVariant MatedModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return source[row]->getName();
    case 1:
        return source[row]->getLoc();
    case 2:
        {SplittablePart* part = source[row]->getPart(0);
        if (part)
            return part->getName();
        else return "None";}
    case 3:
        {SplittablePart* part = source[row]->getPart(1);
        if (part)
            return part->getName();
        else return "None";}
    case 4:
        {
            SplittablePart* part1 = source[row]->getPart(0);
            SplittablePart* part2 = source[row]->getPart(1);
            return (part1 ? part1->getSpace() : 0) + (part2 ? part2->getSpace() : 0);
        }
    case 5:
        return source[row]->getSpace();
    }
    return 0;
}

bool MatedModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool MatedModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 1:
        source[row]->setLoc(value.toString());
        return true;
    case 2:
        {
            QList<SplittablePart*> spList = mecha->getMateableSystems();
            if (source[row]->getPart(0))
            {
                if (!value.toInt())
                {
                    source[row]->remPart(source[row]->getPart(0));
                    return true;
                }
                SplittablePart* part2 = source[row]->getPart(1);
                if (part2)
                {
                    source[row]->remPart(part2);
                    source[row]->remPart(source[row]->getPart(0));
                    source[row]->addPart(spList.at(value.toInt()-1));
                    source[row]->addPart(part2);
                }
            }
            else if (value.toInt() > 0)
            {
                source[row]->addPart(spList.at(value.toInt()-1));
            }
            return true;
        }
    case 3:
        {
            QList<SplittablePart*> spList = mecha->getMateableSystems();
            if (source[row]->getPart(1))
            {
                source[row]->remPart(source[row]->getPart(1));
                source[row]->addPart(spList.at(value.toInt()-1));
            }
            else if (value.toInt() > 0)
            {
                source[row]->addPart(spList.at(value.toInt()-1));
            }
            return true;
        }
    }
    return false;
}

bool MatedModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool MatedModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}

void MatedModel::update()
{
    emit dataChanged(QModelIndex(),QModelIndex());
}
