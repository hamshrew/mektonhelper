#ifndef MISSILESPECENTRY_H
#define MISSILESPECENTRY_H

#include "basespecentry.h"

class Weapon;
class QLineEdit;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QCheckBox;

class MissileSpecEntry : public BaseSpecEntry
{
    Q_OBJECT
public:
    explicit MissileSpecEntry(QWidget *parent = 0);
    virtual ~MissileSpecEntry();

    Weapon* createWeaponPtr();

signals:

public slots:
    void recalcValues();
private:
    QLineEdit* weaponName;
    QLabel* cost;
    QLabel* space;
    QDoubleSpinBox* eff;
    QSpinBox* acc;
    QSpinBox* damage;
    QSpinBox* shots;
    QComboBox* rangeMod;
    QLabel* range;
    QSpinBox* smart;
    QComboBox* skill;
    QComboBox* BR;

    QCheckBox* LR;
    QCheckBox* foam;
    QCheckBox* HV;
    QCheckBox* flare;
    QCheckBox* fuse;
    QCheckBox* smoke;
    QCheckBox* scatter;
    QCheckBox* counter;
    QCheckBox* counterVar;
    QCheckBox* nuke;
};

#endif // MISSILESPECENTRY_H
