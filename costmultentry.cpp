#include "costmultentry.h"
#include "mecha.h"
#include "powerplant.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QScrollArea>

CostMultEntry::CostMultEntry(Mecha* mecha,QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* hRow = new QHBoxLayout();

    hRow->addWidget(buildEnviroBox());

    QVBoxLayout* vStack = new QVBoxLayout();

    vStack->addWidget(buildControlBox());
    vStack->addWidget(buildVernierBox());
    vStack->addWidget(buildHydraulicsBox());

    hRow->addLayout(vStack);

    vStack = new QVBoxLayout();
    vStack->addWidget(buildPlantBox());
    vStack->addWidget(buildACEBox());

    hRow->addLayout(vStack);

    hRow->addWidget(buildCloakBox());

    mainLayout->addLayout(hRow);

    connectAll();

    baseWidget->setLayout(mainLayout);
    scrollArea->setWidget(baseWidget);
    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

QGroupBox* CostMultEntry::buildEnviroBox()
{
    QGroupBox* box = new QGroupBox(tr("Environmental Protection"));

    arctic = new QCheckBox(tr("Arctic"));
    desert = new QCheckBox(tr("Desert"));
    underwater = new QCheckBox(tr("Underwater"));
    highPressure = new QCheckBox(tr("High Pressure"));
    space = new QCheckBox(tr("Space"));
    EMprot = new QCheckBox(tr("EM"));
    reEntry = new QCheckBox(tr("Re-Entry"));

    QVBoxLayout* vLayout = new QVBoxLayout();

    vLayout->addWidget(arctic);
    vLayout->addWidget(desert);
    vLayout->addWidget(underwater);
    vLayout->addWidget(highPressure);
    vLayout->addWidget(space);
    vLayout->addWidget(EMprot);
    vLayout->addWidget(reEntry);

    box->setLayout(vLayout);

    box->setMaximumSize(140,200);

    return box;
}

QGroupBox* CostMultEntry::buildCloakBox()
{
    QGroupBox* box = new QGroupBox(tr("Cloaking"));

    cloakLevel = new QComboBox();
    cloakLevel->addItem(tr("None"));
    cloakLevel->addItem(tr("Basic"));
    cloakLevel->addItem(tr("Active"));
    cloakLevel->setCurrentIndex(0);

    stealth = new QCheckBox(tr("Stealth"));
    combat = new QCheckBox(tr("Combat Cloak"));
    fireControl = new QCheckBox(tr("Fire Control"));
    pulse = new QCheckBox(tr("Pulse Refract"));
    magnetic = new QCheckBox(tr("Magnetic Refract"));
    beam = new QCheckBox(tr("Beam Refract"));

    // Set Cloaking Options Disabled
    combat->setDisabled(true);
    fireControl->setDisabled(true);
    pulse->setDisabled(true);
    magnetic->setDisabled(true);
    beam->setDisabled(true);

    QVBoxLayout* vLayout = new QVBoxLayout();

    vLayout->addWidget(cloakLevel);
    vLayout->addWidget(stealth);
    vLayout->addWidget(combat);
    vLayout->addWidget(fireControl);
    vLayout->addWidget(pulse);
    vLayout->addWidget(magnetic);
    vLayout->addWidget(beam);

    box->setLayout(vLayout);

    box->setMaximumSize(140,200);

    return box;
}

QGroupBox* CostMultEntry::buildControlBox()
{
    QGroupBox* box = new QGroupBox(tr("Cockpit Controls"));
    QVBoxLayout* vLayout = new QVBoxLayout();

    controls = new QComboBox();
    controls->addItem("Manual");
    controls->addItem("Screen");
    controls->addItem("Virtual");
    controls->addItem("Reflex");
    controls->setCurrentIndex(1);

    vLayout->addWidget(controls);
    box->setLayout(vLayout);

    box->setMaximumSize(150,70);

    return box;
}

QGroupBox* CostMultEntry::buildACEBox()
{
    QGroupBox* box = new QGroupBox(tr("ACE"));
    QVBoxLayout* vLayout = new QVBoxLayout();

    ACE = new QComboBox();
    ACE->addItem("None");
    ACE->addItem("+33%");
    ACE->addItem("+67%");
    ACE->addItem("+100%");
    ACE->setCurrentIndex(0);

    vLayout->addWidget(ACE);
    box->setLayout(vLayout);

    box->setMaximumSize(150,70);

    return box;
}

QGroupBox* CostMultEntry::buildHydraulicsBox()
{
    QGroupBox* box = new QGroupBox(tr("Hydraulics"));
    QVBoxLayout* vLayout = new QVBoxLayout();

    hydraulics = new QComboBox();
    hydraulics->addItem("Space");
    hydraulics->addItem("Marine");
    hydraulics->addItem("Standard");
    hydraulics->addItem("Heavy");
    hydraulics->addItem("Superheavy");
    hydraulics->setCurrentIndex(2);

    vLayout->addWidget(hydraulics);
    box->setLayout(vLayout);

    box->setMaximumSize(150,70);

    return box;
}

QGroupBox* CostMultEntry::buildPlantBox()
{
    QGroupBox* box = new QGroupBox(tr("Power Plant"));
    QVBoxLayout* vLayout = new QVBoxLayout();

    plantCharge = new QComboBox();
    plantCharge->addItem("Undercharged");
    plantCharge->addItem("Standard");
    plantCharge->addItem("Overcharged");
    plantCharge->addItem("Supercharged");
    plantCharge->setCurrentIndex(1);

    plantSource = new QComboBox();
    plantSource->addItem("Fusion");
    plantSource->addItem("Bioenergy");
    plantSource->addItem("Power Cell");
    plantSource->addItem("Combustion");
    plantSource->setCurrentIndex(0);

    plantHot = new QCheckBox(tr("Hot"));

    vLayout->addWidget(plantCharge);
    vLayout->addWidget(plantSource);
    vLayout->addWidget(plantHot);
    box->setLayout(vLayout);

    box->setMaximumSize(150,140);

    return box;
}

QGroupBox* CostMultEntry::buildVernierBox()
{
    QGroupBox* box = new QGroupBox(tr("Maneuver Verniers"));
    QVBoxLayout* vLayout = new QVBoxLayout();

    verniers = new QSpinBox();
    verniers->setRange(0,10);
    verniers->setPrefix("+");

    vLayout->addWidget(verniers);
    box->setLayout(vLayout);

    box->setMaximumSize(150,75);

    return box;
}

void CostMultEntry::connectAll()
{
    connect(arctic,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(desert,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(underwater,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(highPressure,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(space,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(EMprot,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(reEntry,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));

    connect(controls,SIGNAL(currentIndexChanged(int)),SLOT(someDataChanged()));

    connect(verniers,SIGNAL(valueChanged(int)),SLOT(someDataChanged()));

    connect(hydraulics,SIGNAL(currentIndexChanged(int)),SLOT(someDataChanged()));

    connect(plantCharge,SIGNAL(currentIndexChanged(int)),SLOT(someDataChanged()));
    connect(plantSource,SIGNAL(currentIndexChanged(int)),SLOT(someDataChanged()));
    connect(plantHot,SIGNAL(toggled(bool)),SLOT(someDataChanged()));

    connect(ACE,SIGNAL(currentIndexChanged(int)),SLOT(someDataChanged()));

    connect(cloakLevel,SIGNAL(currentIndexChanged(int)),SLOT(someDataChanged()));
    connect(stealth,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(combat,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(fireControl,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(pulse,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(beam,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
    connect(magnetic,SIGNAL(stateChanged(int)),SLOT(someDataChanged()));
}

void CostMultEntry::disconnectAll()
{
    disconnect(arctic,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(desert,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(underwater,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(highPressure,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(space,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(EMprot,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(reEntry,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));

    disconnect(controls,SIGNAL(currentIndexChanged(int)),this,SLOT(someDataChanged()));

    disconnect(verniers,SIGNAL(valueChanged(int)),this,SLOT(someDataChanged()));

    disconnect(hydraulics,SIGNAL(currentIndexChanged(int)),this,SLOT(someDataChanged()));

    disconnect(plantCharge,SIGNAL(currentIndexChanged(int)),this,SLOT(someDataChanged()));
    disconnect(plantSource,SIGNAL(currentIndexChanged(int)),this,SLOT(someDataChanged()));
    disconnect(plantHot,SIGNAL(toggled(bool)),this,SLOT(someDataChanged()));

    disconnect(ACE,SIGNAL(currentIndexChanged(int)),this,SLOT(someDataChanged()));

    disconnect(cloakLevel,SIGNAL(currentIndexChanged(int)),this,SLOT(someDataChanged()));
    disconnect(stealth,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(combat,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(fireControl,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(pulse,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(beam,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
    disconnect(magnetic,SIGNAL(stateChanged(int)),this,SLOT(someDataChanged()));
}

void CostMultEntry::someDataChanged()
{
    setEnviro();

    CockpitControls control;

    control.setName(controls->currentText());
    control.setCostMult((controls->currentIndex() -1) * 0.05);
    if (controls->currentIndex())
        control.setBonusMult((controls->currentIndex() -1) * 0.333);
    else control.setBonus(-2);

    mecha->setCockpitControls(control);

    mecha->setManeuverVerniers(verniers->value());

    mecha->setHydraulics(hydraulics->currentIndex() - 2);

    PowerPlant* pp = new PowerPlant();

    pp->setCharge(plantCharge->currentIndex() - 1);
    pp->setSource((PlantSources)plantSource->currentIndex());
    pp->setHot(plantHot->isChecked());

    mecha->setPowerPlant(pp);

    mecha->setACE(ACE->currentIndex());

    setCloaking();

    emit updatedMult();
}

void CostMultEntry::setEnviro()
{
    mecha->setEnvironmental(ARCTIC,arctic->isChecked());
    mecha->setEnvironmental(DESERT,desert->isChecked());
    mecha->setEnvironmental(UNDERWATER,underwater->isChecked());
    mecha->setEnvironmental(HIGH_PRESSURE,highPressure->isChecked());
    mecha->setEnvironmental(SPACE,space->isChecked());
    mecha->setEnvironmental(EM,EMprot->isChecked());
    mecha->setEnvironmental(RE_ENTRY,reEntry->isChecked());
}

void CostMultEntry::setCloaking()
{
    mecha->setCloakLevel(cloakLevel->currentIndex());
    mecha->setStealth(stealth->isChecked());

    // Set Availables
    if (cloakLevel->currentIndex())
    {
        combat->setEnabled(true);
        fireControl->setEnabled(true);
    }
    else
    {
        combat->setChecked(false);
        combat->setDisabled(true);
        fireControl->setChecked(false);
        fireControl->setDisabled(true);
    }

    if (cloakLevel->currentIndex() < 2)
    {
        pulse->setDisabled(true);
        pulse->setChecked(false);
        magnetic->setDisabled(true);
        magnetic->setChecked(false);
        beam->setDisabled(true);
        beam->setChecked(false);
    }
    else
    {
        pulse->setEnabled(true);
        magnetic->setEnabled(true);
        beam->setEnabled(true);
    }

    // Set Mecha
    mecha->setCombatCloak(combat->isChecked());
    mecha->setFireControl(fireControl->isChecked());
    mecha->setPulseRefract(pulse->isChecked());
    mecha->setBeamRefract(beam->isChecked());
    mecha->setMagneticRefract(magnetic->isChecked());
}

void CostMultEntry::refresh(Mecha *newMech)
{
    mecha = newMech;
    disconnectAll();

    QList<EnvTypes> envs = mecha->getEnvList();

    arctic->setChecked(envs.contains(ARCTIC));
    desert->setChecked(envs.contains(DESERT));
    underwater->setChecked(envs.contains(UNDERWATER));
    highPressure->setChecked(envs.contains(HIGH_PRESSURE));
    space->setChecked(envs.contains(SPACE));
    EMprot->setChecked(envs.contains(EM));
    reEntry->setChecked(envs.contains(RE_ENTRY));

    controls->setCurrentIndex(controls->findText(mecha->getCockpitControls().getName()));

    verniers->setValue(mecha->getVernierRating());

    hydraulics->setCurrentIndex(mecha->getHydraulics()+2);

    const PowerPlant* pp = mecha->getPowerPlant();

    plantCharge->setCurrentIndex(pp->getCharge() + 1);
    plantSource->setCurrentIndex((int)pp->getSource());
    plantHot->setChecked(pp->isHot());

    ACE->setCurrentIndex(mecha->getACE());

    cloakLevel->setCurrentIndex(mecha->getCloakLevel());
    stealth->setChecked(mecha->hasStealth());
    combat->setChecked(mecha->hasCombatCloak());
    fireControl->setChecked(mecha->hasFireControl());
    pulse->setChecked(mecha->hasPulseRefract());
    beam->setChecked(mecha->hasBeamRefract());
    magnetic->setChecked(mecha->hasMagneticRefract());

    connectAll();
}
