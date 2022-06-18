#include "propentry.h"
#include "propulsion.h"
#include "fuelmodel.h"
#include "fuel.h"
#include "mecha.h"
#include "servo.h"
#include "comboboxdelegate.h"
#include "spinboxdelegate.h"
#include "dspinboxdelegate.h"
#include "loclistdelegate.h"
#include "servocomp.h"
#include "propmodel.h"
#include "gtmodel.h"
#include "mektonfuncs.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QMap>
#include <QScrollArea>

PropEntry::PropEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QVBoxLayout* entryLayout = new QVBoxLayout();

    QHBoxLayout* hRow = new QHBoxLayout();

    QGroupBox* box = new QGroupBox(tr("Weight Efficiency"));

    QLabel* label = new QLabel(tr("Reduce by "));

    wtEff = new QSpinBox();
    wtEff->setRange(0,mecha->getWeight());
    wtEff->setSuffix(tr(" tons."));
    wtEff->setValue(0);

    connect(wtEff,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    hRow->addWidget(label);
    hRow->addWidget(wtEff);

    box->setLayout(hRow);

    hRow = new QHBoxLayout();

    hRow->addWidget(box);

    box = new QGroupBox(tr("Weight Inefficiency"));

    label = new QLabel(tr("Multiply by "));

    wtInEff = new QSpinBox();
    wtInEff->setRange(1,50);
    wtInEff->setSuffix(tr("x"));
    wtInEff->setValue(1);

    connect(wtInEff,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    QHBoxLayout* nRow = new QHBoxLayout();

    nRow->addWidget(label);
    nRow->addWidget(wtInEff);

    box->setLayout(nRow);

    hRow->addWidget(box);

    entryLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    hRow->addWidget(buildPropMABox());
    hRow->addWidget(buildPropSpaceBox());

    entryLayout->addLayout(hRow);

    hRow = new QHBoxLayout();

    hRow->addWidget(buildFuelBox());
    hRow->addWidget(buildGTBox());
    entryLayout->addLayout(hRow);

    baseWidget->setLayout(entryLayout);
    scrollArea->setWidget(baseWidget);
    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

QStringList PropEntry::calcValidSizes() const
{
    QStringList validSizes;
    validSizes.push_back("None");

    QList<Servo*>& servos = mecha->getServoRef();

    ServoSizes torsoSize = SUPERLIGHT;

    Servo* servo;

    foreach (servo,servos)
    {
        if (servo->getType() == TORSO)
            torsoSize = servo->getSize();
    }

    if (torsoSize)
        torsoSize = ServoSizes(torsoSize - 1);

    int i = torsoSize;
    while (i < ServoComp::instance()->getSizes().size())
    {
        validSizes.push_back(ServoComp::instance()->getSizes()[i]);
        ++i;
    }

    return validSizes;
}

QGroupBox* PropEntry::buildPropMABox()
{
    QGroupBox* box = new QGroupBox("Propulsion Movement");

    QVBoxLayout* boxLayout = new QVBoxLayout();

    QHBoxLayout* propRow;

    QLabel* label;

    QStringList validSizes = calcValidSizes();

    // Build the checkboxes
    propRow = new QHBoxLayout();
    label = new QLabel(tr("Wheels"));
    label->setMaximumWidth(150);
    label->setMinimumWidth(150);
    wheels = new QComboBox();
    wheels->addItems(validSizes);
    propRow->addWidget(label,0,Qt::AlignLeft);
    propRow->addWidget(wheels,0,Qt::AlignLeft);
    label = new QLabel(tr("Kills: "));
    label->setMaximumWidth(45);
    propRow->addWidget(label,0,Qt::AlignLeft);
    wheelWeight = new QLabel("0");
    wheelWeight->setMaximumWidth(40);
    propRow->addWidget(wheelWeight,0,Qt::AlignLeft);
    label = new QLabel(tr(""));
    propRow->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(propRow);

    connect(wheels,SIGNAL(currentIndexChanged(int)),SLOT(updatePropulsion()));

    propRow = new QHBoxLayout();
    label = new QLabel(tr("Treads"));
    label->setMaximumWidth(150);
    label->setMinimumWidth(150);
    treads = new QComboBox();
    treads->addItems(validSizes);
    propRow->addWidget(label,0,Qt::AlignLeft);
    propRow->addWidget(treads,0,Qt::AlignLeft);
    label = new QLabel(tr("Kills: "));
    label->setMaximumWidth(45);
    propRow->addWidget(label,0,Qt::AlignLeft);
    treadWeight = new QLabel("0");
    treadWeight->setMaximumWidth(40);
    propRow->addWidget(treadWeight,0,Qt::AlignLeft);
    label = new QLabel(tr(""));
    propRow->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(propRow);

    connect(treads,SIGNAL(currentIndexChanged(int)),SLOT(updatePropulsion()));

    // Build the spinboxes
    propRow = new QHBoxLayout();
    label = new QLabel(tr("GES"));
    label->setMaximumWidth(150);
    label->setMinimumWidth(150);
    GESBox = new QSpinBox();
    GESBox->setRange(0,20);
    propRow->addWidget(label,0,Qt::AlignLeft);
    propRow->addWidget(GESBox,0,Qt::AlignLeft);
    label = new QLabel(tr("Cost: "));
    label->setMaximumWidth(40);
    propRow->addWidget(label,0,Qt::AlignLeft);
    GESCost = new QLabel("0.0");
    GESCost->setMaximumWidth(40);
    propRow->addWidget(GESCost,0,Qt::AlignLeft);
    label = new QLabel(tr("Requires Fuel"));
    propRow->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(propRow);

    connect(GESBox,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    propRow = new QHBoxLayout();
    label = new QLabel(tr("HydroJet"));
    label->setMaximumWidth(150);
    label->setMinimumWidth(150);
    HydroJetBox = new QSpinBox();
    HydroJetBox->setRange(0,2000);
    propRow->addWidget(label,0,Qt::AlignLeft);
    propRow->addWidget(HydroJetBox,0,Qt::AlignLeft);
    label = new QLabel(tr("Cost: "));
    label->setMaximumWidth(40);
    propRow->addWidget(label,0,Qt::AlignLeft);
    HydroJetCost = new QLabel("0.0");
    HydroJetCost->setMaximumWidth(40);
    propRow->addWidget(HydroJetCost,0,Qt::AlignLeft);
    label = new QLabel(tr("Requires Fuel"));
    propRow->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(propRow);

    connect(HydroJetBox,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    propRow = new QHBoxLayout();
    label = new QLabel(tr("HydroThrusters"));
    label->setMaximumWidth(150);
    label->setMinimumWidth(150);
    HydroThrustBox = new QSpinBox();
    HydroThrustBox->setRange(0,2000);
    propRow->addWidget(label,0,Qt::AlignLeft);
    propRow->addWidget(HydroThrustBox,0,Qt::AlignLeft);
    label = new QLabel(tr("Cost: "));
    label->setMaximumWidth(40);
    propRow->addWidget(label,0,Qt::AlignLeft);
    HydroThrustCost = new QLabel("0.0");
    HydroThrustCost->setMaximumWidth(40);
    propRow->addWidget(HydroThrustCost,0,Qt::AlignLeft);
    label = new QLabel(tr("Requires Fuel"));
    propRow->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(propRow);

    connect(HydroThrustBox,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    propRow = new QHBoxLayout();
    label = new QLabel(tr("MagnetoHydroDynamics"));
    label->setMaximumWidth(150);
    label->setMinimumWidth(150);
    MHDBox = new QSpinBox();
    MHDBox->setRange(0,2000);
    propRow->addWidget(label,0,Qt::AlignLeft);
    propRow->addWidget(MHDBox,0,Qt::AlignLeft);
    label = new QLabel(tr("Cost: "));
    label->setMaximumWidth(40);
    propRow->addWidget(label,0,Qt::AlignLeft);
    MHDCost = new QLabel("0.0");
    MHDCost->setMaximumWidth(40);
    propRow->addWidget(MHDCost,0,Qt::AlignLeft);
    label = new QLabel(tr(""));
    propRow->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(propRow);

    connect(MHDBox,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    propRow = new QHBoxLayout();
    label = new QLabel(tr("Thrusters"));
    label->setMaximumWidth(150);
    label->setMinimumWidth(150);
    ThrusterBox = new QSpinBox();
    ThrusterBox->setRange(0,2000);
    propRow->addWidget(label,0,Qt::AlignLeft);
    propRow->addWidget(ThrusterBox,0,Qt::AlignLeft);
    label = new QLabel(tr("Cost: "));
    label->setMaximumWidth(40);
    propRow->addWidget(label,0,Qt::AlignLeft);
    ThrusterCost = new QLabel("0.0");
    ThrusterCost->setMaximumWidth(40);
    propRow->addWidget(ThrusterCost,0,Qt::AlignLeft);
    label = new QLabel(tr("Requires Fuel"));
    propRow->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(propRow);

    connect(ThrusterBox,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    propRow = new QHBoxLayout();
    label = new QLabel(tr("Gravitics"));
    label->setMaximumWidth(150);
    label->setMinimumWidth(150);
    GravBox = new QSpinBox();
    GravBox->setRange(0,2000);
    propRow->addWidget(label,0,Qt::AlignLeft);
    propRow->addWidget(GravBox,0,Qt::AlignLeft);
    label = new QLabel(tr("Cost: "));
    label->setMaximumWidth(40);
    propRow->addWidget(label,0,Qt::AlignLeft);
    GravCost = new QLabel("0.0");
    GravCost->setMaximumWidth(40);
    propRow->addWidget(GravCost,0,Qt::AlignLeft);
    label = new QLabel(tr(""));
    propRow->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(propRow);

    connect(GravBox,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    box->setLayout(boxLayout);

    box->setMaximumWidth(380);
    return box;
}

QGroupBox* PropEntry::buildPropSpaceBox()
{
    QGroupBox* box = new QGroupBox(tr("Propulsion Spaces"));

    QVBoxLayout* boxLayout = new QVBoxLayout();

    QHBoxLayout* hLayout = new QHBoxLayout();

    QLabel* label = new QLabel(tr("System: "));
    propChoice = new QComboBox();
    propChoice->setMinimumWidth(150);

    QStringList choices;
    choices.push_back("None");
    choices.append(mecha->getPropMapRef().keys());

    hLayout->addWidget(label);
    hLayout->addWidget(propChoice);

    boxLayout->addLayout(hLayout);

    label = new QLabel(tr("Unassigned Space: "));
    propUnSpace = new QLabel(tr("0"));
    hLayout->addWidget(label);
    hLayout->addWidget(propUnSpace);

    label = new QLabel(tr("Eff: "));
    propEff = new QDoubleSpinBox();
    propEff->setRange(0,500);
    propEff->setDecimals(1);
    propEff->setSingleStep(0.1);

    connect(propEff,SIGNAL(valueChanged(double)),SLOT(updatePropulsion()));

    hLayout->addWidget(label);
    hLayout->addWidget(propEff);

    propChoice->addItems(choices);

    connect(propChoice,SIGNAL(currentIndexChanged(QString)),SLOT(resetPropTable(QString)));

    pModel = new PropModel(mecha,"None");

    propTable = new QTableView();


    propTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    propTable->setSelectionMode(QAbstractItemView::SingleSelection);
    propTable->setModel(pModel);

    DSpinBoxDelegate* effDelegate = new DSpinBoxDelegate(0,200,1);

    propTable->setItemDelegateForColumn(1,effDelegate);

    propTable->setColumnWidth(0,60);
    propTable->setColumnWidth(1,70);

    connect(pModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    boxLayout->addWidget(propTable);

    box->setLayout(boxLayout);

    return box;
}

QGroupBox* PropEntry::buildGTBox()
{
    QGroupBox* box = new QGroupBox(tr("Ground Transport Kills"));

    QVBoxLayout* boxLayout = new QVBoxLayout();

    QHBoxLayout* hLayout = new QHBoxLayout();

    QLabel* label = new QLabel(tr("System: "));
    gtChoice = new QComboBox();
    gtChoice->setMinimumWidth(50);

    QStringList choices;
    choices.push_back("Wheels");
    choices.push_back("Treads");;

    hLayout->addWidget(label);
    hLayout->addWidget(gtChoice);

    boxLayout->addLayout(hLayout);

    label = new QLabel(tr("Unassigned Kills: "));
    gtUnKills = new QLabel(tr("0"));
    hLayout->addWidget(label);
    hLayout->addWidget(gtUnKills);

    gtChoice->addItems(choices);

    connect(gtChoice,SIGNAL(currentIndexChanged(QString)),SLOT(resetGTTable(QString)));

    gtModel = new GTModel(mecha,"Wheels");

    gtTable = new QTableView();


    gtTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    gtTable->setSelectionMode(QAbstractItemView::SingleSelection);
    gtTable->setModel(gtModel);

    SpinBoxDelegate* killDelegate = new SpinBoxDelegate(0,22);

    gtTable->setItemDelegateForColumn(1,killDelegate);

    gtTable->setColumnWidth(0,60);
    gtTable->setColumnWidth(1,70);

    connect(gtModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    boxLayout->addWidget(gtTable);

    box->setLayout(boxLayout);

    return box;
}

void PropEntry::resetGTTable(const QString &gtName)
{
    delete gtModel;
    gtModel = new GTModel(mecha,gtName);

    gtUnKills->setText(QString::number(mecha->getTotalTransKills(gtName) - mecha->getTransKills(gtName)));

    connect(gtModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
    gtTable->setModel(gtModel);
}

QGroupBox* PropEntry::buildFuelBox()
{
    QGroupBox* box = new QGroupBox(tr("Fuel"));

    QVBoxLayout* boxLayout = new QVBoxLayout();

    QHBoxLayout* hLayout = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Internal Fuel: "));

    hLayout->addWidget(label,0,Qt::AlignLeft);

    fuelWeight = new QSpinBox();
    fuelWeight->setRange(0,10);
    fuelWeight->setValue(0);
    fuelWeight->setDisabled(true);
    fuelWeight->setSuffix("%");

    connect(fuelWeight,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

    hLayout->addWidget(fuelWeight,0,Qt::AlignLeft);

    label = new QLabel(tr(" of Weight"));
    hLayout->addWidget(label,1,Qt::AlignLeft);
    boxLayout->addLayout(hLayout);

    hLayout = new QHBoxLayout();

    label = new QLabel(tr("Maximum Distance"));
    hLayout->addWidget(label,0,Qt::AlignLeft);
    maxDistance = new QLabel("0km");
    hLayout->addWidget(maxDistance,1,Qt::AlignLeft);
    boxLayout->addLayout(hLayout);

    label = new QLabel(tr("Extra Fuel"));
    QFont labelFont;
    labelFont.setBold(true);
    labelFont.setPointSize(10);
    label->setFont(labelFont);
    hLayout = new QHBoxLayout();
    hLayout->addWidget(label,0,Qt::AlignCenter);
    boxLayout->addLayout(hLayout);

    fuelTable = new QTableView(this);
    fModel = new FuelModel(mecha->getFuelRef());

    fuelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    fuelTable->setSelectionMode(QAbstractItemView::SingleSelection);
    fuelTable->setModel(fModel);

    QStringList intExtList;
    intExtList.push_back("Internal");
    intExtList.push_back("External");

    ComboBoxDelegate* extDelegate = new ComboBoxDelegate(intExtList);
    llDelegate = new LocListDelegate(mecha,false,false,true,false);
    SpinBoxDelegate* numDelegate = new SpinBoxDelegate(1,99);
    DSpinBoxDelegate* effDelegate = new DSpinBoxDelegate(0,200,1);

    fuelTable->setItemDelegateForColumn(0,extDelegate);
    fuelTable->setItemDelegateForColumn(1,llDelegate);
    fuelTable->setItemDelegateForColumn(2,numDelegate);
    fuelTable->setItemDelegateForColumn(5,effDelegate);

    fuelTable->setColumnWidth(0,60);
    fuelTable->setColumnWidth(1,70);
    fuelTable->setColumnWidth(2,40);
    fuelTable->setColumnWidth(3,40);
    fuelTable->setColumnWidth(4,40);
    fuelTable->setColumnWidth(5,40);

    connect(fModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    boxLayout->addWidget(fuelTable);

    QPushButton* addTank = new QPushButton(tr("Add Tank"));
    QPushButton* remTank = new QPushButton(tr("Remove Tank"));

    connect(addTank,SIGNAL(clicked()),SLOT(addFuel()));
    connect(remTank,SIGNAL(clicked()),SLOT(remFuel()));

    hLayout = new QHBoxLayout();

    hLayout->addWidget(addTank);
    hLayout->addWidget(remTank);

    boxLayout->addLayout(hLayout);

    box->setLayout(boxLayout);

    box->setMaximumWidth(380);

    return box;
}

void PropEntry::updatePropulsion()
{
    if (wtEff->value())
    {
        if (wtEff->value() > mecha->getBaseWeight())
            wtEff->setValue((int)mecha->getBaseWeight() - 1);

        wtInEff->setDisabled(true);
    }
    else wtInEff->setEnabled(true);

    mecha->setWeightEff(wtEff->value());

    if (wtInEff->value() > 1)
    {
        mecha->setWeightEff(- (wtInEff->value() - 1));
        wtEff->setDisabled(true);
    }
    else wtEff->setEnabled(true);

    if (wheels->currentIndex())
    {
        mecha->setWheelSize(ServoComp::instance()->getSizeByString(wheels->currentText())+1);
    }
    else
    {
        mecha->setWheelSize(0);
    }

    if (treads->currentIndex())
    {
        mecha->setTreadSize(ServoComp::instance()->getSizeByString(treads->currentText())+1);
    }
    else
    {
        mecha->setTreadSize(0);
    }

    QMap<QString,Propulsion*>& propMap = mecha->getPropMapRef();

    // Set ratings for all of them
    if (GESBox->value())
    {
        if (propMap["GES"])
        {
            propMap["GES"]->setRating(GESBox->value());
            propMap["GES"]->normalize(mecha->getWeight());
        }
        else
        {
            propMap["GES"] = new Propulsion("GES");
            propMap["GES"]->setRating(GESBox->value());
            propMap["GES"]->normalize(mecha->getWeight());
        }
    }
    else
    {
        if (propMap["GES"])
        {
            delete propMap["GES"];
            propMap.remove("GES");
        }
    }

    if (HydroJetBox->value())
    {
        if (propMap["HydroJets"])
        {
            propMap["HydroJets"]->setRating(HydroJetBox->value());
            propMap["HydroJets"]->normalize(mecha->getWeight());
        }
        else
        {
            propMap["HydroJets"] = new Propulsion("HydroJets");
            propMap["HydroJets"]->setRating(HydroJetBox->value());
            propMap["HydroJets"]->normalize(mecha->getWeight());
        }
    }
    else
    {
        if (propMap["HydroJets"])
        {
            delete propMap["HydroJets"];
            propMap.remove("HydroJets");
        }
    }

    if (HydroThrustBox->value())
    {
        if (propMap["HydroThrusters"])
        {
            propMap["HydroThrusters"]->setRating(HydroThrustBox->value());
            propMap["HydroThrusters"]->normalize(mecha->getWeight());
        }
        else
        {
            propMap["HydroThrusters"] = new Propulsion("HydroThrusters");
            propMap["HydroThrusters"]->setRating(HydroThrustBox->value());
            propMap["HydroThrusters"]->normalize(mecha->getWeight());
        }
    }
    else
    {
        if (propMap["HydroThrusters"])
        {
            delete propMap["HydroThrusters"];
            propMap.remove("HydroThrusters");
        }
    }

    if (MHDBox->value())
    {
        if (propMap["MagnetoHydroDynamics"])
        {
            propMap["MagnetoHydroDynamics"]->setRating(MHDBox->value());
            propMap["MagnetoHydroDynamics"]->normalize(mecha->getWeight());
        }
        else
        {
            propMap["MagnetoHydroDynamics"] = new Propulsion("MagnetoHydroDynamics");
            propMap["MagnetoHydroDynamics"]->setRating(MHDBox->value());
            propMap["MagnetoHydroDynamics"]->normalize(mecha->getWeight());
        }
    }
    else
    {
        if (propMap["MagnetoHydroDynamics"])
        {
            delete propMap["MagnetoHydroDynamics"];
            propMap.remove("MagnetoHydroDynamics");
        }
    }

    if (ThrusterBox->value())
    {
        if (propMap["Thrusters"])
        {
            propMap["Thrusters"]->setRating(ThrusterBox->value());
            propMap["Thrusters"]->normalize(mecha->getWeight());
        }
        else
        {
            propMap["Thrusters"] = new Propulsion("Thrusters");
            propMap["Thrusters"]->setRating(ThrusterBox->value());
            propMap["Thrusters"]->normalize(mecha->getWeight());
        }
    }
    else
    {
        if (propMap["Thrusters"])
        {
            delete propMap["Thrusters"];
            propMap.remove("Thrusters");
        }
    }

    if (GravBox->value())
    {
        if (propMap["Gravitics"])
        {
            propMap["Gravitics"]->setRating(GravBox->value());
            propMap["Gravitics"]->normalize(mecha->getWeight());
        }
        else
        {
            propMap["Gravitics"] = new Propulsion("Gravitics");
            propMap["Gravitics"]->setRating(GravBox->value());
            propMap["Gravitics"]->normalize(mecha->getWeight());
        }
    }
    else
    {
        if (propMap["Gravitics"])
        {
            delete propMap["Gravitics"];
            propMap.remove("Gravitics");
        }
    }

    mecha->setBaseFuel(fuelWeight->value());

    if (propChoice->currentIndex() && mecha->getPropMapRef()[propChoice->currentText()])
    {
        mecha->getPropMapRef()[propChoice->currentText()]->setEff(propEff->value(),mecha->getWeight());
    }

    refreshEntry();

    emit propChanged();
}

void PropEntry::adjustSizes()
{
    wheels->clear();
    treads->clear();

    QStringList valid = calcValidSizes();

    wheels->addItems(valid);
    treads->addItems(valid);
}

void PropEntry::refreshEntry()
{
    QString s;

    wheelWeight->setText(QString::number(mecha->getWheelSize() + ((mecha->getWheelSize() == 11) ? 1 :0)));

    treadWeight->setText(QString::number(mecha->getTreadSize() *2));

    if (mecha->getPropMapRef().contains("GES") && mecha->getPropMapRef()["GES"])
    {
        s.sprintf("%.2f",mecha->getPropMapRef()["GES"]->getCost(mecha->getWeight()));
        GESCost->setText(s);
    }
    else GESCost->setText("0.00");

    if (mecha->getPropMapRef().contains("HydroJets") && mecha->getPropMapRef()["HydroJets"])
    {
        s.sprintf("%.2f",mecha->getPropMapRef()["HydroJets"]->getCost(mecha->getWeight()));
        HydroJetCost->setText(s);
    }
    else HydroJetCost->setText("0.00");

    if (mecha->getPropMapRef().contains("HydroThrusters") && mecha->getPropMapRef()["HydroThrusters"])
    {
        s.sprintf("%.2f",mecha->getPropMapRef()["HydroThrusters"]->getCost(mecha->getWeight()));
        HydroThrustCost->setText(s);
    }
    else HydroThrustCost->setText("0.00");;

    if (mecha->getPropMapRef().contains("MagnetoHydroDynamics") && mecha->getPropMapRef()["MagnetoHydroDynamics"])
    {
        s.sprintf("%.2f",mecha->getPropMapRef()["MagnetoHydroDynamics"]->getCost(mecha->getWeight()));
        MHDCost->setText(s);
    }
    else MHDCost->setText("0.00");;

    if (mecha->getPropMapRef().contains("Thrusters") && mecha->getPropMapRef()["Thrusters"])
    {
        s.sprintf("%.2f",mecha->getPropMapRef()["Thrusters"]->getCost(mecha->getWeight()));
        ThrusterCost->setText(s);
    }
    else ThrusterCost->setText("0.00");;

    if (mecha->getPropMapRef().contains("Gravitics") && mecha->getPropMapRef()["Gravitics"])
    {
        s.sprintf("%.2f",mecha->getPropMapRef()["Gravitics"]->getCost(mecha->getWeight()));
        GravCost->setText(s);
    }
    else GravCost->setText("0.00");;

    fuelWeight->setValue(mecha->getBaseFuel());

    if (mecha->usesFuel())
    {
        fuelWeight->setEnabled(true);
    }
    else
    {
        fuelWeight->setDisabled(true);
    }

    maxDistance->setText(QString::number(mecha->getMaxFlight()) + "km");

    // check to see if prop combo box needs updated
    QStringList compStr;
    compStr.push_back("None");
    mecha->cleanup();
    compStr.append(mecha->getPropMapRef().keys());

    QStringList boxStr;

    for (int i = 0;i < propChoice->count();++i)
    {
        boxStr.push_back(propChoice->itemText(i));
    }

    if (!(compStr == boxStr))
    {
        propChoice->clear();
        propChoice->addItems(compStr);
    }

    if (propChoice->currentIndex() && mecha->getPropMapRef()[propChoice->currentText()])
    {
        QString s;
        s.sprintf("%.1f",mecha->getPropMapRef()[propChoice->currentText()]->getTotalSpace(mecha->getWeight())
                  - mecha->getPropMapRef()[propChoice->currentText()]->getAllocSpace());
        propUnSpace->setText(s);
    }
    else
    {
        propUnSpace->setText("0");
    }

    gtUnKills->setText(QString::number(mecha->getTotalTransKills(gtChoice->currentText()) - mecha->getTransKills(gtChoice->currentText())));
}

void PropEntry::resetPropTable(const QString &propName)
{
    if (propName == "")
        return;

    if (propName != "None"  && mecha->getPropMapRef()[propChoice->currentText()])
    {
        propEff->setValue(mecha->getPropMapRef()[propName]->getEff());
    }
    else propEff->setValue(0);

    if (propChoice->currentIndex() && mecha->getPropMapRef()[propChoice->currentText()])
    {
        QString s;
        s.sprintf("%.1f",mecha->getPropMapRef()[propChoice->currentText()]->getTotalSpace(mecha->getWeight())
                  - mecha->getPropMapRef()[propChoice->currentText()]->getAllocSpace());
        propUnSpace->setText(s);
    }
    else
    {
        propUnSpace->setText("0");
    }

    delete pModel;
    pModel = new PropModel(mecha,propName);

    connect(pModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
    propTable->setModel(pModel);
}

void PropEntry::someDataChanged()
{
    refreshEntry();
    emit propChanged();
}

void PropEntry::addFuel()
{
    Fuel* fuel = new Fuel();

    mecha->addFuel(fuel);
    fModel->insertRows(fModel->rowCount(),1);

    refreshEntry();
    emit propChanged();
}

void PropEntry::remFuel()
{
    if (!fuelTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(fuelTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    int sel = index.row();

    // we only allow one selection

    fModel->removeRows(index.row(),1);
    mecha->removeFuel(sel);

    refreshEntry();
    emit propChanged();
}

void PropEntry::refresh(Mecha *newMech)
{
    mecha = newMech;
    delete fModel;
    fModel = new FuelModel(mecha->getFuelRef());
    fuelTable->setModel(fModel);

    llDelegate = new LocListDelegate(mecha,false,false,true,false);

    fuelTable->setItemDelegateForColumn(1,llDelegate);

    connect(fModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

   // propChoice->setCurrentIndex(0);

    disconnect(fuelWeight,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(wheels,SIGNAL(currentIndexChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(treads,SIGNAL(currentIndexChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(wtEff,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(wtInEff,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));

    wtEff->setValue(0);
    wtInEff->setValue(1);
    wtInEff->setDisabled(true);
    wtEff->setEnabled(true);
    adjustSizes();
    refillValues();
    refreshEntry();
    resetPropTable("None");
    resetGTTable("Wheels");
    if (mecha->getWtEff() > 0)
        wtEff->setValue(mecha->getWtEff());
    else {
        wtInEff->setValue(1 - mecha->getWtEff());
        wtInEff->setEnabled(true);
        if (wtInEff->value() != 1)
            wtEff->setDisabled(true);
    }

    connect(fuelWeight,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));
    connect(wheels,SIGNAL(currentIndexChanged(int)),this,SLOT(updatePropulsion()));
    connect(treads,SIGNAL(currentIndexChanged(int)),this,SLOT(updatePropulsion()));
    connect(wtEff,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));
    connect(wtInEff,SIGNAL(valueChanged(int)),SLOT(updatePropulsion()));

}

void PropEntry::refillValues()
{
    disconnect(GESBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(HydroJetBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(HydroThrustBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(MHDBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(ThrusterBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    disconnect(GravBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));

    if (mecha->getPropMapRef().contains("GES") && mecha->getPropMapRef()["GES"])
    {
        GESBox->setValue(mecha->getPropMapRef()["GES"]->getRating());
    }
    else GESBox->setValue(0);

    if (mecha->getPropMapRef().contains("HydroJets") && mecha->getPropMapRef()["HydroJets"])
    {
        HydroJetBox->setValue(mecha->getPropMapRef()["HydroJets"]->getRating());
    }
    else HydroThrustBox->setValue(0);

    if (mecha->getPropMapRef().contains("HydroThrusters") && mecha->getPropMapRef()["HydroThrusters"])
    {
        HydroThrustBox->setValue(mecha->getPropMapRef()["HydroThrusters"]->getRating());
    }
    else HydroThrustBox->setValue(0);;

    if (mecha->getPropMapRef().contains("MagnetoHydroDynamics") && mecha->getPropMapRef()["MagnetoHydroDynamics"])
    {
        MHDBox->setValue(mecha->getPropMapRef()["MagnetoHydroDynamics"]->getRating());
    }
    else MHDBox->setValue(0);;

    if (mecha->getPropMapRef().contains("Thrusters") && mecha->getPropMapRef()["Thrusters"])
    {
        ThrusterBox->setValue(mecha->getPropMapRef()["Thrusters"]->getRating());
    }
    else ThrusterBox->setValue(0);;

    if (mecha->getPropMapRef().contains("Gravitics") && mecha->getPropMapRef()["Gravitics"])
    {
        GravBox->setValue(mecha->getPropMapRef()["Gravitics"]->getRating());
    }
    else GravBox->setValue(0);

    connect(GESBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    connect(HydroJetBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    connect(HydroThrustBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    connect(MHDBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    connect(ThrusterBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));
    connect(GravBox,SIGNAL(valueChanged(int)),this,SLOT(updatePropulsion()));

    disconnect(propChoice,SIGNAL(currentIndexChanged(QString)),this,SLOT(resetPropTable(QString)));

    propChoice->setCurrentIndex(0);

    connect(propChoice,SIGNAL(currentIndexChanged(QString)),this,SLOT(resetPropTable(QString)));

    if (mecha->hasWheels())
        wheels->setCurrentIndex(wheels->findText(ServoComp::instance()->getSizeName((ServoSizes)(mecha->getWheelSize()-1))));
    else wheels->setCurrentIndex(0);

    if (mecha->hasTreads())
        treads->setCurrentIndex(treads->findText(ServoComp::instance()->getSizeName((ServoSizes)(mecha->getTreadSize()-1))));
    else treads->setCurrentIndex(0);

}
