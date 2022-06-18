#ifndef TYPEDELEGATE_H
#define TYPEDELEGATE_H

#include <QStyledItemDelegate>

class TypeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TypeDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // TYPEDELEGATE_H
