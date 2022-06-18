#ifndef BEAMSPECENTRY_H
#define BEAMSPECENTRY_H

#include "basespecentry.h"

class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QCheckBox;
class QLabel;

class Weapon;

class BeamSpecEntry : public BaseSpecEntry
{
    Q_OBJECT
public:
    explicit BeamSpecEntry(QWidget *parent = 0);
    virtual ~BeamSpecEntry();
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
    QComboBox* rangeMod;
    QLabel* range;
    QSpinBox* damage;
    QComboBox* shots;
    QSpinBox* warmUp;
    QComboBox* burstValue;
    QComboBox* wideAngle;
    QCheckBox* clipFed;
    QCheckBox* antiMissile;
    QCheckBox* antiPersonnel;
    QCheckBox* antiMekton;
    QCheckBox* fragile;
    QCheckBox* longRange;
    QCheckBox* hydro;
    QCheckBox* megaBeam;
    QCheckBox* disruptor;

};

#endif // BEAMSPECENTRY_H
