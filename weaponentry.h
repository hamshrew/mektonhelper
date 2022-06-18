#ifndef WEAPONENTRY_H
#define WEAPONENTRY_H

#include "weapon.h"

#include <QWidget>
#include <QList>

class Mecha;
class QTableView;
class QPushButton;
class WeaponModel;
class SpecModel;
class LocListDelegate;

class WeaponEntry : public QWidget
{
    Q_OBJECT
public:
    explicit WeaponEntry(Mecha* mecha,QWidget *parent = 0);

signals:
    void updatedWeapons();
public slots:
    void someDataChanged();
    void openSpecFile();
    void createSpecWindow();
    void addSelectedWeapon();
    void removeSelectedWeapon();
    void refresh(Mecha* newMech);
    void linkWeapons();
    void crossLinkWeapons();
    void unlinkWeapons();
private:
    Mecha* mecha;
    QTableView* weaponTable;
    QTableView* specTable;

    WeaponModel* model;
    SpecModel* specModel;

    LocListDelegate* locDelegate;

    QList<Weapon> specList;

    QString currentSpec;

    void loadFile(const QString& filename);
};

#endif // WEAPONENTRY_H
