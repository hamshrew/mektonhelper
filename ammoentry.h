#ifndef AMMOENTRY_H
#define AMMOENTRY_H

#include <QWidget>
#include <QHash>
#include <QString>
#include "weapon.h"

class QTableView;
class QComboBox;
class QLabel;
class QSpinBox;
class QCheckBox;
class QGroupBox;
class QHBoxLayout;
class Mecha;
class AmmoModel;
class LocListDelegate;

class AmmoEntry : public QWidget
{
    Q_OBJECT
public:
    explicit AmmoEntry(Mecha* mecha, QWidget *parent = 0);
    ~AmmoEntry();

signals:
    void updatedAmmo();
public slots:
    void someDataChanged();
    void weaponsUpdated();
    void selectType();
    void updateCosts();
    void addAmmo();
    void remAmmo();
    void refresh(Mecha* newMecha);
private:
    Mecha* mecha;
    QHash<QString,Weapon> wpHash;
    QTableView* ammoTable;

    AmmoModel* model;

    LocListDelegate* locDelegate;

    QComboBox* weaponChoice;
    QLabel* clipCost;
    QLabel* clipSpace;
    QSpinBox* numShots;
    QSpinBox* numClips;

    // All Ammo Options
    QComboBox* ammoBase;
    QCheckBox* tracer;
    QCheckBox* kinetic;
    QCheckBox* tangler;
    QCheckBox* ap;
    QCheckBox* disruptor;
    QCheckBox* incendiary;
    QCheckBox* scatter;
    QComboBox* shock;
    QSpinBox* blast;
    QCheckBox* nuke;

    void refillWeaponBox();
    QGroupBox* buildOptionBox();
    void enableAmmo(bool enable);
};

#endif // AMMOENTRY_H
