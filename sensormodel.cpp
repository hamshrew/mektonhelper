#include "sensormodel.h"
#include "sensor.h"

#include <QString>
#include <QStringList>

SensorModel::SensorModel(QList<Sensor*>& data, QObject *parent) :
    QAbstractTableModel(parent),
    sList(data)
{
}

int SensorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return sList.size();
}

int SensorModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 10;
}

QVariant SensorModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags SensorModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() < 3 || index.column() == 8)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QString SensorModel::getToolTip(int row, int column) const
{
    switch(column)
    {
    case 9:
        return sList[row]->getReconList().join(", ");
    default:
        return "No Help Yet.";
    }
}

QVariant SensorModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Class";
    case 2:
        return "Loc";
    case 3:
        return "Cost";
    case 4:
        return "Kills";
    case 5:
        return "Space";
    case 6:
        return "Sensors";
    case 7:
        return "Comm Range";
    case 8:
        return "Eff";
    case 9:
        return "Recon Systems";
    }
    return 0;
}

QVariant SensorModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return sList[row]->getName();
    case 1:
        return sList[row]->getClass();
    case 2:
        return sList[row]->getLoc();
    case 3:
        return sList[row]->getCost();
    case 4:
        return sList[row]->getKills();
    case 5:
        return sList[row]->getSpace();
    case 6:
        return sList[row]->getSenseRange();
    case 7:
        return sList[row]->getCommRange();
    case 8:
        return sList[row]->getEff();
    case 9:
        {
            QStringList recons = sList[row]->getReconList();
            QString newList;

            QStringList::iterator it = recons.begin();

            while (it != recons.end())
            {
                newList += *it;
                ++it;
                if (it != recons.end())
                    newList += ", ";
            }

            return newList;
        }
    }
    return 0;
}

bool SensorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || index.row() >= sList.size())
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

bool SensorModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 0:
        sList[row]->setName(value.toString());
        return true;
    case 1:
        sList[row]->setClass(value.toString());
        sList[row]->setEff(0);
        return true;
    case 2:
        sList[row]->setLoc(value.toString());
        return true;
    case 8:
        {
            double eff = value.toDouble();
            if (eff > (sList[row]->getSpace() + sList[row]->getEff()))
            {
                eff = (sList[row]->getSpace() + sList[row]->getEff());
            }
            sList[row]->setEff(eff);
        }
        return true;
    }
    return false;
}

bool SensorModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool SensorModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
