#ifndef OPTIONMODEL_H
#define OPTIONMODEL_H

#include <QAbstractTableModel>

class Option;
class Mecha;

class OptionModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit OptionModel(Mecha* mecha, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    Mecha* mecha;
    QList<Option*>& source;

    QVariant getDataByPosition(int row, int column) const;
    bool setDataByPosition(int row, int column, const QVariant &value);

signals:

public slots:

};

#endif // OPTIONMODEL_H
