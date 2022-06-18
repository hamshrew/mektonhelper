#ifndef ARMORDELEGATE_H
#define ARMORDELEGATE_H

#include <QStyledItemDelegate>

class ArmorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ArmorDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // ARMORDELEGATE_H
