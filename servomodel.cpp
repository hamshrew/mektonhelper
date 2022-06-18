#include "servomodel.h"
#include "servo.h"
#include <QComboBox>

ServoModel::ServoModel(int rows, int columns, QList<Servo*>& data, QObject *parent) :
        QAbstractTableModel(parent),
        m_rows(rows),
        m_columns(columns),
        servoList(data)
{
}

int ServoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return servoList.size();
}

int ServoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columns;
}

QVariant ServoModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags ServoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if ((index.column() < 3 && index.column()) || index.column() == 7
        || index.column() == 10 || index.column() == 12
        || index.column() == 14 || index.column() == 6)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ServoModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        return "Size";
    case 3:
        return "Space";
    case 4:
        return "Kills";
    case 5:
        return "Cost";
    case 6:
        return "Mounts";
    case 7:
        return "Armor";
    case 8:
        return "SP";
    case 9:
        return "Armor Wt";
    case 10:
        return "Armor Type";
    case 11:
        return "DC";
    case 12:
        return "RAM";
    case 13:
        return "Armor Cost";
    case 14:
        return "Reinforce";
    }
    return 0;
}

QVariant ServoModel::getDataByPosition(int row, int column) const
{
    switch (column)
    {
    case 0:
        return servoList[row]->getDesignation();
    case 1:
        return servoList[row]->getTypeString();
    case 2:
        return servoList[row]->getSizeString();
    case 3:
        return servoList[row]->getSpace();
    case 4:
        return servoList[row]->getKills();
    case 5:
        return servoList[row]->getCost();
    case 6:
        return servoList[row]->getMounts();
    case 7:
        return servoList[row]->getArmorString();
    case 8:
        return servoList[row]->getArmorProt();
    case 9:
        {QString s;

        s.sprintf("%.1f tons",servoList[row]->getArmorWeight());
        return s;}
    case 10:
        return servoList[row]->getArmorTypeString();
    case 11:
        return ServoComp::instance()->getArmorDC(servoList[row]->getArmorType());
    case 12:
        return servoList[row]->getRAMString();
    case 13:
        return servoList[row]->getArmorCost();
    case 14:
        return servoList[row]->getReinforce();
    }
    return 0;
}

bool ServoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || index.row() >= servoList.size())
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

bool ServoModel::setDataByPosition(int row, int column, const QVariant &value)
{
    switch (column)
    {
    case 0:
        {Servo* s;
        foreach (s,servoList)
        {
            if (s->getDesignation() == value.toString())
            {
                return false;
            }
        }

        servoList[row]->setDesignation(value.toString());
        return true;}
    case 1:
        servoList[row]->setType((ServoTypes)value.toInt());
        return true;
    case 2:
        servoList[row]->setSize((ServoSizes)value.toInt());
        return true;
    case 6:
        servoList[row]->setMounts(value.toInt());
        return true;
    case 7:
        servoList[row]->setArmorSize((ServoSizes)value.toInt());
        return true;
    case 10:
        servoList[row]->setArmorType((ArmorTypes)value.toInt());
        return true;
    case 12:
        servoList[row]->setRAM((RAMTypes)value.toInt());
        return true;
    case 14:
        servoList[row]->setReinforce(value.toInt());
        return true;
    }
    return false;
}

bool ServoModel::insertRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), row, row+count-1);

    endInsertRows();
    return true;
}

bool ServoModel::removeRows(int row, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), row, row+count-1);

    endRemoveRows();
    return true;
}
