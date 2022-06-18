#ifndef POOLWEAPONMODEL_H
#define POOLWEAPONMODEL_H

#include <QAbstractTableModel>

class Mecha;
class Weapon;

class PoolWeaponModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PoolWeaponModel(Mecha* mecha, QList<Weapon*>& data, QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void update();
    QString getToolTip(int row,int column) const;

private:
    Mecha* mecha;
    QList<Weapon*>& weaponList;

    QVariant getDataByPosition(int row, int column) const;
    bool setDataByPosition(int row, int column, const QVariant &value);
signals:

public slots:

};

#endif // POOLWEAPONMODEL_H
