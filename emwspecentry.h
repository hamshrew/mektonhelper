#ifndef EMWSPECENTRY_H
#define EMWSPECENTRY_H

#include "basespecentry.h"

class Weapon;
class QLineEdit;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QCheckBox;

class EMWSpecEntry : public BaseSpecEntry
{
    Q_OBJECT
public:
    explicit EMWSpecEntry(QWidget *parent = 0);
    virtual ~EMWSpecEntry();

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
    QSpinBox* attackFactor;
    QComboBox* turns;
    QCheckBox* shield;
    QCheckBox* shieldVar;
    QCheckBox* recharge;
    QCheckBox* thrown;
    QCheckBox* quick;
    QCheckBox* hyper;
};

#endif // EMWSPECENTRY_H
