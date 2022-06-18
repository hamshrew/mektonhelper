#ifndef SPLITENTRY_H
#define SPLITENTRY_H

#include <QWidget>
#include <QModelIndex>

class SplittablePart;
class Mecha;
class QTableView;
class SplittableModel;
class SplitModel;

class SplitEntry : public QWidget
{
    Q_OBJECT
public:
    explicit SplitEntry(Mecha* mecha, QWidget *parent = 0);

signals:
    void splitsUpdated();
public slots:
    void someDataChanged();
    void refresh(Mecha*);
    void newSelect(QModelIndex index);
    void updateTables();
private:
    Mecha* mecha;
    QList<SplittablePart*> splitList;

    QTableView* partsTable;
    QTableView* splitTable;
    SplittableModel* partsModel;
    SplitModel* splitModel;
};

#endif // SPLITENTRY_H
