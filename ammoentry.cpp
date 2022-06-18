#include "ammoentry.h"
#include "ammomodel.h"
#include "ammo.h"
#include "mecha.h"
#include "mektonfuncs.h"
#include "loclistdelegate.h"
#include "dspinboxdelegate.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QScrollArea>
#include <cmath>

AmmoEntry::AmmoEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QVBoxLayout* entryLayout = new QVBoxLayout();
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    ammoTable = new QTableView();

    weaponChoice = new QComboBox();
    weaponChoice->setMinimumWidth(350);
    refillWeaponBox();

    QList<Ammo*>& ammoList = mecha->getAmmoRef();
    model = new AmmoModel(ammoList,wpHash);

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    ammoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ammoTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ammoTable->setModel(model);

    // Set column widths
    ammoTable->setColumnWidth(0,200);
    ammoTable->setColumnWidth(1,35);
    ammoTable->setColumnWidth(2,55);
    ammoTable->setColumnWidth(3,50);
    ammoTable->setColumnWidth(4,40);
    ammoTable->setColumnWidth(5,40);
    ammoTable->setColumnWidth(6,50);
    ammoTable->setColumnWidth(7,50);
    ammoTable->setColumnWidth(8,40);
    ammoTable->setColumnWidth(9,40);
    ammoTable->setColumnWidth(10,350);

    locDelegate = new LocListDelegate(mecha,true,false,true,false);
    DSpinBoxDelegate* effDelegate = new DSpinBoxDelegate(0,500);

    ammoTable->setItemDelegateForColumn(6,locDelegate);
    ammoTable->setItemDelegateForColumn(9,effDelegate);

    layout->addWidget(ammoTable);

    QHBoxLayout* buttonEntry = new QHBoxLayout();

    QPushButton* addAmmo = new QPushButton(tr("Add Ammo"));

    QPushButton* delAmmo = new QPushButton(tr("Remove Ammo"));

    connect(addAmmo,SIGNAL(clicked()),SLOT(addAmmo()));
    connect(delAmmo,SIGNAL(clicked()),SLOT(remAmmo()));

    buttonEntry->addWidget(addAmmo);
    buttonEntry->addWidget(delAmmo);

    layout->addLayout(buttonEntry);

    QLabel* label = new QLabel(tr("Weapon: "));

    QHBoxLayout* mainEntry = new QHBoxLayout();

    connect(weaponChoice,SIGNAL(currentIndexChanged(int)),SLOT(selectType()));

    mainEntry->addWidget(label);
    mainEntry->addWidget(weaponChoice);

    label = new QLabel(tr("Shots/clip: "));
    numShots = new QSpinBox();
    numShots->setRange(0,10000);
    numShots->setDisabled(true);
    mainEntry->addWidget(label);
    mainEntry->addWidget(numShots);

    label = new QLabel(tr("Clips: "));
    numClips = new QSpinBox();
    numClips->setRange(0,10000);
    numClips->setDisabled(true);
    mainEntry->addWidget(label);
    mainEntry->addWidget(numClips);

    label = new QLabel(tr("Cost/clip: "));
    clipCost = new QLabel("0");
    mainEntry->addWidget(label);
    mainEntry->addWidget(clipCost);

    label = new QLabel(tr("Space/clip: "));
    clipSpace = new QLabel("0");
    mainEntry->addWidget(label);
    mainEntry->addWidget(clipSpace);

    connect(numShots,SIGNAL(valueChanged(int)),SLOT(updateCosts()));

    layout->addLayout(mainEntry);

    layout->addWidget(buildOptionBox());

    enableAmmo(false);

    baseWidget->setLayout(layout);
    scrollArea->setWidget(baseWidget);
    entryLayout->addWidget(scrollArea);
    this->setLayout(entryLayout);
}

AmmoEntry::~AmmoEntry()
{

}

void AmmoEntry::someDataChanged()
{
    emit updatedAmmo();
}

void AmmoEntry::weaponsUpdated()
{
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
    refillWeaponBox();

    delete model;
    model = new AmmoModel(mecha->getAmmoRef(),wpHash);

    ammoTable->setModel(model);

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    Ammo* a;

    QList<Ammo*>& ammoList = mecha->getAmmoRef();

    foreach (a,ammoList)
    {
        a->setSpace(wpHash[a->getWeaponName()].getCost() * 0.01 * a->getShots() * a->getClips());
    }
}

void AmmoEntry::refillWeaponBox()
{
    weaponChoice->clear();
    weaponChoice->addItem("None");

    QList<Weapon*>& weaponList = mecha->getWeaponsRef();

    wpHash.clear();

    Weapon* wp;

    foreach (wp,weaponList)
    {
        if ((wp->isUseAmmo() || wp->isUseClips()) && !wpHash.contains(wp->getName()))
        {
            weaponChoice->addItem(wp->getName());
            wpHash[wp->getName()] = *wp;
        }
    }
}

QGroupBox* AmmoEntry::buildOptionBox()
{
    QGroupBox* box = new QGroupBox(tr("Ammunition Options"));

    QVBoxLayout* mainBoxLayout = new QVBoxLayout();

    QHBoxLayout* baseOptions = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Base Ammo Type: "));
    ammoBase = new QComboBox();
    ammoBase->addItem(tr("High-Ex"));
    ammoBase->addItem(tr("Paintball"));
    ammoBase->addItem(tr("Foam"));
    ammoBase->setCurrentIndex(0);
    baseOptions->addWidget(label);
    baseOptions->addWidget(ammoBase);

    label = new QLabel(tr("Shock: "));
    shock = new QComboBox();
    shock->addItem(tr("None"));
    shock->addItem(tr("Shock Only"));
    shock->addItem(tr("Shock Added"));
    shock->setCurrentIndex(0);
    baseOptions->addWidget(label);
    baseOptions->addWidget(shock);

    label = new QLabel(tr("Blast Radius: "));
    blast = new QSpinBox();
    blast->setRange(0,5);
    baseOptions->addWidget(label);
    baseOptions->addWidget(blast);

    label = new QLabel(tr("Tracer: "));
    tracer = new QCheckBox();
    baseOptions->addWidget(label);
    baseOptions->addWidget(tracer);

    label = new QLabel(tr("Nuclear: "));
    nuke = new QCheckBox();
    baseOptions->addWidget(label);
    baseOptions->addWidget(nuke);

    connect(ammoBase,SIGNAL(currentIndexChanged(int)),SLOT(updateCosts()));
    connect(shock,SIGNAL(currentIndexChanged(int)),SLOT(updateCosts()));
    connect(blast,SIGNAL(valueChanged(int)),SLOT(updateCosts()));
    connect(tracer,SIGNAL(stateChanged(int)),SLOT(updateCosts()));
    connect(nuke,SIGNAL(stateChanged(int)),SLOT(updateCosts()));

    mainBoxLayout->addLayout(baseOptions);

    QHBoxLayout* checkOptions = new QHBoxLayout();

    label = new QLabel(tr("Kinetic: "));
    kinetic = new QCheckBox();
    checkOptions->addWidget(label);
    checkOptions->addWidget(kinetic);

    label = new QLabel(tr("Tangler: "));
    tangler = new QCheckBox();
    checkOptions->addWidget(label);
    checkOptions->addWidget(tangler);

    label = new QLabel(tr("A-P: "));
    ap = new QCheckBox();
    checkOptions->addWidget(label);
    checkOptions->addWidget(ap);

    label = new QLabel(tr("Disruptor: "));
    disruptor = new QCheckBox();
    checkOptions->addWidget(label);
    checkOptions->addWidget(disruptor);

    label = new QLabel(tr("Incendiary: "));
    incendiary = new QCheckBox();
    checkOptions->addWidget(label);
    checkOptions->addWidget(incendiary);

    label = new QLabel(tr("Scatter: "));
    scatter = new QCheckBox();
    checkOptions->addWidget(label);
    checkOptions->addWidget(scatter);

    connect(kinetic,SIGNAL(stateChanged(int)),SLOT(updateCosts()));
    connect(tangler,SIGNAL(stateChanged(int)),SLOT(updateCosts()));
    connect(ap,SIGNAL(stateChanged(int)),SLOT(updateCosts()));
    connect(disruptor,SIGNAL(stateChanged(int)),SLOT(updateCosts()));
    connect(incendiary,SIGNAL(stateChanged(int)),SLOT(updateCosts()));
    connect(scatter,SIGNAL(stateChanged(int)),SLOT(updateCosts()));


    mainBoxLayout->addLayout(checkOptions);

    box->setLayout(mainBoxLayout);
    return box;
}

void AmmoEntry::enableAmmo(bool enable)
{
    numShots->setDisabled(!enable);
    numClips->setDisabled(!enable);
    ammoBase->setDisabled(!enable);
    blast->setDisabled(!enable);
    shock->setEnabled(enable);
    tracer->setDisabled(!enable);
    nuke->setEnabled(blast->value());
    kinetic->setEnabled(enable);
    tangler->setEnabled(enable);
    ap->setEnabled(enable);
    disruptor->setEnabled(enable);
    incendiary->setEnabled(enable);
    scatter->setEnabled(enable);
}

void AmmoEntry::selectType()
{
    if (weaponChoice->currentIndex())
    {
        if (!wpHash.contains(weaponChoice->currentText()))
        {
            return;
        }
        if (wpHash[weaponChoice->currentText()].isUseAmmo())
        {
            enableAmmo(true);
            numShots->setValue(1);
            numClips->setValue(1);
            ammoBase->setCurrentIndex(0);
            return;
        }

        if (wpHash[weaponChoice->currentText()].isUseClips())
        {
            enableAmmo(false);
            numShots->setValue(0);
            numClips->setValue(1);
            clipCost->setText("1");
            numClips->setEnabled(true);
            return;
        }
    }

    enableAmmo(false);
}

void AmmoEntry::updateCosts()
{
    if (weaponChoice->currentIndex() == 0)
    {
        clipCost->setText("0");
        clipSpace->setText("0");
        enableAmmo(false);
        return;
    }

    if (wpHash[weaponChoice->currentText()].isUseClips())
    {
        enableAmmo(false);
        clipCost->setText("1");
        numClips->setEnabled(true);
        return;
    }

    enableAmmo(true);

    double ammoCost = wpHash[weaponChoice->currentText()].getCost() * 0.01 * numShots->value();
    double ammoSpace = ammoCost;

    if (ammoBase->currentIndex())
        ammoCost *= 0.5;
    if (tracer->isChecked())
        ammoCost *= 3;
    if (tangler->isChecked())
        ammoCost *= 3;
    if (ap->isChecked())
        ammoCost *= 4;
    if (disruptor->isChecked())
        ammoCost *= 4;
    if (incendiary->isChecked())
        ammoCost *= 4;
    if (scatter->isChecked())
        ammoCost *= 5;
    if (blast->value())
    {
        ammoCost *= (blast->value() * 2 + 4);
        nuke->setEnabled(true);
    }
    else
    {
        nuke->setDisabled(true);
        nuke->setChecked(false);
    }
    if (nuke->isChecked())
        ammoCost *= 1000;
    if (shock->currentIndex())
        ammoCost *= (shock->currentIndex() *2 +2);

    clipCost->setText(QString::number(ammoCost,'f',2));
    clipSpace->setText(QString::number(ammoSpace,'f',2));
}

void AmmoEntry::addAmmo()
{
    if (!weaponChoice->currentIndex() || !numClips->value())
    {
        QMessageBox msg;
        msg.setText("You have not selected a Weapon Type, or your clips are set at 0");
        msg.exec();
        return;
    }

    Ammo* ammo = new Ammo();

    Weapon wp = wpHash[weaponChoice->currentText()];

    if (!numShots->value() && !wp.isUseClips())
    {
        QMessageBox msg;
        msg.setText("Shots in clip are set to 0 and this is not a Beam Weapon.");
        msg.exec();
        return;
    }

    ammo->setWeaponName(weaponChoice->currentText());
    ammo->setAcc(wp.getAcc() + tracer->isChecked());
    ammo->setClips(numClips->value());
    ammo->setCost(mekround(clipCost->text().toDouble() * numClips->value()));

    if (wp.isUseClips())
        ammo->setIsClip(true);

    int damage = wp.getDamage();
    if (nuke->isChecked())
        damage = (int)(damage/10);

    if (!damage)
        damage = 1;

    ammo->setDamage(damage);

    ammo->setKills((int)round(wp.getCost() * 0.01 * numShots->value()));
    if (!ammo->getKills())
        ammo->setKills(1);

    ammo->setKills(ammo->getKills() * ammo->getClips());
    ammo->setLoc("None");
    ammo->setShots(numShots->value());

    if (wp.isUseClips())
    {
        ammo->setIsClip(true);
        if (wp.getShots() == QString(QChar(0x221e)))
            ammo->setShots(15);
        else
            ammo->setShots(wp.getShots().toInt());
    }

    if (wp.getLoc() == "1H" || wp.getLoc() == "2H" || ammo->isClip())
        ammo->setSpace(0);
    else
        ammo->setSpace(mekround(wp.getCost() * 0.01 * ammo->getShots() * ammo->getClips()));

    // Add any notes.
    if (blast->value())
        ammo->addNote("Blast: " + QString::number(blast->value()));
    if (shock->currentIndex())
        ammo->addNote(shock->currentText());
    if (ammoBase->currentIndex())
        ammo->addNote(ammoBase->currentText());
    if (tracer->isChecked())
        ammo->addNote("Tracer");
    if (kinetic->isChecked())
        ammo->addNote("Kinetic");
    if (tangler->isChecked())
        ammo->addNote("Tangler");
    if (ap->isChecked())
        ammo->addNote("A-P");
    if (disruptor->isChecked())
        ammo->addNote("Disruptor");
    if (incendiary->isChecked())
        ammo->addNote("Incendiary");
    if (scatter->isChecked())
        ammo->addNote("Scattershot");
    if (nuke->isChecked())
        ammo->addNote("Nuclear");

    mecha->addAmmo(ammo);

    model->insertRows(model->rowCount(),1,QModelIndex());
    emit updatedAmmo();
}

void AmmoEntry::remAmmo()
{
    if (!ammoTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(ammoTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    model->removeRows(index.row(),1);
    mecha->removeAmmo(sel);
    emit updatedAmmo();
}

void AmmoEntry::refresh(Mecha *newMecha)
{
    mecha = newMecha;

    refillWeaponBox();

    delete model;
    model = new AmmoModel(mecha->getAmmoRef(),wpHash);

    locDelegate = new LocListDelegate(mecha,true,false,true,false);

    ammoTable->setItemDelegateForColumn(6,locDelegate);

    ammoTable->setModel(model);
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
}
