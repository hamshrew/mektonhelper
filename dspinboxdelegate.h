#ifndef DSPINBOXDELEGATE_H
#define DSPINBOXDELEGATE_H

#include <QStyledItemDelegate>

class DSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DSpinBoxDelegate(double min, double max, int prec = 1, QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

private:
    double min, max;
    int prec;
};

#endif // DSPINBOXDELEGATE_H
