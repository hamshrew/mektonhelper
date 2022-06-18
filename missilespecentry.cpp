#include "missilespecentry.h"
#include "weapon.h"
#include "missilecomp.h"
#include "mektonfuncs.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>

MissileSpecEntry::MissileSpecEntry(QWidget *parent) :
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
    cost->setText("0.1");
    baseLine->addWidget(label);
    baseLine->addWidget(cost);

    label = new QLabel("Space: ");
    space = new QLabel();
    space->setText("1");
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
    acc->setValue(0);
    damLine->addWidget(label);
    damLine->addWidget(acc);

    label = new QLabel(tr("Range Mod: "));
    rangeMod = new QComboBox();
    rangeMod->addItems(MissileComp::instance()->getRangeList());
    rangeMod->setCurrentIndex(4);
    damLine->addWidget(label);
    damLine->addWidget(rangeMod);

    label = new QLabel(tr("Range: "));
    range = new QLabel("4");
    damLine->addWidget(label);
    damLine->addWidget(range);

    label = new QLabel("Shots: ");
    shots = new QSpinBox();
    shots->setRange(1,5000);
    shots->setValue(1);
    damLine->addWidget(label);
    damLine->addWidget(shots);

    connect(damage,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(acc,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(rangeMod,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));
    connect(shots,SIGNAL(valueChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(damLine);

    QHBoxLayout* handLine = new QHBoxLayout;

    label = new QLabel(tr("Smart: "));
    smart = new QSpinBox();
    smart->setRange(0,4);
    smart->setValue(0);
    handLine->addWidget(label);
    handLine->addWidget(smart);

    label = new QLabel(tr("Skill: "));
    skill = new QComboBox();
    skill->addItems(MissileComp::instance()->getSkillList());
    skill->setCurrentIndex(0);
    handLine->addWidget(label);
    handLine->addWidget(skill);

    label = new QLabel(tr("BR: "));
    BR = new QComboBox();
    BR->addItems(MissileComp::instance()->getBRList());
    handLine->addWidget(label);
    handLine->addWidget(BR);

    label = new QLabel(tr("CounterMissile: "));
    counter = new QCheckBox();
    handLine->addWidget(label);
    handLine->addWidget(counter);

    label = new QLabel(tr("Variable: "));
    counterVar = new QCheckBox();
    handLine->addWidget(label);
    handLine->addWidget(counterVar);

    connect(smart,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(skill,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));
    connect(BR,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));
    connect(counter,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(counterVar,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(handLine);

    QHBoxLayout* checkLine = new QHBoxLayout;

    label = new QLabel(tr("Long Range: "));
    LR = new QCheckBox();
    checkLine->addWidget(label);
    checkLine->addWidget(LR);

    label = new QLabel(tr("Hypervelocity: "));
    HV = new QCheckBox();
    checkLine->addWidget(label);
    checkLine->addWidget(HV);

    label = new QLabel(tr("Fuse: "));
    fuse = new QCheckBox();
    checkLine->addWidget(label);
    checkLine->addWidget(fuse);

    label = new QLabel(tr("Nuclear: "));
    nuke = new QCheckBox();
    nuke->setDisabled(true);
    checkLine->addWidget(label);
    checkLine->addWidget(nuke);

    connect(LR,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(HV,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(fuse,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(nuke,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(checkLine);

    QHBoxLayout* lastLine = new QHBoxLayout;

    label = new QLabel(tr("Foam: "));
    foam = new QCheckBox();
    lastLine->addWidget(label);
    lastLine->addWidget(foam);

    label = new QLabel(tr("Flare: "));
    flare = new QCheckBox();
    lastLine->addWidget(label);
    lastLine->addWidget(flare);

    label = new QLabel(tr("Smoke: "));
    smoke = new QCheckBox();
    lastLine->addWidget(label);
    lastLine->addWidget(smoke);

    label = new QLabel(tr("Scatter: "));
    scatter = new QCheckBox();
    lastLine->addWidget(label);
    lastLine->addWidget(scatter);

    connect(foam,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(flare,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(smoke,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(scatter,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(lastLine);

    this->setLayout(lineLayout);
}

MissileSpecEntry::~MissileSpecEntry()
{

}

void MissileSpecEntry::recalcValues()
{
    MissileComp* comp = MissileComp::instance();

    float wCost = comp->calcDamCost(damage->value());

    wCost *= comp->getAccMul(acc->value());
    wCost *= comp->getRangeMul(rangeMod->currentText());
    wCost *= comp->getSmartMul(smart->value());
    if (smart->value())
        wCost *= comp->getSkillMul(skill->currentText());

    wCost *= comp->getBRMul(BR->currentText().toInt());

    // fix Nuclear stuff if needed
    if (BR->currentIndex())
    {
        nuke->setDisabled(false);
    }
    else
    {
        nuke->setChecked(false);
        nuke->setDisabled(true);
    }

    // fix CounterMissile if needed
    if (!counter->isChecked())
    {
        counterVar->setDisabled(true);
        counterVar->setChecked(false);
    }
    else
    {
        counterVar->setDisabled(false);
    }

    // all checkboxes
    wCost *= comp->getLRMul(LR->isChecked());
    wCost *= comp->getHVMul(HV->isChecked());
    wCost *= comp->getFuseMul(fuse->isChecked());
    wCost *= comp->getFoamMul(foam->isChecked());
    wCost *= comp->getFlareMul(flare->isChecked());
    wCost *= comp->getSmokeScatterMul(smoke->isChecked(),scatter->isChecked());
    wCost *= comp->getCounterMul(counter->isChecked(),counterVar->isChecked());

    wCost *= shots->value();

    // correct wCost to nearest tenth
    wCost = mekround(wCost);

    double spEff = (eff->value() >= wCost) ? wCost : eff->value();

    eff->setValue(spEff);

    cost->setText(QString::number((wCost * comp->getNuclearMul(nuke->isChecked()))+ mekround(spEff/2.0)));

    space->setText(QString::number((wCost) - spEff));

    range->setText(QString::number(round(comp->calcRange(damage->value()) * (rangeMod->currentText().section('%',0,0).toInt() / 100.0))));
}

Weapon* MissileSpecEntry::createWeaponPtr()
{
    if (weaponName->text().isEmpty())
    {
        return NULL;
    }

    Weapon* wp = new Weapon();

    wp->setType("Missile");
    wp->setName(weaponName->text());
    wp->setAcc(acc->value());
    wp->setRange(range->text().toInt());
    wp->setDamage(damage->value());
    wp->setShots(QString::number(shots->value()));
    wp->setKills(damage->value() * shots->value() / 15.0);
    if (wp->getKills() < 1)
        wp->setKills(1);
    wp->setCost(cost->text().toDouble() - mekround(eff->value()*0.5));
    wp->setSpace(space->text().toDouble() + eff->value());
    wp->setEff(eff->value());

    if (LR->isChecked())
        wp->addNote("Long Range");
    if (HV->isChecked())
        wp->addNote("Hypervelocity");
    if (fuse->isChecked())
        wp->addNote("Fuse");
    if (nuke->isChecked())
        wp->addNote("Nuclear");
    if (foam->isChecked())
        wp->addNote("Foam");
    if (flare->isChecked())
        wp->addNote("Flare");
    if (smoke->isChecked())
        wp->addNote("Smoke");
    if (scatter->isChecked())
        wp->addNote("Scatter");
    if (counter->isChecked())
    {
        if (counterVar->isChecked())
        {
            wp->addNote("Variable CounterMissile");
        }
        else wp->addNote("CounterMissile");
    }
    if (BR->currentIndex())
    {
        wp->addNote("BV=" + MissileComp::instance()->getBRList().at(BR->currentIndex()));
    }

    return wp;
}
