#ifndef MATEDENTRY_H
#define MATEDENTRY_H

#include <QWidget>

class Mecha;
class QTableView;
class QGroupBox;
class QLineEdit;
class MatedModel;
class LocListDelegate;

class MatedEntry : public QWidget
{
    Q_OBJECT
public:
    explicit MatedEntry(Mecha* mecha,QWidget *parent = 0);

signals:
    void matedChange();

public slots:
    void refresh(Mecha* newMech);
    void addSystem();
    void remSystem();
    void someDataChanged();

private:
    Mecha* mecha;
    QTableView* matedTable;
    MatedModel* matedModel;
    QLineEdit* sysName;

    QGroupBox* buildMatedBox();
};

#endif // MATEDENTRY_H
