#include "shieldentry.h"
#include "mecha.h"
#include "shield.h"
#include "shieldmodel.h"
#include "mektonfuncs.h"
#include "loclistdelegate.h"
#include "dspinboxdelegate.h"
#include "sizedelegate.h"
#include "armordelegate.h"
#include "reflector.h"
#include "reflectormodel.h"

#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QGroupBox>
#include <QScrollArea>

ShieldEntry::ShieldEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();

    // First add Shields
    QGroupBox* shieldBox = new QGroupBox(tr("Shields"));

    QVBoxLayout* shieldLayout = new QVBoxLayout();

    shieldTable = new QTableView();
    model = new ShieldModel(mecha);
    shieldTable->setModel(model);

    shieldTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    shieldTable->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    SizeDelegate* sizeDel = new SizeDelegate();
    locDel = new LocListDelegate(mecha,true,false,false);
    ArmorDelegate* armDel = new ArmorDelegate();
    DSpinBoxDelegate* effDel = new DSpinBoxDelegate(0,100);

    shieldTable->setItemDelegateForColumn(2,sizeDel);
    shieldTable->setItemDelegateForColumn(3,locDel);
    shieldTable->setItemDelegateForColumn(5,armDel);
    shieldTable->setItemDelegateForColumn(10,effDel);

    shieldTable->setColumnWidth(0,100);
    shieldTable->setColumnWidth(1,60);
    shieldTable->setColumnWidth(2,100);
    shieldTable->setColumnWidth(3,60);
    shieldTable->setColumnWidth(4,60);
    shieldTable->setColumnWidth(5,60);
    shieldTable->setColumnWidth(6,50);
    shieldTable->setColumnWidth(7,60);
    shieldTable->setColumnWidth(8,50);
    shieldTable->setColumnWidth(9,50);
    shieldTable->setColumnWidth(10,40);
    shieldTable->setColumnWidth(11,200);

    shieldLayout->addWidget(shieldTable,1);

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
    sType->addItems(Shield::listTypes());
    hRow->addWidget(label);
    hRow->addWidget(sType);

    connect(sType,SIGNAL(currentIndexChanged(int)),SLOT(updateValues()));

    label = new QLabel(tr("Class: "));
    sClass = new QComboBox();
    sClass->addItems(ServoComp::instance()->getSizes());
    hRow->addWidget(label);
    hRow->addWidget(sClass);

    connect(sClass,SIGNAL(currentIndexChanged(int)),SLOT(updateValues()));

    shieldLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    label = new QLabel(tr("Armor: "));
    sArmor = new QComboBox();
    sArmor->addItems(ServoComp::instance()->getArmorTypes());
    sArmor->setCurrentIndex(1);
    hRow->addWidget(label);
    hRow->addWidget(sArmor);

    connect(sArmor,SIGNAL(currentIndexChanged(int)),SLOT(updateValues()));

    label = new QLabel(tr("SP: "));
    SP = new QLabel("5");
    hRow->addWidget(label);
    hRow->addWidget(SP);

    label = new QLabel(tr("Cost: "));
    cost = new QLabel("5");
    hRow->addWidget(label);
    hRow->addWidget(cost);

    label = new QLabel(tr("Space: "));
    space = new QLabel("1");
    hRow->addWidget(label);
    hRow->addWidget(space);

    label = new QLabel(tr("Eff: "));
    eff = new QDoubleSpinBox();
    eff->setRange(0,100);
    eff->setDecimals(1);
    eff->setSingleStep(0.1);
    hRow->addWidget(label);
    hRow->addWidget(eff);

    connect(eff,SIGNAL(valueChanged(double)),SLOT(updateValues()));

    shieldLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    label = new QLabel(tr("Defense Ability: "));
    DA = new QSpinBox();
    DA->setRange(-4,0);
    DA->setValue(-2);
    hRow->addWidget(label);
    hRow->addWidget(DA);

    connect(DA,SIGNAL(valueChanged(int)),SLOT(updateValues()));

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

    connect(reset,SIGNAL(currentIndexChanged(int)),SLOT(updateValues()));

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

    connect(tiu,SIGNAL(currentIndexChanged(int)),SLOT(updateValues()));

    shieldLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

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

    connect(wkNo,SIGNAL(clicked()),SLOT(updateValues()));
    connect(wkEn,SIGNAL(clicked()),SLOT(updateValues()));
    connect(wkMat,SIGNAL(clicked()),SLOT(updateValues()));
    connect(wkRng,SIGNAL(clicked()),SLOT(updateValues()));

    shieldLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    label = new QLabel(tr("Binder: "));
    binder = new QComboBox();
    binder->addItem("0");
    binder->addItem("1/4");
    binder->addItem("1/3");
    binder->addItem("1/2");
    binder->addItem("2/3");
    binder->addItem("3/4");
    hRow->addWidget(label);
    hRow->addWidget(binder);

    connect(binder,SIGNAL(currentIndexChanged(int)),SLOT(updateValues()));

    label = new QLabel(tr("Binder Space: "));
    binderSpace = new QLabel("0");
    hRow->addWidget(label);
    hRow->addWidget(binderSpace);

    ablative = new QCheckBox(tr("Ablative"));
    enclosing = new QCheckBox(tr("Enclosing"));
    surge = new QCheckBox(tr("Surge"));
    ablative->setDisabled(true);
    enclosing->setDisabled(true);
    surge->setDisabled(true);
    hRow->addWidget(ablative);
    hRow->addWidget(enclosing);
    hRow->addWidget(surge);

    connect(ablative,SIGNAL(stateChanged(int)),SLOT(updateValues()));
    connect(enclosing,SIGNAL(stateChanged(int)),SLOT(updateValues()));
    connect(surge,SIGNAL(stateChanged(int)),SLOT(updateValues()));

    shieldLayout->addLayout(hRow);

    shieldBox->setLayout(shieldLayout);

    mainLayout->addWidget(shieldBox);

    // Now for reflectors
    QGroupBox* refBox = new QGroupBox(tr("Reflectors"));

    QVBoxLayout* refLay = new QVBoxLayout();

    refTable = new QTableView();
    rModel = new ReflectorModel(mecha);
    refTable->setModel(rModel);

    refTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    refTable->setSelectionMode(QAbstractItemView::SingleSelection);

    refLocDel = new LocListDelegate(mecha,false,true,true,true);

    refTable->setItemDelegateForColumn(2,refLocDel);

    connect(rModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    refLay->addWidget(refTable);

    hRow = new QHBoxLayout();

    QPushButton* addRef = new QPushButton(tr("Add Reflector"));
    QPushButton* remRef = new QPushButton(tr("Remove Reflector"));

    connect(addRef,SIGNAL(clicked()),SLOT(addReflect()));
    connect(remRef,SIGNAL(clicked()),SLOT(remReflect()));

    hRow->addWidget(addRef);
    hRow->addWidget(remRef);

    refLay->addLayout(hRow,0);

    hRow = new QHBoxLayout();

    label = new QLabel(tr("Name: "));
    refName = new QLineEdit(tr(""));
    hRow->addWidget(label,0);
    hRow->addWidget(refName,1);

    label = new QLabel(tr("Value: "));
    refVal = new QSpinBox();
    refVal->setRange(1,10);
    hRow->addWidget(label,0);
    hRow->addWidget(refVal,0);

    connect(refVal,SIGNAL(valueChanged(int)),SLOT(updateValues()));

    label = new QLabel(tr("Cost: "));
    refCost = new QLabel("1");
    hRow->addWidget(label,0);
    hRow->addWidget(refCost,0);

    label = new QLabel(tr("Space: "));
    refSpace = new QLabel("1");
    hRow->addWidget(label,0);
    hRow->addWidget(refSpace,0);

    label = new QLabel(tr("Eff: "));
    refEff = new QDoubleSpinBox();
    refEff->setRange(0,100);
    refEff->setDecimals(1);
    refEff->setSingleStep(1.0);
    hRow->addWidget(label,0);
    hRow->addWidget(refEff,0);

    connect(refEff,SIGNAL(valueChanged(double)),SLOT(updateValues()));

    refLay->addLayout(hRow,0);

    refBox->setLayout(refLay);

    mainLayout->addWidget(refBox);

    baseWidget->setLayout(mainLayout);
    scrollArea->setWidget(baseWidget);
    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

void ShieldEntry::someDataChanged()
{
    emit shieldsUpdated();
}

void ShieldEntry::refresh(Mecha *mecha)
{
    this->mecha = mecha;
    delete model;
    model = new ShieldModel(mecha);

    delete locDel;

    locDel = new LocListDelegate(mecha,true,false,false);

    shieldTable->setItemDelegateForColumn(3,locDel);

    shieldTable->setModel(model);
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    delete rModel;
    rModel = new ReflectorModel(mecha);

    delete refLocDel;

    refLocDel = new LocListDelegate(mecha,false,true,true,true);

    refTable->setItemDelegateForColumn(2,refLocDel);

    refTable->setModel(rModel);
    connect(rModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
}

void ShieldEntry::updateValues()
{
    Shield sh;

    sh.setType((ShieldTypes)sType->currentIndex());

    if (binder->currentIndex() && sh.getType() != REACTIVE)
    {
        QStringList bList = binder->currentText().split('/');
        if (bList.length() < 2)
            sh.setBinderMul(0);
        else sh.setBinderMul(bList.at(0).toInt() / bList.at(1).toDouble());
    }
    else sh.setBinderMul(0);
    sh.setSize((ServoSizes)sClass->currentIndex());

    binder->setDisabled(sh.getType() == REACTIVE);
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
    sh.setEff(eff->value());

    cost->setText(QString::number(sh.getTotalCost()));
    space->setText(QString::number(sh.getSpace()));
    SP->setText(QString::number(sh.getSP()));
    binderSpace->setText(QString::number(sh.getBinderSpace()));
    eff->setValue(sh.getEff());

    Reflector ref;

    ref.setValue(refVal->value());

    ref.setEff(refEff->value());

    refCost->setText(QString::number(ref.getCost()));
    refSpace->setText(QString::number(ref.getSpace()));
    refEff->setValue(ref.getEff());
}

void ShieldEntry::addShield()
{
    if (sName->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please enter a Name for your Shield.");
        msgBox.exec();
        return;
    }

    if (mecha->getServoNames().contains(sName->text()))
    {
        QMessageBox msgBox;
        msgBox.setText("Shields may not share names with Servos.");
        msgBox.exec();
        return;
    }

    if (mecha->getShieldNames().contains(sName->text()))
    {
        QMessageBox msgBox;
        msgBox.setText("Shields must have a unique name.");
        msgBox.exec();
        return;
    }

    Shield* sh = new Shield();

    sh->setName(sName->text());

    sh->setType((ShieldTypes)sType->currentIndex());

    if (binder->currentIndex() && sh->getType() != REACTIVE)
    {
        QStringList bList = binder->currentText().split('/');
        if (bList.length() < 2)
            sh->setBinderMul(0);
        else sh->setBinderMul(bList.at(0).toInt() / bList.at(1).toDouble());
    }
    else sh->setBinderMul(0);
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
    sh->setEff(eff->value());

    mecha->addShield(sh);

    model->insertRows(model->rowCount(),1,QModelIndex());
    emit shieldsUpdated();
}

void ShieldEntry::addReflect()
{
    if (refName->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please enter a Name for your Reflector.");
        msgBox.exec();
        return;
    }

    Reflector* ref = new Reflector();

    ref->setName(refName->text());
    ref->setValue(refVal->value());
    ref->setEff(refEff->value());

    mecha->addReflector(ref);

    rModel->insertRows(model->rowCount(),1,QModelIndex());
    emit shieldsUpdated();
}

void ShieldEntry::remShield()
{
    if (!shieldTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(shieldTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    model->removeRows(index.row(),1);
    mecha->removeShield(sel);
    emit shieldsUpdated();
}


void ShieldEntry::remReflect()
{
    if (!refTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(refTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    rModel->removeRows(index.row(),1);
    mecha->removeReflector(sel);
    emit shieldsUpdated();
}
