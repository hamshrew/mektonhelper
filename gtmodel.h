#ifndef GTMODEL_H
#define GTMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class Mecha;

class GTModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GTModel(Mecha* mecha,const QString& type, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void setType(const QString& newVal) {type = newVal;}

private:
    Mecha* mecha;
    QStringList locs;
    QString type;

    QVariant getDataByPosition(int row, int column) const;
    bool setDataByPosition(int row, int column, const QVariant &value);
signals:

public slots:

};

#endif // GTMODEL_H
