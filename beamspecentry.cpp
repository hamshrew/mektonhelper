#include "beamspecentry.h"
#include "beamcomp.h"
#include "weapon.h"
#include "mektonfuncs.h"

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDoubleSpinBox>

#include <cmath>

BeamSpecEntry::BeamSpecEntry(QWidget *parent) :
    BaseSpecEntry(parent)
{
    QVBoxLayout* lineLayout = new QVBoxLayout();

    QHBoxLayout* baseLine = new QHBoxLayout();

    weaponName = new QLineEdit();
    weaponName->setText("");

    QLabel* label = new QLabel("Name: ");
    baseLine->addWidget(label);
    baseLine->addWidget(weaponName);

    label = new QLabel("Cost: ");
    cost = new QLabel();
    cost->setText("1.5");
    baseLine->addWidget(label);
    baseLine->addWidget(cost);

    label = new QLabel("Space: ");
    space = new QLabel();
    space->setText("2");
    baseLine->addWidget(label);
    baseLine->addWidget(space);

    label = new QLabel(tr("Eff: "));
    eff = new QDoubleSpinBox();
    eff->setRange(0,500);
    eff->setValue(0);
    eff->setDecimals(1);
    eff->setSingleStep(0.1);
    baseLine->addWidget(label);
    baseLine->addWidget(eff);

    connect(eff,SIGNAL(valueChanged(double)),SLOT(recalcValues()));

    lineLayout->addLayout(baseLine);

    QHBoxLayout* damLine = new QHBoxLayout();

    label = new QLabel("Damage: ");
    damage = new QSpinBox();
    damage->setMinimum(1);
    damage->setMaximum(20);
    damage->setValue(1);
    damLine->addWidget(label);
    damLine->addWidget(damage);

    label = new QLabel("Accuracy: ");
    acc = new QSpinBox();
    acc->setRange(-2,3);
    acc->setValue(1);
    damLine->addWidget(label);
    damLine->addWidget(acc);

    label = new QLabel("Range: ");
    range = new QLabel();
    range->setText("4");
    damLine->addWidget(label);
    damLine->addWidget(range);

    label = new QLabel("Range Mod: ");
    rangeMod = new QComboBox();
    rangeMod->addItem("25%");
    rangeMod->addItem("50%");
    rangeMod->addItem("75%");
    rangeMod->addItem("100%");
    rangeMod->addItem("125%");
    rangeMod->addItem("150%");
    rangeMod->addItem("175%");
    rangeMod->addItem("200%");
    rangeMod->addItem("250%");
    rangeMod->addItem("300%");
    rangeMod->setCurrentIndex(3);
    damLine->addWidget(label);
    damLine->addWidget(rangeMod);

    connect(damage,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(rangeMod,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));
    connect(acc,SIGNAL(valueChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(damLine);

    QHBoxLayout* shotsLine = new QHBoxLayout();

    label = new QLabel("Warm Up: ");
    warmUp = new QSpinBox();
    warmUp->setRange(0,3);
    warmUp->setValue(0);
    shotsLine->addWidget(label);
    shotsLine->addWidget(warmUp);

    label = new QLabel("Shots: ");
    shots = new QComboBox();
    shots->addItem("0");
    shots->addItem("1");
    shots->addItem("2");
    shots->addItem("3");
    shots->addItem("5");
    shots->addItem("10");
    shots->addItem(QString(QChar(0x221e)));
    shots->setCurrentIndex(6);
    shotsLine->addWidget(label);
    shotsLine->addWidget(shots);

    label = new QLabel("Wide Angle: ");
    wideAngle = new QComboBox();
    wideAngle->addItem("None");;
    wideAngle->addItem("Hex");
    wideAngle->addItem("60");
    wideAngle->addItem("180");
    wideAngle->addItem("300");
    wideAngle->addItem("360");
    wideAngle->setCurrentIndex(0);
    shotsLine->addWidget(label);
    shotsLine->addWidget(wideAngle);

    label = new QLabel("Burst Value: ");
    burstValue = new QComboBox();
    burstValue->addItem("None");
    burstValue->addItem("2");
    burstValue->addItem("3");
    burstValue->addItem("4");
    burstValue->addItem("5");
    burstValue->addItem("6");
    burstValue->addItem("7");
    burstValue->addItem("8");
    burstValue->addItem(QString(QChar(0x221e)));
    burstValue->setCurrentIndex(0);
    shotsLine->addWidget(label);
    shotsLine->addWidget(burstValue);

    connect(warmUp,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(shots,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));
    connect(wideAngle,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));
    connect(burstValue,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(shotsLine);

    QHBoxLayout* varLine = new QHBoxLayout();

    label = new QLabel(tr("Clip-Fed: "));
    clipFed = new QCheckBox();
    clipFed->setCheckState(Qt::Unchecked);
    varLine->addWidget(label);
    varLine->addWidget(clipFed);

    label = new QLabel(tr("Anti-Missile: "));
    antiMissile = new QCheckBox();
    antiMissile->setChecked(false);
    varLine->addWidget(label);
    varLine->addWidget(antiMissile);

    label = new QLabel(tr("Anti-Personnel: "));
    antiPersonnel = new QCheckBox();
    antiPersonnel->setChecked(false);
    varLine->addWidget(label);
    varLine->addWidget(antiPersonnel);

    label = new QLabel(tr("Anti-Mecha: "));
    antiMekton = new QCheckBox();
    antiMekton->setChecked(true);
    antiMekton->setDisabled(true);
    varLine->addWidget(label);
    varLine->addWidget(antiMekton);

    connect(clipFed,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(antiMissile,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(antiPersonnel,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(antiMekton,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(varLine);

    QHBoxLayout* miscLine = new QHBoxLayout();

    label = new QLabel(tr("Fragile: "));
    fragile = new QCheckBox();
    miscLine->addWidget(label);
    miscLine->addWidget(fragile);

    label = new QLabel(tr("Long Range: "));
    longRange = new QCheckBox();
    miscLine->addWidget(label);
    miscLine->addWidget(longRange);

    label = new QLabel(tr("Hydro: "));
    hydro = new QCheckBox();
    hydro->setDisabled(true);
    miscLine->addWidget(label);
    miscLine->addWidget(hydro);

    label = new QLabel(tr("Mega-Beam: "));
    megaBeam = new QCheckBox();
    miscLine->addWidget(label);
    miscLine->addWidget(megaBeam);

    label = new QLabel(tr("Disruptor: "));
    disruptor = new QCheckBox();
    miscLine->addWidget(label);
    miscLine->addWidget(disruptor);

    connect(longRange,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(hydro,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(megaBeam,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(disruptor,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(miscLine);

    this->setLayout(lineLayout);
}

BeamSpecEntry::~BeamSpecEntry()
{

}

void BeamSpecEntry::recalcValues()
{
    BeamComp* comp = BeamComp::instance();

    float wCost = comp->calcDamCost(damage->value());

    wCost *= comp->getRangeModMul(rangeMod->currentIndex());
    wCost *= comp->getAccMul(acc->value());
    wCost *= comp->getWarmUpMul(warmUp->value());
    wCost *= comp->getShotsMul(shots->currentText());
    wCost *= comp->getWideAngleMul(wideAngle->currentText());
    wCost *= comp->getBVMul(burstValue->currentText());
    wCost *= comp->getClipMul(clipFed->isChecked());

    if (antiPersonnel->isChecked() || antiMissile->isChecked())
    {
        antiMekton->setDisabled(false);
    }
    else
    {
        antiMekton->setDisabled(true);
        if (!antiMekton->isChecked())
        {
            antiMekton->setChecked(true);
        }
    }

    wCost *= comp->getVarMul(antiMissile->isChecked(),antiPersonnel->isChecked(),antiMekton->isChecked());

    if (burstValue->currentIndex() == 8 && shots->currentIndex() != 6)
    {
        hydro->setDisabled(false);
    }
    else
    {
        hydro->setDisabled(true);
        if (hydro->isChecked())
        {
            hydro->setChecked(false);
        }
    }

    wCost *= comp->getLRMul(longRange->isChecked());
    wCost *= comp->getHydroMul(hydro->isChecked());
    wCost *= comp->getMegaBeamMul(megaBeam->isChecked());
    wCost *= comp->getDisMul(disruptor->isChecked());

    // correct wCost to nearest tenth
    wCost = mekround(wCost);

    double spEff = (eff->value() >= wCost) ? wCost : eff->value();

    eff->setValue(spEff);

    range->setText(QString::number(comp->calcRange(damage->value(),rangeMod->currentIndex())));

    cost->setText(QString::number(wCost + mekround(spEff/2.0)));

    space->setText(QString::number(wCost - spEff));
}

Weapon* BeamSpecEntry::createWeaponPtr()
{
    if (weaponName->text().isEmpty())
    {
        return NULL;
    }
    Weapon* weapon = new Weapon();

    weapon->setType("Beam");
    weapon->setName(weaponName->text());
    weapon->setAcc(acc->value());
    weapon->setRange(range->text().toUInt());
    weapon->setDamage(damage->text().toUInt());
    weapon->setShots(shots->currentText());
    weapon->setKills(fragile->isChecked() ? 1 : damage->value());
    weapon->setCost(cost->text().toDouble() - mekround(eff->value()*0.5));
    weapon->setSpace(space->text().toDouble() + eff->value());
    weapon->setEff(eff->value());

    if (warmUp->value())
        weapon->addNote("Warm Up: " + QString::number(warmUp->value()));
    if (wideAngle->currentIndex())
        weapon->addNote("Wide Angle: " + wideAngle->currentText());
    if (burstValue->currentIndex())
        weapon->addNote("BV=" + burstValue->currentText());
    if (clipFed->isChecked())
    {
        weapon->addNote("Clip-Fed");
        weapon->setUseClips(true);
    }
    if (fragile->isChecked())
        weapon->addNote("Fragile");
    if (longRange->isChecked())
        weapon->addNote("Long Range");
    if (hydro->isChecked())
        weapon->addNote("Hydro");
    if (megaBeam->isChecked())
        weapon->addNote("Mega-Beam");
    if (disruptor->isChecked())
        weapon->addNote("Disruptor");

    if (antiMissile->isChecked() && antiPersonnel->isChecked() && antiMekton->isChecked())
    {
        weapon->addNote("All-Purpose");
    }
    else if (!antiMekton->isChecked())
    {
        if (antiMissile->isChecked())
            weapon->addNote("Anti-Missle");
        if (antiPersonnel->isChecked())
            weapon->addNote("Anti-Personnel");
    }
    else if (antiMekton->isChecked() && (antiMissile->isChecked() || antiPersonnel->isChecked()))
    {
        if (antiMissile->isChecked())
            weapon->addNote("Variable Anti-Missle");
        if (antiPersonnel->isChecked())
            weapon->addNote("Variable Anti-Personnel");
    }

    return weapon;
}
