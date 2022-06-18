#ifndef MATEDELEGATE_H
#define MATEDELEGATE_H

#include <QStyledItemDelegate>

class Mecha;

class MateDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MateDelegate(Mecha* mecha,QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    Mecha* mecha;

signals:

public slots:

};

#endif // MATEDELEGATE_H
