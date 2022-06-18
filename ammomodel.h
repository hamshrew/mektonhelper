#ifndef AMMOMODEL_H
#define AMMOMODEL_H

#include <QAbstractTableModel>
#include <QList>

class Ammo;
class Weapon;

class AmmoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AmmoModel(QList<Ammo*>& data, QHash<QString,Weapon>& wpHash,QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void update();

private:
    QList<Ammo*>& ammoList;
    QHash<QString,Weapon>& wpHash;

    QVariant getDataByPosition(int row, int column) const;
    bool setDataByPosition(int row, int column, const QVariant &value);

signals:

public slots:

};

#endif // AMMOMODEL_H
