#ifndef RAMDELEGATE_H
#define RAMDELEGATE_H

#include <QStyledItemDelegate>

class RAMDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RAMDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // RAMDELEGATE_H
