#ifndef LOCLISTDELEGATE_H
#define LOCLISTDELEGATE_H

#include <QStyledItemDelegate>

class Mecha;

class LocListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LocListDelegate(Mecha* mecha,bool useHands = true, bool useMounts = true, bool useBinders = true, bool useSplit = true, QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

private:
    Mecha* mecha;
    bool useHands;
    bool useMounts;
    bool useBinders;
    bool useSplit;
};

#endif // LOCLISTDELEGATE_H
