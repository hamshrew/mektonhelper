#ifndef SIZEDELEGATE_H
#define SIZEDELEGATE_H

#include <QStyledItemDelegate>

class SizeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SizeDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // SIZEDELEGATE_H
