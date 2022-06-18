#include "meleespecentry.h"
#include "weapon.h"
#include "meleecomp.h"
#include "mektonfuncs.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QDoubleSpinBox>

MeleeSpecEntry::MeleeSpecEntry(QWidget *parent) :
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

    label = new QLabel(tr("Thrown: "));
    thrown = new QCheckBox();
    damLine->addWidget(label);
    damLine->addWidget(thrown);

    label = new QLabel(tr("Returning: "));
    returns = new QCheckBox();
    returns->setDisabled(true);
    damLine->addWidget(label);
    damLine->addWidget(returns);

    connect(damage,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(acc,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(thrown,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(returns,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(damLine);

    QHBoxLayout* handLine = new QHBoxLayout;

    label = new QLabel(tr("Handy: "));
    handy = new QCheckBox();
    handLine->addWidget(label);
    handLine->addWidget(handy);

    label = new QLabel(tr("Clumsy: "));
    clumsy = new QCheckBox();
    handLine->addWidget(label);
    handLine->addWidget(clumsy);

    label = new QLabel(tr("Entangle: "));
    entangle = new QCheckBox();
    handLine->addWidget(label);
    handLine->addWidget(entangle);

    label = new QLabel(tr("Range: "));
    range = new QSpinBox();
    range->setDisabled(true);
    range->setValue(0);
    range->setRange(0,19);
    handLine->addWidget(label);
    handLine->addWidget(range);

    label = new QLabel(tr("Disruptor: "));
    disruptor = new QCheckBox();
    handLine->addWidget(label);
    handLine->addWidget(disruptor);

    connect(handy,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(clumsy,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(entangle,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(range,SIGNAL(valueChanged(int)),SLOT(recalcValues()));
    connect(disruptor,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(handLine);

    QHBoxLayout* brutalLine = new QHBoxLayout;

    label = new QLabel(tr("Quick: "));
    quick = new QCheckBox();
    brutalLine->addWidget(label);
    brutalLine->addWidget(quick);

    label = new QLabel(tr("Armor-Piercing: "));
    ap = new QCheckBox();
    brutalLine->addWidget(label);
    brutalLine->addWidget(ap);

    label = new QLabel(tr("Shock(only): "));
    shock = new QCheckBox();
    brutalLine->addWidget(label);
    brutalLine->addWidget(shock);

    label = new QLabel(tr("Shock And Normal: "));
    shockAdd = new QCheckBox();
    shockAdd->setDisabled(true);
    brutalLine->addWidget(label);
    brutalLine->addWidget(shockAdd);

    connect(quick,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(ap,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(shock,SIGNAL(stateChanged(int)),SLOT(recalcValues()));
    connect(shockAdd,SIGNAL(stateChanged(int)),SLOT(recalcValues()));

    lineLayout->addLayout(brutalLine);

    this->setLayout(lineLayout);
}

MeleeSpecEntry::~MeleeSpecEntry()
{

}

void MeleeSpecEntry::recalcValues()
{
    MeleeComp* comp = MeleeComp::instance();

    float wCost = comp->calcDamCost(damage->value() + range->value());

    wCost *= comp->getAccMul(acc->value());

    // fix Returning stuff if needed
    if (thrown->isChecked())
    {
        returns->setDisabled(false);
    }
    else
    {
        returns->setChecked(false);
        returns->setDisabled(true);
    }

    // fix Range
    if (entangle->isChecked())
    {
        range->setEnabled(true);
    }
    else
    {
        range->setDisabled(true);
        range->setValue(0);
    }

    if (range->value())
    {
        damage->setMaximum(20 - range->value());
    }

    // fix Shock if needed
    if (!shock->isChecked())
    {
        shockAdd->setDisabled(true);
        shockAdd->setChecked(false);
    }
    else
    {
        shockAdd->setDisabled(false);
    }

    // all checkboxes
    wCost *= comp->getHandyMul(handy->isChecked());
    wCost *= comp->getClumsyMul(clumsy->isChecked());
    wCost *= comp->getThrownMul(thrown->isChecked(),returns->isChecked());
    wCost *= comp->getQuickMul(quick->isChecked());
    wCost *= comp->getEntangleMul(entangle->isChecked());
    wCost *= comp->getDisruptorMul(disruptor->isChecked());
    wCost *= comp->getAPMul(ap->isChecked());
    wCost *= comp->getShockMul(shock->isChecked(),shockAdd->isChecked());

    // correct wCost to nearest tenth
    wCost = mekround(wCost);

    double spEff = (eff->value() > wCost) ? wCost : eff->value();

    eff->setValue(spEff);

    cost->setText(QString::number(wCost + (spEff/2.0)));

    space->setText(QString::number(wCost - spEff));
}

Weapon* MeleeSpecEntry::createWeaponPtr()
{
    if (weaponName->text().isEmpty())
    {
        return NULL;
    }

    Weapon* wp = new Weapon();

    wp->setType("Melee");
    wp->setName(weaponName->text());
    wp->setAcc(acc->value());
    wp->setRange(range->value());
    wp->setDamage(damage->value());
    wp->setShots("-");
    wp->setKills(damage->value());
    wp->setCost(cost->text().toDouble() - eff->value()*0.5);
    wp->setSpace(space->text().toDouble() + eff->value());
    wp->setEff(eff->value());

    if (thrown->isChecked())
        wp->addNote("Thrown");
    if (returns->isChecked())
        wp->addNote("Returning");
    if (handy->isChecked()) {
        wp->addNote("Handy");
        wp->setHandy(true);
    }
    if (quick->isChecked())
        wp->addNote("Quick");
    if (clumsy->isChecked()) {
        wp->addNote("Clumsy");
        wp->setClumsy(true);
    }
    if (entangle->isChecked())
        wp->addNote("Entangle");
    if (ap->isChecked())
        wp->addNote("Armor-Piercing");
    if (disruptor->isChecked())
        wp->addNote("Disruptor");

    if (shock->isChecked()) {
        if (shockAdd->isChecked()) {
            wp->addNote("Shock Added");
        }
        else wp->addNote("Shock Only");
    }

    wp->setMelee(true);

    return wp;
}
