#ifndef SPECMODEL_H
#define SPECMODEL_H

#include <QAbstractTableModel>
#include "weapon.h"

class SpecModel : public QAbstractTableModel
{
public:
    SpecModel(QList<Weapon>& data, QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QString getToolTip(int row,int column) const;

private:
    QList<Weapon>& weaponList;

    QVariant getDataByPosition(int row, int column) const;
};

#endif // SPECMODEL_H
