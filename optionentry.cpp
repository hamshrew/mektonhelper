#include "optionentry.h"
#include "mecha.h"
#include "option.h"
#include "optionmodel.h"
#include "optioncomp.h"
#include "dspinboxdelegate.h"
#include "loclistdelegate.h"
#include "spinboxdelegate.h"
#include "servocomp.h"
#include "remotemodel.h"
#include "mechafile.h"
#include "drone.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>

OptionEntry::OptionEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();

    mainLayout->addWidget(buildOptionBox());
    mainLayout->addWidget(buildRemoteBox());

    baseWidget->setLayout(mainLayout);
    scrollArea->setWidget(baseWidget);
    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

QGroupBox* OptionEntry::buildOptionBox()
{
    QGroupBox* box = new QGroupBox(tr("Options"));
    QVBoxLayout* mainLayout = new QVBoxLayout();

    optionTable = new QTableView();
    model = new OptionModel(mecha);

    optionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    optionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    optionTable->setModel(model);

    locDel = new LocListDelegate(mecha,false,false,true,false);
    SpinBoxDelegate* numDel = new SpinBoxDelegate(1,50);
    DSpinBoxDelegate* effDel = new DSpinBoxDelegate(0,300);

    optionTable->setItemDelegateForColumn(2,numDel);
    optionTable->setItemDelegateForColumn(3,locDel);
    optionTable->setItemDelegateForColumn(6,effDel);

    optionTable->setColumnWidth(0,150);
    optionTable->setColumnWidth(1,250);
    optionTable->setColumnWidth(2,60);
    optionTable->setColumnWidth(3,60);
    optionTable->setColumnWidth(4,50);
    optionTable->setColumnWidth(5,50);
    optionTable->setColumnWidth(6,50);

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    mainLayout->addWidget(optionTable);

    QHBoxLayout* hRow = new QHBoxLayout();

    QPushButton* addButton = new QPushButton(tr("Add System"));
    QPushButton* remButton = new QPushButton(tr("Remove System"));

    connect(addButton,SIGNAL(clicked()),SLOT(addOption()));
    connect(remButton,SIGNAL(clicked()),SLOT(remOption()));

    hRow->addWidget(addButton);
    hRow->addWidget(remButton);
    mainLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    QLabel* label = new QLabel(tr("System: "));
    optionChoice = new QComboBox();
    optionChoice->addItems(OptionComp::instance()->getOptions());
    hRow->addWidget(label);
    hRow->addWidget(optionChoice);

    connect(optionChoice,SIGNAL(currentIndexChanged(int)),SLOT(updatePrev()));

    label = new QLabel(tr("Notes: "));
    notes = new QLineEdit("");
    hRow->addWidget(label);
    hRow->addWidget(notes,1);

    label = new QLabel(tr("Number: "));
    number = new QSpinBox();
    number->setRange(1,50);
    hRow->addWidget(label);
    hRow->addWidget(number);

    connect(number,SIGNAL(valueChanged(int)),SLOT(updatePrev()));

    label = new QLabel(tr("Cost: "));
    cost = new QLabel("0.1");
    hRow->addWidget(label);
    hRow->addWidget(cost);

    label = new QLabel(tr("Space: "));
    space = new QLabel("0");
    hRow->addWidget(label);
    hRow->addWidget(space);

    mainLayout->addLayout(hRow);

    box->setLayout(mainLayout);
    return box;
}

QGroupBox* OptionEntry::buildRemoteBox()
{
    QGroupBox* box = new QGroupBox(tr("Remotes"));

    QVBoxLayout* layout = new QVBoxLayout();

    remoteClass = new QComboBox();
    remoteClass->addItem("None");
    remoteClass->addItems(ServoComp::instance()->getSizes());

    controlRange = new QComboBox();
    controlRange->addItem("0");
    controlRange->addItem("1");
    controlRange->addItem("3");
    controlRange->addItem("5");
    controlRange->addItem("7");
    controlRange->addItem("9");
    controlRange->addItem("12");
    controlRange->addItem("15");
    controlRange->addItem("20");

    QHBoxLayout* row = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Class: "));
    row->addWidget(label);
    row->addWidget(remoteClass);
    label = new QLabel(tr("Control Range: "));
    row->addWidget(label);
    row->addWidget(controlRange);

    label = new QLabel(tr("Operation Range:"));
    opRange = new QLabel("1");
    row->addWidget(label);
    row->addWidget(opRange);

    label = new QLabel(tr("Location: "));
    remoteLoc = new QComboBox();
    remoteLoc->addItem("None");
    remoteLoc->addItems(mecha->getServoNames());

    QStringList shList = mecha->getShieldNames();
    QString sh;

    foreach (sh,shList)
    {
        if (mecha->getBinderSpace(sh + " Binder"))
            remoteLoc->addItem(sh + " Binder");
    }

    row->addWidget(label);
    row->addWidget(remoteLoc);

    layout->addLayout(row);

    row = new QHBoxLayout();

    wired = new QCheckBox("Wire-Controlled");
    row->addWidget(wired);

    remStored = new QCheckBox("Store Remotes");
    row->addWidget(remStored);

    label = new QLabel(tr("Cost: "));
    rCost = new QLabel("0");
    row->addWidget(label);
    row->addWidget(rCost);

    label = new QLabel(tr("Space: "));
    rSpace = new QLabel("0");
    row->addWidget(label);
    row->addWidget(rSpace);

    label = new QLabel(tr("Eff: "));
    remEff = new QDoubleSpinBox();
    remEff->setRange(0,44);
    remEff->setDecimals(1);
    remEff->setSingleStep(0.1);
    row->addWidget(label);
    row->addWidget(remEff);

    layout->addLayout(row);

    droneTable = new QTableView();
    droneModel = new RemoteModel(mecha);

    droneTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    droneTable->setSelectionMode(QAbstractItemView::SingleSelection);
    droneTable->setModel(droneModel);

    droneTable->setColumnWidth(0,350);
    droneTable->setColumnWidth(1,50);
    droneTable->setColumnWidth(2,60);

    row = new QHBoxLayout();

    row->addWidget(droneTable);

    layout->addLayout(row);

    connect(droneModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(remoteDataChanged()));

    row = new QHBoxLayout();

    QPushButton* addButton = new QPushButton(tr("Add Drone"));
    QPushButton* remButton = new QPushButton(tr("Remove Drone"));

    connect(addButton,SIGNAL(clicked()),SLOT(addDrone()));
    connect(remButton,SIGNAL(clicked()),SLOT(remDrone()));

    row->addWidget(addButton);
    row->addWidget(remButton);
    layout->addLayout(row);

    connect(remoteClass,SIGNAL(currentIndexChanged(int)),SLOT(remoteDataChanged()));
    connect(controlRange,SIGNAL(currentIndexChanged(int)),SLOT(remoteDataChanged()));
    connect(remoteLoc,SIGNAL(currentIndexChanged(int)),SLOT(remoteDataChanged()));
    connect(wired,SIGNAL(stateChanged(int)),SLOT(remoteDataChanged()));
    connect(remStored,SIGNAL(stateChanged(int)),SLOT(remoteDataChanged()));
    connect(remEff,SIGNAL(valueChanged(double)),SLOT(remoteDataChanged()));

    box->setLayout(layout);
    return box;
}

void OptionEntry::remoteDataChanged()
{
    Remotes* rem = mecha->getRemotes();

    rem->setClass(remoteClass->currentIndex());
    rem->setRange(controlRange->currentText().toUInt());
    rem->setLoc(remoteLoc->currentText());
    rem->setWired(wired->isChecked());
    rem->setStoresRemotes(remStored->isChecked());
    rem->setEff(remEff->value());

    rCost->setText(QString::number(rem->getCost()));
    rSpace->setText(QString::number(rem->getSpace()));
    opRange->setText(QString::number(rem->getOperationRange()));

    disconnect(remEff,SIGNAL(valueChanged(double)),this,SLOT(remoteDataChanged()));
    remEff->setValue(rem->getEff());
    connect(remEff,SIGNAL(valueChanged(double)),SLOT(remoteDataChanged()));

    emit optionsUpdated();
}

void OptionEntry::updateServos()
{
    disconnect(remoteLoc,SIGNAL(currentIndexChanged(int)),this,SLOT(remoteDataChanged()));

    remoteLoc->clear();
    remoteLoc->addItem("None");
    remoteLoc->addItems(mecha->getServoNames());

    QStringList shList = mecha->getShieldNames();
    QString sh;

    foreach (sh,shList)
    {
        if (mecha->getBinderSpace(sh + " Binder"))
            remoteLoc->addItem(sh + " Binder");
    }

    remoteLoc->setCurrentIndex(remoteLoc->findText(mecha->getRemotes()->getLoc()));

    connect(remoteLoc,SIGNAL(currentIndexChanged(int)),SLOT(remoteDataChanged()));
}

void OptionEntry::someDataChanged()
{
    emit optionsUpdated();
}

void OptionEntry::updatePrev()
{
    Option opt;

    opt.setName(optionChoice->currentText());
    opt.setNumber(number->value());

    cost->setText(QString::number(opt.getCost(mecha->getWeight())));

    space->setText(QString::number(opt.getSpace(mecha->getWeight())));
}

void OptionEntry::addOption()
{
    Option* opt = new Option();

    opt->setName(optionChoice->currentText());
    opt->setNotes(notes->text());
    opt->setNumber(number->value());

    mecha->addOption(opt);

    model->insertRows(model->rowCount(),1,QModelIndex());
    emit optionsUpdated();
}

void OptionEntry::remOption()
{
    if (!optionTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(optionTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    model->removeRows(index.row(),1);
    mecha->removeOption(sel);
    emit optionsUpdated();
}

void OptionEntry::addDrone()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Load Drone...","","Mek File (*.mek);;All Files (*.*)");
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not open Mecha file for reading.");
        msgBox.exec();
        return;
    }

    MechaFile mechFile;
    if (!mechFile.parseFile(&file))
    {
        QMessageBox msgBox;
        msgBox.setText("Mecha File chosen is not valid or corrupted.");
        msgBox.exec();
        file.close();
        return;
    }

    file.close();

    Drone* drone = new Drone();

    drone->setName(mechFile.getMecha()->getName());
    drone->setCost(mechFile.getMecha()->getCost());
    drone->setWeight(mechFile.getMecha()->getWeight());

    mecha->getRemotes()->addDrone(drone);
    droneModel->insertRows(model->rowCount(),1,QModelIndex());
    remoteDataChanged();
}

void OptionEntry::remDrone()
{
    if (!droneTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(droneTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    droneModel->removeRows(index.row(),1);
    mecha->getRemotes()->remDrone(sel);
    remoteDataChanged();
}

void OptionEntry::refresh(Mecha *newMech)
{
    mecha = newMech;
    delete model;
    model = new OptionModel(mecha);

    locDel = new LocListDelegate(mecha,false,false,true,false);

    optionTable->setItemDelegateForColumn(3,locDel);

    optionTable->setModel(model);
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    delete droneModel;
    droneModel = new RemoteModel(mecha);

    droneTable->setModel(droneModel);
    connect(droneModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(remoteDataChanged()));

    disconnect(remoteClass,SIGNAL(currentIndexChanged(int)),this,SLOT(remoteDataChanged()));
    disconnect(controlRange,SIGNAL(currentIndexChanged(int)),this,SLOT(remoteDataChanged()));
    disconnect(remoteLoc,SIGNAL(currentIndexChanged(int)),this,SLOT(remoteDataChanged()));
    disconnect(wired,SIGNAL(stateChanged(int)),this,SLOT(remoteDataChanged()));
    disconnect(remStored,SIGNAL(stateChanged(int)),this,SLOT(remoteDataChanged()));
    disconnect(remEff,SIGNAL(valueChanged(double)),this,SLOT(remoteDataChanged()));

    const Remotes* rem = mecha->getConstRemotePtr();

    remoteClass->setCurrentIndex(rem->getClass());
    controlRange->setCurrentIndex(controlRange->findText(QString::number(rem->getControlRange())));
    remoteLoc->clear();
    remoteLoc->addItem("None");
    remoteLoc->addItems(mecha->getServoNames());

    QStringList shList = mecha->getShieldNames();
    QString sh;

    foreach (sh,shList)
    {
        if (mecha->getBinderSpace(sh + " Binder"))
            remoteLoc->addItem(sh + " Binder");
    }

    remoteLoc->setCurrentIndex(remoteLoc->findText(mecha->getRemotes()->getLoc()));

    wired->setChecked(rem->isWired());
    remStored->setChecked(rem->isStoring());
    rCost->setText(QString::number(rem->getCost()));
    rSpace->setText(QString::number(rem->getSpace()));
    opRange->setText(QString::number(rem->getOperationRange()));
    remEff->setValue(rem->getEff());

    connect(remoteClass,SIGNAL(currentIndexChanged(int)),SLOT(remoteDataChanged()));
    connect(controlRange,SIGNAL(currentIndexChanged(int)),SLOT(remoteDataChanged()));
    connect(remoteLoc,SIGNAL(currentIndexChanged(int)),SLOT(remoteDataChanged()));
    connect(wired,SIGNAL(stateChanged(int)),SLOT(remoteDataChanged()));
    connect(remStored,SIGNAL(stateChanged(int)),SLOT(remoteDataChanged()));
    connect(remEff,SIGNAL(valueChanged(double)),SLOT(remoteDataChanged()));
}
