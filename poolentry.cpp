#include "poolentry.h"
#include "poolmodel.h"
#include "poolweaponmodel.h"
#include "poolshieldmodel.h"
#include "weapon.h"
#include "shield.h"
#include "mecha.h"
#include "energypool.h"
#include "loclistdelegate.h"
#include "dspinboxdelegate.h"
#include "basespecentry.h"
#include "beamspecentry.h"
#include "meleespecentry.h"
#include "projspecentry.h"
#include "emwspecentry.h"

#include <QTableView>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QScrollArea>
#include <QRadioButton>
#include <QMessageBox>

PoolEntry::PoolEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(buildPoolBox());
    layout->addWidget(buildWeaponView());
    layout->addWidget(buildShieldView());

    baseWidget->setLayout(layout);
    scrollArea->setWidget(baseWidget);
    mainLayout->addWidget(scrollArea);
    this->setLayout(mainLayout);
}

QGroupBox* PoolEntry::buildPoolBox()
{
    QGroupBox* box = new QGroupBox(tr("Energy Pools"));

    QVBoxLayout* layout = new QVBoxLayout();

    poolTable = new QTableView();
    poolModel = new PoolModel(mecha);
    poolTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    poolTable->setSelectionMode(QAbstractItemView::SingleSelection);

    poolTable->setModel(poolModel);

    llDel = new LocListDelegate(mecha,true,true,true,true);
    DSpinBoxDelegate* effDel = new DSpinBoxDelegate(0,400);

    poolTable->setItemDelegateForColumn(1,llDel);
    poolTable->setItemDelegateForColumn(9,effDel);

    poolTable->setColumnWidth(0,150);
    poolTable->setColumnWidth(1,70);
    poolTable->setColumnWidth(2,70);
    poolTable->setColumnWidth(3,80);
    poolTable->setColumnWidth(4,70);
    poolTable->setColumnWidth(5,50);
    poolTable->setColumnWidth(6,70);
    poolTable->setColumnWidth(7,50);
    poolTable->setColumnWidth(8,50);
    poolTable->setColumnWidth(9,50);

    layout->addWidget(poolTable);

    connect(poolTable,SIGNAL(clicked(QModelIndex)),SLOT(poolSelected(QModelIndex)));
    connect(poolModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    QHBoxLayout* row = new QHBoxLayout();

    QPushButton* addPoolButton = new QPushButton(tr("Add Pool"));
    QPushButton* remPoolButton = new QPushButton(tr("Remove Pool"));

    connect(addPoolButton,SIGNAL(clicked()),SLOT(addPool()));
    connect(remPoolButton,SIGNAL(clicked()),SLOT(remPool()));

    row->addWidget(addPoolButton);
    row->addWidget(remPoolButton);

    layout->addLayout(row);

    row = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Name"));
    poolName = new QLineEdit("");
    row->addWidget(label);
    row->addWidget(poolName);

    label = new QLabel(tr("Power(Avail./Max)"));
    poolPower = new QComboBox();
    poolPower->addItem("Battery/50");
    poolPower->addItem("5/25");
    poolPower->addItem("10/40");
    poolPower->addItem("15/45");
    poolPower->addItem("20/50");
    poolPower->addItem("25/55");
    poolPower->addItem("30/60");
    row->addWidget(label);
    row->addWidget(poolPower);
    connect(poolPower,SIGNAL(currentIndexChanged(int)),SLOT(updatePoolStats()));

    label = new QLabel(tr("Portfolio"));
    poolSize = new QComboBox();
    poolSize->addItem("1");
    poolSize->addItem("2");
    poolSize->addItem("3");
    poolSize->addItem("4");
    poolSize->addItem("5");
    poolSize->addItem("7");
    poolSize->addItem("10");
    poolSize->addItem(QString(QChar(0x221e)));
    poolSize->setCurrentIndex(2);
    row->addWidget(label);
    row->addWidget(poolSize);
    connect(poolSize,SIGNAL(currentIndexChanged(int)),SLOT(updatePoolStats()));

    morphable = new QCheckBox(tr("Morphable"));
    row->addWidget(morphable);
    connect(morphable,SIGNAL(stateChanged(int)),SLOT(updatePoolStats()));

    label = new QLabel(tr("Cost: "));
    poolCost = new QLabel("10");
    row->addWidget(label);
    row->addWidget(poolCost);

    label = new QLabel(tr("Space: "));
    poolSpace = new QLabel("10");
    row->addWidget(label);
    row->addWidget(poolSpace);

    label = new QLabel(tr("Eff: "));
    poolEff = new QDoubleSpinBox();
    row->addWidget(label);
    row->addWidget(poolEff);
    connect(poolEff,SIGNAL(valueChanged(double)),SLOT(updatePoolStats()));

    layout->addLayout(row);

    box->setLayout(layout);
    return box;
}

void PoolEntry::someDataChanged()
{
    emit updatedPools();
}

void PoolEntry::updatePoolStats()
{
    EnergyPool* pool = new EnergyPool();

    pool->setName(poolName->text());
    pool->setRating(poolPower->currentIndex());
    pool->setPortfolio(poolSize->currentText().toInt());
    pool->setMorphable(morphable->isChecked());
    pool->setEff(poolEff->value());

    poolCost->setText(QString::number(pool->getCost()));
    poolSpace->setText(QString::number(pool->getSpace()));

    disconnect(poolEff,SIGNAL(valueChanged(double)),this,SLOT(updatePoolStats()));

    poolEff->setValue(pool->getEff());

    connect(poolEff,SIGNAL(valueChanged(double)),SLOT(updatePoolStats()));
}

QGroupBox* PoolEntry::buildWeaponView()
{
    QGroupBox* box = new QGroupBox(tr("Weapons in Pool"));
    QVBoxLayout* layout = new QVBoxLayout();

    weaponTable = new QTableView();
    weaponModel = 0;

    weaponTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    weaponTable->setSelectionMode(QAbstractItemView::SingleSelection);

    weaponTable->setColumnWidth(0,150);
    weaponTable->setColumnWidth(1,40);
    weaponTable->setColumnWidth(2,70);
    weaponTable->setColumnWidth(3,70);
    weaponTable->setColumnWidth(4,50);
    weaponTable->setColumnWidth(5,50);
    weaponTable->setColumnWidth(6,200);

    layout->addWidget(weaponTable);

    QPushButton* addWeaponButton = new QPushButton(tr("Add Weapon"));
    QPushButton* remWeaponButton = new QPushButton(tr("Remove Weapon"));

    QHBoxLayout* row = new QHBoxLayout();

    row->addWidget(addWeaponButton);
    row->addWidget(remWeaponButton);

    connect(addWeaponButton,SIGNAL(clicked()),SLOT(addWeapon()));
    connect(remWeaponButton,SIGNAL(clicked()),SLOT(remWeapon()));

    layout->addLayout(row);

    selectType = new QComboBox(this);

    selectType->addItem("Beam Weapon");
    selectType->addItem("Energy Melee Weapon");
    selectType->setCurrentIndex(0);
    layout->addWidget(selectType);

    connect(selectType,SIGNAL(currentIndexChanged(int)),SLOT(changeEntryType(int)));

    specBox = new QGroupBox(selectType->currentText());
    QVBoxLayout* needLayout = new QVBoxLayout();
    wSpec = new BeamSpecEntry(this);
    needLayout->addWidget(wSpec);
    specBox->setLayout(needLayout);
    layout->addWidget(specBox);

    box->setLayout(layout);
    return box;
}

void PoolEntry::changeEntryType(int type)
{
    specBox->setTitle(selectType->currentText());
    qDeleteAll(specBox->children());
    switch (type)
    {
    case 0: wSpec = new BeamSpecEntry(this);
        break;
    case 1: wSpec = new EMWSpecEntry(this);
        break;
    case 2: wSpec = new MeleeSpecEntry(this);
        break;
    case 3: wSpec = new ProjSpecEntry(this);
        break;
    default: wSpec = new BeamSpecEntry(this);
        break;
    }

    QVBoxLayout* needLayout = new QVBoxLayout();
    needLayout->addWidget(wSpec);
    specBox->setLayout(needLayout);
    wSpec->show();
    return;
}

QGroupBox* PoolEntry::buildShieldView()
{
    QGroupBox* box = new QGroupBox(tr("Shields in Pool"));
    QVBoxLayout* shieldLayout = new QVBoxLayout();

    shieldTable = new QTableView();
    shieldModel = 0;

    shieldTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    shieldTable->setSelectionMode(QAbstractItemView::SingleSelection);

    shieldLayout->addWidget(shieldTable);

    QHBoxLayout* hRow = new QHBoxLayout();

    QPushButton* addShield = new QPushButton(tr("Add Shield"));
    QPushButton* remShield = new QPushButton(tr("Remove Shield"));

    connect(addShield,SIGNAL(clicked()),SLOT(addShield()));
    connect(remShield,SIGNAL(clicked()),SLOT(remShield()));

    hRow->addWidget(addShield);
    hRow->addWidget(remShield);

    shieldLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Name: "));
    sName = new QLineEdit("");
    hRow->addWidget(label,0);
    hRow->addWidget(sName,1);

    label = new QLabel(tr("Type: "));
    sType = new QComboBox();
    sType->addItem("Reactive");
    hRow->addWidget(label);
    hRow->addWidget(sType);

    connect(sType,SIGNAL(currentIndexChanged(int)),SLOT(updateShieldValues()));

    label = new QLabel(tr("Class: "));
    sClass = new QComboBox();
    sClass->addItems(ServoComp::instance()->getSizes());
    hRow->addWidget(label);
    hRow->addWidget(sClass);

    connect(sClass,SIGNAL(currentIndexChanged(int)),SLOT(updateShieldValues()));

    label = new QLabel(tr("Armor: "));
    sArmor = new QComboBox();
    sArmor->addItems(ServoComp::instance()->getArmorTypes());
    sArmor->setCurrentIndex(1);
    hRow->addWidget(label);
    hRow->addWidget(sArmor);

    connect(sArmor,SIGNAL(currentIndexChanged(int)),SLOT(updateShieldValues()));

    label = new QLabel(tr("SP: "));
    SP = new QLabel("5");
    hRow->addWidget(label);
    hRow->addWidget(SP);

    label = new QLabel(tr("Cost: "));
    cost = new QLabel("5");
    hRow->addWidget(label);
    hRow->addWidget(cost);

    shieldLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    label = new QLabel(tr("Defense Ability: "));
    DA = new QSpinBox();
    DA->setRange(-4,0);
    DA->setValue(-2);
    hRow->addWidget(label);
    hRow->addWidget(DA);

    connect(DA,SIGNAL(valueChanged(int)),SLOT(updateShieldValues()));

    label = new QLabel(tr("Reset Time: "));
    reset = new QComboBox();
    reset->addItem("0");
    reset->addItem("1");
    reset->addItem("2");
    reset->addItem("3");
    reset->addItem("X");
    reset->setCurrentIndex(2);
    reset->setDisabled(true);
    hRow->addWidget(label);
    hRow->addWidget(reset);

    connect(reset,SIGNAL(currentIndexChanged(int)),SLOT(updateShieldValues()));

    label = new QLabel(tr("Turns In Use: "));
    tiu = new QComboBox();
    tiu->addItem(QString(QChar(0x221e)));
    tiu->addItem("10");
    tiu->addItem("7");
    tiu->addItem("5");
    tiu->addItem("4");
    tiu->addItem("3");
    tiu->addItem("2");
    tiu->addItem("1");
    tiu->setDisabled(true);
    hRow->addWidget(label);
    hRow->addWidget(tiu);

    connect(tiu,SIGNAL(currentIndexChanged(int)),SLOT(updateShieldValues()));

    label = new QLabel(tr("Weakness: "));
    hRow->addWidget(label);
    wkNo = new QRadioButton(tr("None"),this);
    wkNo->setChecked(true);
    wkEn = new QRadioButton(tr("Energy Only"),this);
    wkMat = new QRadioButton(tr("Matter Only"),this);
    wkRng = new QRadioButton(tr("Ranged Only"),this);
    wkNo->setDisabled(true);
    wkEn->setDisabled(true);
    wkMat->setDisabled(true);
    wkRng->setDisabled(true);
    hRow->addWidget(wkNo);
    hRow->addWidget(wkEn);
    hRow->addWidget(wkMat);
    hRow->addWidget(wkRng);

    connect(wkNo,SIGNAL(clicked()),SLOT(updateShieldValues()));
    connect(wkEn,SIGNAL(clicked()),SLOT(updateShieldValues()));
    connect(wkMat,SIGNAL(clicked()),SLOT(updateShieldValues()));
    connect(wkRng,SIGNAL(clicked()),SLOT(updateShieldValues()));

    shieldLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    ablative = new QCheckBox(tr("Ablative"));
    enclosing = new QCheckBox(tr("Enclosing"));
    surge = new QCheckBox(tr("Surge"));
    ablative->setDisabled(true);
    enclosing->setDisabled(true);
    surge->setDisabled(true);
    hRow->addWidget(ablative);
    hRow->addWidget(enclosing);
    hRow->addWidget(surge);

    connect(ablative,SIGNAL(stateChanged(int)),SLOT(updateShieldValues()));
    connect(enclosing,SIGNAL(stateChanged(int)),SLOT(updateShieldValues()));
    connect(surge,SIGNAL(stateChanged(int)),SLOT(updateShieldValues()));

    updateShieldValues();

    shieldLayout->addLayout(hRow);

    box->setLayout(shieldLayout);
    return box;
}

void PoolEntry::updateShieldValues()
{
    Shield sh;

    QString shieldType = sType->currentText();

    if (shieldType == "Reactive")
        sh.setType(REACTIVE);
    else if (shieldType == "Standard")
        sh.setType(STANDARD);
    else if (shieldType == "Active")
        sh.setType(ACTIVE);

    sh.setBinderMul(0);
    sh.setSize((ServoSizes)sClass->currentIndex());

    sArmor->setDisabled(sh.getType() == REACTIVE);
    DA->setDisabled(sh.getType());

    reset->setEnabled(sh.getType() == REACTIVE);
    tiu->setEnabled(sh.getType() == REACTIVE);
    wkNo->setEnabled(sh.getType() == REACTIVE);
    wkEn->setEnabled(sh.getType() == REACTIVE);
    wkMat->setEnabled(sh.getType() == REACTIVE);
    wkRng->setEnabled(sh.getType() == REACTIVE);
    ablative->setEnabled(sh.getType() == REACTIVE);
    enclosing->setEnabled(sh.getType() == REACTIVE);
    surge->setEnabled(sh.getType() == REACTIVE);

    // calculate Option Multipliers
    double optMul = 1.0;

    if (!sh.getType())
    {
        if (DA->value() > -2)
        {
            optMul *= 1 + (DA->value() + 2)*0.25;
        }
        else if (DA->value() < -2)
        {
            optMul *= 1 + (DA->value() + 2)/5.0;
        }
    }

    if (sh.getType() == REACTIVE)
    {
        if (!ablative->isChecked())
        {
            switch (reset->currentIndex())
            {
            case 0:
                optMul *= 2;
                break;
            case 1:
                optMul *= 1.5;
                break;
            case 3:
                optMul *= 0.75;
                break;
            case 4:
                optMul *= 0.5;
                break;
            }
        }

        optMul *= 1.0 - tiu->currentIndex()*0.1;

        if (wkEn->isChecked() || wkMat->isChecked() || wkRng->isChecked())
            optMul *= 0.75;

        if (enclosing->isChecked())
        {
            optMul *= 0.5;
        }

        if (surge->isChecked())
        {
            optMul *= 2.5;
        }
    }
    else
    {
        sh.setArmor((ArmorTypes)sArmor->currentIndex());
    }

    sh.setOptMul(optMul);

    cost->setText(QString::number(sh.getTotalCost()));
    SP->setText(QString::number(sh.getSP()));
}

void PoolEntry::poolSelected(QModelIndex index)
{
    int row = index.row();

    if (weaponModel)
        delete weaponModel;

    weaponModel = new PoolWeaponModel(mecha,mecha->getPoolRef()[row]->getWeaponsRef());
    weaponTable->setModel(weaponModel);

    weaponTable->setColumnWidth(0,150);
    weaponTable->setColumnWidth(1,40);
    weaponTable->setColumnWidth(2,70);
    weaponTable->setColumnWidth(3,70);
    weaponTable->setColumnWidth(4,50);
    weaponTable->setColumnWidth(5,50);
    weaponTable->setColumnWidth(6,250);

    disconnect(selectType,SIGNAL(currentIndexChanged(int)),this,SLOT(changeEntryType(int)));

    selectType->clear();
    selectType->addItem("Beam Weapon");
    selectType->addItem("Energy Melee Weapon");

    if (mecha->getPoolRef()[row]->isMorphable())
    {
        selectType->addItem("Melee Weapon");
        selectType->addItem("Projectile Weapon");
    }

    connect(selectType,SIGNAL(currentIndexChanged(int)),SLOT(changeEntryType(int)));

    // We WANT it to change here
    selectType->setCurrentIndex(0);

    // Shields next
    if (shieldModel)
        delete shieldModel;

    shieldModel = new PoolShieldModel(mecha->getPoolRef()[row]->getShieldsRef());
    shieldTable->setModel(shieldModel);

    shieldTable->setColumnWidth(0,150);
    shieldTable->setColumnWidth(1,60);
    shieldTable->setColumnWidth(2,80);
    shieldTable->setColumnWidth(3,35);
    shieldTable->setColumnWidth(4,80);
    shieldTable->setColumnWidth(5,35);
    shieldTable->setColumnWidth(6,50);
    shieldTable->setColumnWidth(7,200);

    disconnect(sType,SIGNAL(currentIndexChanged(int)),this,SLOT(updateShieldValues()));

    sType->clear();

    if (mecha->getPoolRef()[row]->isMorphable())
    {
        sType->addItems(Shield::listTypes());
    }
    else sType->addItem("Reactive");

    connect(selectType,SIGNAL(currentIndexChanged(int)),SLOT(changeEntryType(int)));

    // We WANT it to change here
    sType->setCurrentIndex(0);
}

void PoolEntry::addPool()
{
    if (poolName->text().isEmpty())
        return;

    EnergyPool* pool = new EnergyPool();

    pool->setName(poolName->text());
    pool->setRating(poolPower->currentIndex());
    pool->setPortfolio(poolSize->currentText().toInt());
    pool->setMorphable(morphable->isChecked());
    pool->setEff(poolEff->value());

    mecha->addPool(pool);

    poolModel->insertRows(poolModel->rowCount(),1,QModelIndex());
    emit updatedPools();
}

void PoolEntry::remPool()
{
    if (!poolTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(poolTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    poolModel->removeRows(index.row(),1);
    mecha->removePool(sel);
    emit updatedPools();
}

void PoolEntry::addWeapon()
{
    if (!poolTable->selectionModel()->hasSelection())
        return;

    Weapon* newWeapon = wSpec->createWeaponPtr();

    if (newWeapon == NULL)
    {
        return;
    }

    QItemSelection selection(poolTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    if (newWeapon->getCost() > mecha->getPoolRef()[sel]->getMaxPower())
        return;

    if (mecha->getPoolRef()[sel]->getPortfolio() <= mecha->getPoolRef()[sel]->getNumApps())
        return;

    mecha->getPoolRef()[sel]->addWeapon(newWeapon);

    weaponModel->insertRows(weaponModel->rowCount(),1);
}

void PoolEntry::remWeapon()
{
    if (!poolTable->selectionModel()->hasSelection() || !weaponTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(poolTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    selection = weaponTable->selectionModel()->selection();

    index = selection.indexes()[0];

    int row = index.row();

    weaponModel->removeRows(index.row(),1);
    mecha->getPoolRef()[sel]->remWeapon(row);
}

void PoolEntry::addShield()
{
    if (!poolTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(poolTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    if (sName->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please enter a Name for your Shield.");
        msgBox.exec();
        return;
    }

    Shield* sh = new Shield();

    sh->setName(sName->text());

    QString shieldType = sType->currentText();

    if (shieldType == "Reactive")
        sh->setType(REACTIVE);
    else if (shieldType == "Standard")
        sh->setType(STANDARD);
    else if (shieldType == "Active")
        sh->setType(ACTIVE);

    sh->setBinderMul(0);
    sh->setSize((ServoSizes)sClass->currentIndex());

    // calculate Option Multipliers
    double optMul = 1.0;

    if (!sh->getType())
    {
        if (DA->value() > -2)
        {
            optMul *= 1 + (DA->value() + 2)*0.25;
        }
        else if (DA->value() < -2)
        {
            optMul *= 1 + (DA->value() + 2)/5.0;
        }
        sh->addNote("DA=" + QString::number(DA->value()));
    }

    if (sh->getType() == REACTIVE)
    {
        if (!ablative->isChecked())
        {
            switch (reset->currentIndex())
            {
            case 0:
                optMul *= 2;
                break;
            case 1:
                optMul *= 1.5;
                break;
            case 3:
                optMul *= 0.75;
                break;
            case 4:
                optMul *= 0.5;
                break;
            }
            sh->addNote("Reset Time: " + reset->currentText());
        }

        optMul *= 1.0 - tiu->currentIndex()*0.1;
        sh->addNote("TIU=" + tiu->currentText());

        if (wkEn->isChecked() || wkMat->isChecked() || wkRng->isChecked())
            optMul *= 0.75;

        if (wkEn->isChecked())
            sh->addNote("Energy Only");

        if (wkMat->isChecked())
            sh->addNote("Matter Only");

        if (wkRng->isChecked())
            sh->addNote("Ranged Only");

        if (ablative->isChecked())
            sh->addNote("Ablative");

        if (enclosing->isChecked())
        {
            optMul *= 0.5;
            sh->addNote("Enclosing");
        }

        if (surge->isChecked())
        {
            optMul *= 2.5;
            sh->addNote("Offensive");
        }
    }
    else
    {
        sh->setArmor((ArmorTypes)sArmor->currentIndex());
    }

    sh->setOptMul(optMul);

    if (sh->getCost() > mecha->getPoolRef()[sel]->getMaxPower())
        return;

    if (mecha->getPoolRef()[sel]->getPortfolio() <= mecha->getPoolRef()[sel]->getNumApps())
        return;

    mecha->getPoolRef()[sel]->addShield(sh);

    shieldModel->insertRows(shieldModel->rowCount(),1,QModelIndex());
}

void PoolEntry::remShield()
{
    if (!poolTable->selectionModel()->hasSelection() || !shieldTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(poolTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    selection = shieldTable->selectionModel()->selection();

    index = selection.indexes()[0];

    int row = index.row();

    // we only allow one selection

    shieldModel->removeRows(index.row(),1);
    mecha->getPoolRef()[sel]->remShield(row);
}

void PoolEntry::refresh(Mecha *newMech)
{
    mecha = newMech;

    if (weaponModel)
        delete weaponModel;
    if (shieldModel)
        delete shieldModel;

    delete poolModel;

    poolModel = new PoolModel(mecha);
    poolTable->setModel(poolModel);

    connect(poolModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
}
