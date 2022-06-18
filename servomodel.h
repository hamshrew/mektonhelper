#ifndef SERVOMODEL_H
#define SERVOMODEL_H

#include <QAbstractTableModel>
#include <QList>

class Servo;

class ServoModel : public QAbstractTableModel
{
public:
    ServoModel(int rows, int columns, QList<Servo*>& data, QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    int m_rows, m_columns;
    QList<Servo*>& servoList;

    QVariant getDataByPosition(int row, int column) const;
    bool setDataByPosition(int row, int column, const QVariant &value);
};

#endif // SERVOMODEL_H
