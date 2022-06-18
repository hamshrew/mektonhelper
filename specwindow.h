#ifndef SPECWINDOW_H
#define SPECWINDOW_H

#include <QWidget>
#include <QList>
#include "weapon.h"

class QComboBox;
class QGroupBox;
class QTableView;
class QLineEdit;
class QVBoxLayout;
class SpecModel;
class BaseSpecEntry;
class QTextEdit;

class SpecWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SpecWindow(QWidget *parent = 0);

signals:

public slots:
    void addCurrentWeapon();
    void loadNewSpec();
    void writeNewSpec();
    void removeWeapon();
    void changeEntryType(int type);

private:
    QComboBox* selectType;
    QGroupBox* specBox;
    BaseSpecEntry* wSpec;
    QTableView* specTable;
    QList<Weapon> weaponList;
    SpecModel* model;
    QLineEdit* specTitle;
    QLineEdit* specAuthor;
    QVBoxLayout* needLayout;
    QTextEdit* commentBox;

    QString currentSpecName;
};

#endif // SPECWINDOW_H
