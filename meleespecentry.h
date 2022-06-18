#ifndef MELEESPECENTRY_H
#define MELEESPECENTRY_H

#include "basespecentry.h"

class Weapon;
class QLineEdit;
class QLabel;
class QSpinBox;
class QComboBox;
class QCheckBox;
class QDoubleSpinBox;

class MeleeSpecEntry : public BaseSpecEntry
{
    Q_OBJECT
public:
    explicit MeleeSpecEntry(QWidget *parent = 0);
    virtual ~MeleeSpecEntry();

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
    QSpinBox* range;
    QCheckBox* handy;
    QCheckBox* clumsy;
    QCheckBox* entangle;
    QCheckBox* disruptor;
    QCheckBox* quick;
    QCheckBox* ap;
    QCheckBox* shock;
    QCheckBox* shockAdd;
    QCheckBox* thrown;
    QCheckBox* returns;
};

#endif // MELEESPECENTRY_H
