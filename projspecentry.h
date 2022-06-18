#ifndef PROJSPECENTRY_H
#define PROJSPECENTRY_H

#include "basespecentry.h"

class Weapon;
class QLineEdit;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QCheckBox;

class ProjSpecEntry : public BaseSpecEntry
{
    Q_OBJECT
public:
    explicit ProjSpecEntry(QWidget *parent = 0);
    virtual ~ProjSpecEntry();

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
    QSpinBox* multi;
    QComboBox* rangeMod;
    QLabel* range;
    QSpinBox* BV;

    QCheckBox* LR;
    QCheckBox* HV;
    QCheckBox* phalanx;
    QCheckBox* antiPerson;
    QCheckBox* antiMech;
};

#endif // PROJSPECENTRY_H
