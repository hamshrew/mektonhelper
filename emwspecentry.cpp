#include "emwspecentry.h"
#include "weapon.h"
#include "emwcomp.h"
#include "mektonfuncs.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>

EMWSpecEntry::EMWSpecEntry(QWidget *parent) :
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
    acc->setRange(-2,3);
    acc->setValue(1);
    damLine->addWidget(label);
    damLine->addWidget(acc);

    label = new QLabel("Attack Factor: ");
    attackFactor = new QSpinBox();
    attackFactor->setRange(0,5);
    attackFactor->setValue(0);
    damLine->addWidget(label);
    damLine->addWidget(attackFactor);

    connect(damage,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(acc,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(attackFactor,SIGNAL(valueChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(damLine);

    QHBoxLayout* emwLine = new QHBoxLayout();
    label = new QLabel(tr("Turns In Use"));
    turns = new QComboBox();
    turns->addItem("1");
    turns->addItem("2");
    turns->addItem("3");
    turns->addItem("4");
    turns->addItem("5");
    turns->addItem("7");
    turns->addItem("10");
    turns->addItem(QChar(0x221e));
    turns->setCurrentIndex(7);
    emwLine->addWidget(label);
    emwLine->addWidget(turns);

    label = new QLabel(tr("Beam Shield"));
    shield = new QCheckBox();
    emwLine->addWidget(label);
    emwLine->addWidget(shield);

    label = new QLabel(tr("Variable"));
    shieldVar = new QCheckBox();
    shieldVar->setDisabled(true);
    emwLine->addWidget(label);
    emwLine->addWidget(shieldVar);

    connect(turns,SIGNAL(currentIndexChanged(int)),SLOT(recalcValues()));
    connect(shield,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(shieldVar,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(emwLine);

    QHBoxLayout* extraLine = new QHBoxLayout();

    label = new QLabel(tr("Rechargeable: "));
    recharge = new QCheckBox();
    recharge->setDisabled(true);
    extraLine->addWidget(label);
    extraLine->addWidget(recharge);

    label = new QLabel(tr("Thrown: "));
    thrown = new QCheckBox();
    extraLine->addWidget(label);
    extraLine->addWidget(thrown);

    label = new QLabel(tr("Quick: "));
    quick = new QCheckBox();
    extraLine->addWidget(label);
    extraLine->addWidget(quick);

    label = new QLabel(tr("Hyper: "));
    hyper = new QCheckBox();
    extraLine->addWidget(label);
    extraLine->addWidget(hyper);

    connect(recharge,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(thrown,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(quick,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(hyper,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(extraLine);

    this->setLayout(lineLayout);
}

EMWSpecEntry::~EMWSpecEntry()
{

}

void EMWSpecEntry::recalcValues()
{
    EMWComp* comp = EMWComp::instance();

    float wCost = comp->calcDamCost(damage->value());

    wCost *= comp->getAccMul(acc->value());
    wCost *= comp->getAFMul(attackFactor->value());
    wCost *= comp->getTIUMul(turns->currentText());

    // fix Beam Shield stuff if needed
    if (shield->isChecked())
    {
        shieldVar->setDisabled(false);
    }
    else
    {
        shieldVar->setChecked(false);
        shieldVar->setDisabled(true);
    }

    // fix Rechargeable if needed
    if (turns->currentIndex() == 7)
    {
        recharge->setDisabled(true);
        recharge->setChecked(false);
    }
    else
    {
        recharge->setDisabled(false);
    }

    // all checkboxes
    wCost *= comp->getShieldMul(shield->isChecked(),shieldVar->isChecked());
    wCost *= comp->getHyperMul(hyper->isChecked());
    wCost *= comp->getRechargeMul(recharge->isChecked());
    wCost *= comp->getThrownMul(thrown->isChecked());
    wCost *= comp->getQuickMul(quick->isChecked());

    // correct wCost to nearest tenth
    wCost = mekround(wCost);

    double spEff = (eff->value() >= wCost) ? wCost : eff->value();

    eff->setValue(spEff);

    cost->setText(QString::number(wCost + mekround(spEff/2.0)));

    space->setText(QString::number(wCost - spEff));
}

Weapon* EMWSpecEntry::createWeaponPtr()
{
    if (weaponName->text().isEmpty())
    {
        return NULL;
    }

    Weapon* wp = new Weapon();

    wp->setType("EMW");
    wp->setName(weaponName->text());
    wp->setAcc((!shield->isChecked() || shieldVar->isChecked()) ? acc->value() : acc->value() - 3);
    wp->setRange(0);
    wp->setDamage(damage->value());
    wp->setShotPrepend("TIU=");
    wp->setShots(turns->currentText());
    wp->setKills(((int)round(damage->value() / 4.0)) ? ((int)round(damage->value() / 4.0)) : 1);    
    wp->setCost(cost->text().toDouble() - mekround(eff->value()*0.5));
    wp->setSpace(space->text().toDouble() + eff->value());
    wp->setEff(eff->value());

    if (attackFactor->value())
        wp->addNote("AF: " + QString::number(attackFactor->value()));
    if (recharge->isChecked())
        wp->addNote("Rechargeable");
    if (thrown->isChecked())
        wp->addNote("Thrown");
    if (quick->isChecked())
        wp->addNote("Quick");
    if (hyper->isChecked())
        wp->addNote("Hyper");
    if (shieldVar->isChecked())
        wp->addNote("Variable Beam Shield: DA=" + QString::number(acc->value() - 3));

    if (!shieldVar->isChecked() && shield->isChecked())
        wp->addNote("Beam Shield");

    return wp;
}
