#include "projspecentry.h"
#include "weapon.h"
#include "projcomp.h"
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

ProjSpecEntry::ProjSpecEntry(QWidget *parent) :
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
    cost->setText("1");
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
    acc->setRange(-2,2);
    acc->setValue(0);
    damLine->addWidget(label);
    damLine->addWidget(acc);

    label = new QLabel(tr("Range Mod: "));
    rangeMod = new QComboBox();
    rangeMod->addItems(ProjComp::instance()->getRangeList());
    rangeMod->setCurrentIndex(4);
    damLine->addWidget(label);
    damLine->addWidget(rangeMod);

    label = new QLabel(tr("Range: "));
    range = new QLabel("3");
    damLine->addWidget(label);
    damLine->addWidget(range);

    connect(damage,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(acc,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(rangeMod,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(damLine);

    QHBoxLayout* handLine = new QHBoxLayout;
    label = new QLabel(tr("Burst Value: "));
    BV = new QSpinBox();
    BV->setRange(1,8);
    BV->setValue(1);
    handLine->addWidget(label);
    handLine->addWidget(BV);

    label = new QLabel("Multi Feed: ");
    multi = new QSpinBox();
    multi->setRange(1,4);
    multi->setValue(1);
    handLine->addWidget(label);
    handLine->addWidget(multi);

    label = new QLabel(tr("Long Range: "));
    LR = new QCheckBox();
    handLine->addWidget(label);
    handLine->addWidget(LR);

    label = new QLabel(tr("Hypervelocity: "));
    HV = new QCheckBox();
    handLine->addWidget(label);
    handLine->addWidget(HV);

    connect(BV,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(multi,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(LR,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(HV,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(handLine);

    QHBoxLayout* checkLine = new QHBoxLayout;

    label = new QLabel(tr("Phalanx: "));
    phalanx = new QCheckBox();
    checkLine->addWidget(label);
    checkLine->addWidget(phalanx);

    label = new QLabel(tr("Anti-Personnel: "));
    antiPerson = new QCheckBox();
    checkLine->addWidget(label);
    checkLine->addWidget(antiPerson);

    label = new QLabel(tr("Anti-Mecha: "));
    antiMech = new QCheckBox();
    checkLine->addWidget(label);
    checkLine->addWidget(antiMech);

    connect(phalanx,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(antiPerson,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(antiMech,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(checkLine);

    this->setLayout(lineLayout);
}

ProjSpecEntry::~ProjSpecEntry()
{

}

void ProjSpecEntry::recalcValues()
{
    ProjComp* comp = ProjComp::instance();

    float wCost = comp->calcDamCost(damage->value());

    wCost *= comp->getAccMul(acc->value());
    wCost *= comp->getRangeMul(rangeMod->currentText());
    wCost *= comp->getMultiMul(multi->value());
    wCost *= comp->getBVMul(BV->value());

    // fix Phalanx Options if needed
    if (antiPerson->isChecked() || phalanx->isChecked())
    {
        antiMech->setDisabled(false);
    }
    else
    {
        antiMech->setDisabled(true);
        if (!antiMech->isChecked())
        {
            antiMech->setChecked(true);
        }
    }

    // all checkboxes
    wCost *= comp->getLRMul(LR->isChecked());
    wCost *= comp->getHVMul(HV->isChecked());
    wCost *= comp->getVariableMul(phalanx->isChecked(),antiPerson->isChecked(),antiMech->isChecked());

    // correct wCost to nearest tenth
    wCost = mekround(wCost);

    double spEff = (eff->value() >= wCost) ? wCost : eff->value();

    eff->setValue(spEff);

    cost->setText(QString::number(wCost+ mekround(spEff/2.0)));

    space->setText(QString::number((wCost) - spEff));

    range->setText(QString::number(round(comp->calcRange(damage->value()) * (rangeMod->currentText().section('%',0,0).toInt() / 100.0))));
}

Weapon* ProjSpecEntry::createWeaponPtr()
{
    if (weaponName->text().isEmpty())
    {
        return NULL;
    }

    Weapon* wp = new Weapon();

    wp->setType("Projectile");
    wp->setName(weaponName->text());
    wp->setAcc(acc->value());
    wp->setRange(range->text().toInt());
    wp->setDamage(damage->value());
    wp->setShots("-");
    wp->setKills(damage->value());
    wp->setCost(cost->text().toDouble() - mekround(eff->value()*0.5));
    wp->setSpace(space->text().toDouble() + eff->value());
    wp->setEff(eff->value());
    wp->setUseAmmo(true);

    if (LR->isChecked())
        wp->addNote("Long Range");
    if (HV->isChecked())
        wp->addNote("Hypervelocity");

    if (phalanx->isChecked() && antiMech->isChecked() && antiPerson->isChecked())
    {
        wp->addNote("All-Purpose");
    }
    else if (antiMech->isChecked())
    {
        if (phalanx->isChecked())
        {
            wp->addNote("Variable Phalanx");
        }
        else if (antiPerson->isChecked())
        {
            wp->addNote("Variable Anti-Personnel");
        }
    }
    else if (antiPerson->isChecked() && phalanx->isChecked())
    {
        wp->addNote("Anti-Personnel/Phalanx");
    }
    else if (antiPerson->isChecked())
    {
        wp->addNote("Anti-Personnel");
    }
    else if (phalanx->isChecked())
    {
        wp->addNote("Phalanx");
    }

    return wp;
}
