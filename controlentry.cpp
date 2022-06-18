#include "controlentry.h"
#include "mecha.h"
#include "cockpit.h"
#include "sensor.h"
#include "servocomp.h"
#include "recon.h"
#include "reconcomp.h"
#include "cockpitmodel.h"
#include "sensormodel.h"
#include "spinboxdelegate.h"
#include "dspinboxdelegate.h"
#include "loclistdelegate.h"
#include "comboboxdelegate.h"
#include "cockpitcomp.h"
#include "ecm.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QScrollArea>

ControlEntry::ControlEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QVBoxLayout* entryLayout = new QVBoxLayout();

    QVBoxLayout* cpLayout = new QVBoxLayout();

    QGroupBox* cpBox = new QGroupBox("Cockpits");

    cockpitTable = new QTableView(this);

    QList<Cockpit*>& cpList = mecha->getCockpitRef();

    cpModel = new CockpitModel(cpList);

    connect(cpModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    cockpitTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cockpitTable->setSelectionMode(QAbstractItemView::SingleSelection);
    cockpitTable->setModel(cpModel);

    // Set column widths
    cockpitTable->setColumnWidth(0,120);
    cockpitTable->setColumnWidth(1,70);
    cockpitTable->setColumnWidth(2,35);
    cockpitTable->setColumnWidth(3,65);
    cockpitTable->setColumnWidth(4,40);
    cockpitTable->setColumnWidth(5,40);
    cockpitTable->setColumnWidth(7,40);

    locDel = new LocListDelegate(mecha,false,false,true,true);
    SpinBoxDelegate* crewDel = new SpinBoxDelegate(1,500);
    SpinBoxDelegate* passDel = new SpinBoxDelegate(0,10000);

    CockpitComp* cpComp = CockpitComp::instance();

    QStringList encList = cpComp->getEncTypes();

    ComboBoxDelegate* encDel = new ComboBoxDelegate(encList);
    DSpinBoxDelegate* effDel = new DSpinBoxDelegate(0,500,1);

    cockpitTable->setItemDelegateForColumn(1,locDel);
    cockpitTable->setItemDelegateForColumn(2,crewDel);
    cockpitTable->setItemDelegateForColumn(3,passDel);
    cockpitTable->setItemDelegateForColumn(6,encDel);
    cockpitTable->setItemDelegateForColumn(7,effDel);

    cpLayout->addWidget(cockpitTable);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* addButton = new QPushButton(tr("Add Cockpit"));
    QPushButton* remButton = new QPushButton(tr("Remove Cockpit"));

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(remButton);

    cpLayout->addLayout(buttonLayout);

    QHBoxLayout* newCPLayout = new QHBoxLayout();

    QLabel* label = new QLabel("Designation: ");
    cpName = new QLineEdit("");
    newCPLayout->addWidget(label);
    newCPLayout->addWidget(cpName);

    label = new QLabel("Location: ");
    cpLoc = new QComboBox();
    cpLoc->addItem("None");
    cpLoc->addItems(mecha->getServoNames());
    newCPLayout->addWidget(label);
    newCPLayout->addWidget(cpLoc);

    label = new QLabel("Crew: ");
    cpCrew = new QSpinBox();
    cpCrew->setRange(1,500);
    newCPLayout->addWidget(label);
    newCPLayout->addWidget(cpCrew);

    label = new QLabel("Passengers: ");
    cpPass = new QSpinBox();
    cpPass->setRange(0,10000);
    newCPLayout->addWidget(label);
    newCPLayout->addWidget(cpPass);

    label = new QLabel("Enclosure: ");
    cpEnc = new QComboBox();
    cpEnc->addItems(cpComp->getEncTypes());
    newCPLayout->addWidget(label);
    newCPLayout->addWidget(cpEnc);

    cpLayout->addLayout(newCPLayout);

    connect(addButton,SIGNAL(clicked()),SLOT(addCockpit()));
    connect(remButton,SIGNAL(clicked()),SLOT(remCockpit()));

    cpBox->setLayout(cpLayout);

    entryLayout->addWidget(cpBox);

    // Sensor layout
    QVBoxLayout* sLayout = new QVBoxLayout();

    QGroupBox* sBox = new QGroupBox("Sensors");

    sensorTable = new QTableView(this);

    QList<Sensor*>& sList = mecha->getSensorRef();

    sModel = new SensorModel(sList);

    connect(sModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    sensorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    sensorTable->setSelectionMode(QAbstractItemView::SingleSelection);
    sensorTable->setModel(sModel);

    // Set column widths
    sensorTable->setColumnWidth(0,120);
    sensorTable->setColumnWidth(1,120);
    sensorTable->setColumnWidth(2,45);
    sensorTable->setColumnWidth(3,40);
    sensorTable->setColumnWidth(4,40);
    sensorTable->setColumnWidth(5,40);
    sensorTable->setColumnWidth(6,70);
    sensorTable->setColumnWidth(7,80);
    sensorTable->setColumnWidth(8,40);
    sensorTable->setColumnWidth(9,200);

    QStringList classList;

    classList.append(ServoComp::instance()->getSizes());
    classList.push_back("Backup");

    ComboBoxDelegate* classDel = new ComboBoxDelegate(classList);

    sensorTable->setItemDelegateForColumn(1,classDel);
    sensorTable->setItemDelegateForColumn(2,locDel);
    sensorTable->setItemDelegateForColumn(8,effDel);

    sLayout->addWidget(sensorTable);

    buttonLayout = new QHBoxLayout();

    addButton = new QPushButton(tr("Add Sensor"));
    remButton = new QPushButton(tr("Remove Sensor"));

    reconChoice = new QComboBox();
    reconChoice->addItems(ReconComp::instance()->getReconList());

    QPushButton* addRecon = new QPushButton(tr("Add Recon System"));
    QPushButton* remRecon = new QPushButton(tr("Remove Recon System"));

    addRecon->setToolTip("Adds the Recon System in the dropdown menu to the selected Sensor Package.");
    remRecon->setToolTip("Removes the Recon System in the dropdown menu from the selected Sensor Package, if able.");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(remButton);
    buttonLayout->addWidget(reconChoice);
    buttonLayout->addWidget(addRecon);
    buttonLayout->addWidget(remRecon);

    sLayout->addLayout(buttonLayout);

    QHBoxLayout* newSLayout = new QHBoxLayout();

    label = new QLabel("Designation: ");
    sName = new QLineEdit("");
    newSLayout->addWidget(label);
    newSLayout->addWidget(sName);

    label = new QLabel("Class: ");
    sClass = new QComboBox();
    sClass->addItems(ServoComp::instance()->getSizes());
    sClass->addItem("Backup");
    newSLayout->addWidget(label);
    newSLayout->addWidget(sClass);

    label = new QLabel("Location: ");
    sLoc = new QComboBox();
    sLoc->addItem("None");
    sLoc->addItems(mecha->getServoNames());
    newSLayout->addWidget(label);
    newSLayout->addWidget(sLoc);

    sLayout->addLayout(newSLayout);

    connect(addButton,SIGNAL(clicked()),SLOT(addSensor()));
    connect(remButton,SIGNAL(clicked()),SLOT(remSensor()));
    connect(addRecon,SIGNAL(clicked()),SLOT(addReconSys()));
    connect(remRecon,SIGNAL(clicked()),SLOT(remReconSys()));

    sBox->setLayout(sLayout);

    entryLayout->addWidget(sBox);

    entryLayout->addWidget(buildECMBox());

    baseWidget->setLayout(entryLayout);
    scrollArea->setWidget(baseWidget);
    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

QGroupBox* ControlEntry::buildECMBox()
{
    QGroupBox* box = new QGroupBox(tr("ECM/ECCM Systems"));

    QVBoxLayout* mainLay = new QVBoxLayout();

    QHBoxLayout* row = new QHBoxLayout();

    QLabel* label = new QLabel(tr("EW System Location: "));
    ecmLoc = new QComboBox();
    ecmLoc->addItem("None");
    ecmLoc->addItems(mecha->getServoNames());

    QStringList shList = mecha->getShieldNames();
    QString sh;

    foreach (sh,shList)
    {
        if (mecha->getBinderSpace(sh + " Binder"))
            ecmLoc->addItem(sh + " Binder");
    }

    ecmLoc->addItem("Split");

    connect(ecmLoc,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    row->addWidget(label);
    row->addWidget(ecmLoc);

    label = new QLabel(tr("Total ECM Cost: "));
    ecmTotCost = new QLabel("0");
    ecmTotCost->setMinimumWidth(30);
    row->addWidget(label);
    row->addWidget(ecmTotCost);

    label = new QLabel(tr("ECM Space: "));
    ecmSpace = new QLabel("0");
    row->addWidget(label);
    ecmSpace->setMinimumWidth(30);
    row->addWidget(ecmSpace);

    label = new QLabel(tr("Eff: "));
    ecmEff = new QDoubleSpinBox();
    ecmEff->setRange(0,550);
    ecmEff->setDecimals(1);
    ecmEff->setSingleStep(0.1);
    row->addWidget(label);
    row->addWidget(ecmEff);

    connect(ecmEff,SIGNAL(valueChanged(double)),SLOT(updateECM()));

    label = new QLabel(" ");
    row->addWidget(label,1);

    mainLay->addLayout(row);

    // Create a stringlist for the ranges
    QStringList ranges;
    for(int i = 0;i < 5;++i) ranges.push_back(QString::number(i));
    ranges.push_back("6");
    ranges.push_back("8");
    ranges.push_back("10");
    ranges.push_back("20");
    ranges.push_back("60");
    ranges.push_back("100");

    // Add the ECM systems
    row = new QHBoxLayout();
    label = new QLabel(tr("   Sensor Jamming"));
    label->setMinimumWidth(120);
    sensJamVal = new QSpinBox();
    sensJamVal->setRange(0,10);
    row->addWidget(label);
    row->addWidget(sensJamVal);
    label = new QLabel(tr("Radius: "));
    sensJamRad = new QComboBox();
    sensJamRad->addItems(ranges);
    row->addWidget(label);
    row->addWidget(sensJamRad);
    label = new QLabel(tr("Beam: "));
    sensJamBeam = new QComboBox();
    sensJamBeam->addItems(ranges);
    row->addWidget(label);
    row->addWidget(sensJamBeam);
    label = new QLabel(" ");
    row->addWidget(label,1);
    mainLay->addLayout(row);

    connect(sensJamVal,SIGNAL(valueChanged(int)),SLOT(updateECM()));
    connect(sensJamRad,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
    connect(sensJamBeam,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    row = new QHBoxLayout();
    label = new QLabel(tr("   Missile Jamming"));
    label->setMinimumWidth(120);
    missJamVal = new QSpinBox();
    missJamVal->setRange(0,10);
    row->addWidget(label);
    row->addWidget(missJamVal);
    label = new QLabel(tr("Radius: "));
    missJamRad = new QComboBox();
    missJamRad->addItems(ranges);
    row->addWidget(label);
    row->addWidget(missJamRad);
    label = new QLabel(tr("Beam: "));
    missJamBeam = new QComboBox();
    missJamBeam->addItems(ranges);
    row->addWidget(label);
    row->addWidget(missJamBeam);
    label = new QLabel(" ");
    row->addWidget(label,1);
    mainLay->addLayout(row);

    connect(missJamVal,SIGNAL(valueChanged(int)),SLOT(updateECM()));
    connect(missJamRad,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
    connect(missJamBeam,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    row = new QHBoxLayout();
    label = new QLabel(tr("   Radar Jamming"));
    label->setMinimumWidth(120);
    radJamVal = new QSpinBox();
    radJamVal->setRange(0,10);
    row->addWidget(label);
    row->addWidget(radJamVal);
    label = new QLabel(tr("Radius: "));
    radJamRad = new QComboBox();
    radJamRad->addItems(ranges);
    row->addWidget(label);
    row->addWidget(radJamRad);
    label = new QLabel(tr("Beam: "));
    radJamBeam = new QComboBox();
    radJamBeam->addItems(ranges);
    row->addWidget(label);
    row->addWidget(radJamBeam);
    label = new QLabel(" ");
    row->addWidget(label,1);
    mainLay->addLayout(row);

    connect(radJamVal,SIGNAL(valueChanged(int)),SLOT(updateECM()));
    connect(radJamRad,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
    connect(radJamBeam,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    row = new QHBoxLayout();
    label = new QLabel(tr("   ECCM"));
    label->setMinimumWidth(120);
    eccmVal = new QSpinBox();
    eccmVal->setRange(0,10);
    row->addWidget(label);
    row->addWidget(eccmVal);
    label = new QLabel(tr("Radius: "));
    eccmRad = new QComboBox();
    eccmRad->addItems(ranges);
    row->addWidget(label);
    row->addWidget(eccmRad);
    label = new QLabel(tr("Beam: "));
    eccmBeam = new QComboBox();
    eccmBeam->addItems(ranges);
    row->addWidget(label);
    row->addWidget(eccmBeam);
    label = new QLabel(" ");
    row->addWidget(label,1);
    mainLay->addLayout(row);

    connect(eccmVal,SIGNAL(valueChanged(int)),SLOT(updateECM()));
    connect(eccmRad,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
    connect(eccmBeam,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    box->setLayout(mainLay);
    return box;
}

void ControlEntry::someDataChanged()
{
    emit updatedControls();
}

void ControlEntry::updateECM()
{
    // Update all the ECM suites
    ECM* ecm = mecha->getECMPtr("Sensor Jamming");
    ecm->setValue(sensJamVal->value());
    ecm->setLoc(ecmLoc->currentText());
    ecm->setRadius(sensJamRad->currentText().toInt());
    ecm->setBeaming(sensJamBeam->currentText().toInt());

    ecm = mecha->getECMPtr("Missile Jamming");
    ecm->setValue(missJamVal->value());
    ecm->setLoc(ecmLoc->currentText());
    ecm->setRadius(missJamRad->currentText().toInt());
    ecm->setBeaming(missJamBeam->currentText().toInt());

    ecm = mecha->getECMPtr("Radar Jamming");
    ecm->setValue(radJamVal->value());
    ecm->setLoc(ecmLoc->currentText());
    ecm->setRadius(radJamRad->currentText().toInt());
    ecm->setBeaming(radJamBeam->currentText().toInt());

    ecm = mecha->getECMPtr("ECCM");
    ecm->setValue(eccmVal->value());
    ecm->setLoc(ecmLoc->currentText());
    ecm->setRadius(eccmRad->currentText().toInt());
    ecm->setBeaming(eccmBeam->currentText().toInt());

    mecha->getBiggestECMPtr()->setEff(ecmEff->value());

    // set ecmEff
    disconnect(ecmEff,SIGNAL(valueChanged(double)),this,SLOT(updateECM()));
    ecmEff->setValue(mecha->getBiggestECMPtr()->getEff());
    connect(ecmEff,SIGNAL(valueChanged(double)),SLOT(updateECM()));

    // set Cost and Space
    ecmTotCost->setText(QString::number(mecha->getECMCost()));
    ecmSpace->setText(QString::number(mecha->getBiggestECMPtr()->getSpace()));

    emit updatedControls();
}

void ControlEntry::addCockpit()
{
    if (cpName->text() == "" || cpLoc->currentText() == "None")
    {
        QMessageBox msg;
        msg.setText("Your cockpit should have a valid name and location.");
        msg.exec();
        return;
    }

    Cockpit* cp = new Cockpit();

    cp->setName(cpName->text());
    cp->setLoc(cpLoc->currentText());
    cp->setCrew(cpCrew->value());
    cp->setPassengers(cpPass->value());
    cp->setEnc(cpEnc->currentText());
    cp->setEff(0);

    mecha->addCockpit(cp);
    cpModel->insertRows(cpModel->rowCount(),1);

    emit updatedControls();
}

void ControlEntry::remCockpit()
{
    if (!cockpitTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(cockpitTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    cpModel->removeRows(index.row(),1);
    mecha->removeCockpit(sel);
    emit updatedControls();
}

void ControlEntry::addSensor()
{
    if (sName->text() == "" || sLoc->currentText() == "None")
    {
        QMessageBox msg;
        msg.setText("Your sensors should have a valid name and location.");
        msg.exec();
        return;
    }

    Sensor* s = new Sensor();

    s->setName(sName->text());
    s->setLoc(sLoc->currentText());
    s->setClass(sClass->currentText());

    mecha->addSensor(s);
    sModel->insertRows(sModel->rowCount(),1);

    emit updatedControls();
}

void ControlEntry::remSensor()
{
    if (!sensorTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(sensorTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    sModel->removeRows(index.row(),1);
    mecha->removeSensor(sel);
    emit updatedControls();
}

void ControlEntry::addReconSys()
{
    if (!sensorTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(sensorTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection
    QList<Sensor*>& sList = mecha->getSensorRef();

    Recon* rec = new Recon();
    rec->setName(reconChoice->currentText());
    rec->setBaseCost(ReconComp::instance()->calcCost(reconChoice->currentText()));
    rec->setHasLevels(ReconComp::instance()->checkHasLevels(reconChoice->currentText()));
    rec->setLevels(1);
    rec->setKills(ReconComp::instance()->calcKills(reconChoice->currentText()));
    rec->setSpace(ReconComp::instance()->calcSpace(reconChoice->currentText()));

    sList[sel]->addRecon(rec);

    delete sModel;
    sModel = new SensorModel(sList);

    sensorTable->setModel(sModel);

    emit updatedControls();
}

void ControlEntry::remReconSys()
{
    if (!sensorTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(sensorTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection
    QList<Sensor*>& sList = mecha->getSensorRef();

    sList[sel]->remRecon(reconChoice->currentText());

    delete sModel;
    sModel = new SensorModel(sList);

    sensorTable->setModel(sModel);

    emit updatedControls();
}

void ControlEntry::refreshLocs()
{
    cpLoc->clear();
    cpLoc->addItem("None");
    cpLoc->addItems(mecha->getServoNames());
    cpLoc->addItem("Split");

    disconnect(ecmLoc,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));

    ecmLoc->clear();
    ecmLoc->addItem("None");
    ecmLoc->addItems(mecha->getServoNames());

    QStringList shList = mecha->getShieldNames();
    QString sh;

    foreach (sh,shList)
    {
        if (mecha->getBinderSpace(sh + " Binder"))
            ecmLoc->addItem(sh + " Binder");
    }

    ecmLoc->addItem("Split");

    ecmLoc->setCurrentIndex(ecmLoc->findText(mecha->getBiggestECMPtr()->getLoc()));

    connect(ecmLoc,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
}

void ControlEntry::refresh(Mecha *newMech)
{
    mecha = newMech;
    delete cpModel;
    cpModel = new CockpitModel(mecha->getCockpitRef());

    locDel = new LocListDelegate(mecha,false,false);

    cockpitTable->setItemDelegateForColumn(1,locDel);

    cockpitTable->setModel(cpModel);
    connect(cpModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    sModel = new SensorModel(mecha->getSensorRef());
    sensorTable->setItemDelegateForColumn(2,locDel);
    sensorTable->setModel(sModel);
    connect(sModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    // Refresh ECM here
    // Disconnect first
    disconnect(sensJamVal,SIGNAL(valueChanged(int)),this,SLOT(updateECM()));
    disconnect(sensJamRad,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));
    disconnect(sensJamBeam,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));

    disconnect(missJamVal,SIGNAL(valueChanged(int)),this,SLOT(updateECM()));
    disconnect(missJamRad,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));
    disconnect(missJamBeam,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));

    disconnect(radJamVal,SIGNAL(valueChanged(int)),this,SLOT(updateECM()));
    disconnect(radJamRad,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));
    disconnect(radJamBeam,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));

    disconnect(eccmVal,SIGNAL(valueChanged(int)),this,SLOT(updateECM()));
    disconnect(eccmRad,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));
    disconnect(eccmBeam,SIGNAL(currentIndexChanged(int)),this,SLOT(updateECM()));

    disconnect(ecmEff,SIGNAL(valueChanged(double)),this,SLOT(updateECM()));

    // set values
    sensJamVal->setValue(mecha->getECMPtr("Sensor Jamming")->getValue());
    sensJamRad->setCurrentIndex(sensJamRad->findText(QString::number(mecha->getECMPtr("Sensor Jamming")->getRadius())));
    sensJamBeam->setCurrentIndex(sensJamBeam->findText(QString::number(mecha->getECMPtr("Sensor Jamming")->getBeaming())));

    missJamVal->setValue(mecha->getECMPtr("Missile Jamming")->getValue());
    missJamRad->setCurrentIndex(sensJamRad->findText(QString::number(mecha->getECMPtr("Missile Jamming")->getRadius())));
    missJamBeam->setCurrentIndex(sensJamBeam->findText(QString::number(mecha->getECMPtr("Missile Jamming")->getBeaming())));

    radJamVal->setValue(mecha->getECMPtr("Radar Jamming")->getValue());
    radJamRad->setCurrentIndex(sensJamRad->findText(QString::number(mecha->getECMPtr("Radar Jamming")->getRadius())));
    radJamBeam->setCurrentIndex(sensJamBeam->findText(QString::number(mecha->getECMPtr("Radar Jamming")->getBeaming())));

    eccmVal->setValue(mecha->getECMPtr("ECCM")->getValue());
    eccmRad->setCurrentIndex(sensJamRad->findText(QString::number(mecha->getECMPtr("ECCM")->getRadius())));
    eccmBeam->setCurrentIndex(sensJamBeam->findText(QString::number(mecha->getECMPtr("ECCM")->getBeaming())));

    ecmEff->setValue(mecha->getBiggestECMPtr()->getEff());

    ecmTotCost->setText(QString::number(mecha->getECMCost()));
    ecmSpace->setText(QString::number(mecha->getBiggestECMPtr()->getSpace()));

    // Reconnect
    connect(sensJamVal,SIGNAL(valueChanged(int)),SLOT(updateECM()));
    connect(sensJamRad,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
    connect(sensJamBeam,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    connect(missJamVal,SIGNAL(valueChanged(int)),SLOT(updateECM()));
    connect(missJamRad,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
    connect(missJamBeam,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    connect(radJamVal,SIGNAL(valueChanged(int)),SLOT(updateECM()));
    connect(radJamRad,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
    connect(radJamBeam,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    connect(eccmVal,SIGNAL(valueChanged(int)),SLOT(updateECM()));
    connect(eccmRad,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));
    connect(eccmBeam,SIGNAL(currentIndexChanged(int)),SLOT(updateECM()));

    connect(ecmEff,SIGNAL(valueChanged(double)),SLOT(updateECM()));

    refreshLocs();
}
