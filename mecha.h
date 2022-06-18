#ifndef MECHA_H
#define MECHA_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QPair>
#include "mekton_defs.h"
#include "cockpitcontrols.h"
#include "mverniers.h"
#include "powerplant.h"
#include "remotes.h"
#include "transformation.h"

class Servo;
class Weapon;
class Ammo;
class Cockpit;
class Sensor;
class Propulsion;
class Fuel;
class Option;
class Shield;
class Reflector;
class SplittablePart;
class ECM;
class EnergyPool;

class Mecha : public QObject
{
    Q_OBJECT
public:
    explicit Mecha(QObject *parent = 0);
    ~Mecha();

    void addServo(Servo* servo);
    void removeServo(int index);

    void addWeapon(Weapon* weapon);
    void removeWeapon(int index);

    void addAmmo(Ammo* ammo);
    void removeAmmo(int index);

    void addCockpit(Cockpit* cp);
    void removeCockpit(int index);

    void addSensor(Sensor* s);
    void removeSensor(int index);

    void addFuel(Fuel* f);
    void removeFuel(int index);

    void addOption(Option* o);
    void removeOption(int index);

    void addShield(Shield* s) {shieldList.push_back(s);}
    void removeShield(int index);

    void addReflector(Reflector* r) {refList.push_back(r);}
    void removeReflector(int index);

    void addPool(EnergyPool* pool);
    void removePool(int index);

    void cleanup();
    void clear();

    // Accessors here
    const QString& getName() const {return name;}
    qreal getCost() const;
    qreal getBaseCost() const;
    qreal getFrameWeight() const;
    qreal getWeight() const;
    int getBestDamageBonus() const;
    qreal getBaseWeight() const;
    qreal getWtEffMod() const;
    int getWtEff() const {return wtEff;}
    qreal getGroundPropWeight() const;
    int getWalkSpeed() const;
    int getMV() const;
    int getBaseFuel() const {return fuelWeight;}
    int getFuelPercent() const;
    int getMaxFlight() const;
    qreal getFuelWeight() const {return (getBaseWeight() + getWtEffMod())* (getFuelPercent() / 100.0);}
    qreal getFuelCost() const;
    qreal getPropCost() const;
    qreal getECMCost() const;
    int getWheelSize() const {return wheelSize;}
    int getTreadSize() const {return treadSize;}
    int getTransKills(const QString& type,const QString& loc) const;
    int getTransKills(const QString& type) const;
    int getTotalTransKills(const QString& type) const;
    double getSpacesUsed(const QString& loc) const;
    int getNumHands() const;
    int getMaxHandSpace(int numHands) const;
    double getHeldWeaponSpace(int numHands) const;
    double getBMod() const;
    int getBinderSpace(const QString& bind) const;
    double getUnallocatedSpace() const;
    const CockpitControls& getCockpitControls() const {return controls;}

    bool hasWheels() const {return wheelSize;}
    bool hasTreads() const {return treadSize;}

    QList<Servo*>& getServoRef();
    QStringList getServoNames() const;
    QList<Weapon*>& getWeaponsRef();
    QList<Ammo*>& getAmmoRef();
    QList<EnergyPool*>& getPoolRef() {return poolList;}
    QList<Cockpit*>& getCockpitRef();
    QList<Sensor*>& getSensorRef();
    QList<Fuel*>& getFuelRef();
    QList<Option*>& getOptionRef();
    QList<Shield*>& getShieldRef() {return shieldList;}
    QList<Reflector*>& getReflectRef() {return refList;}
    QStringList getShieldNames() const;
    QList<SplittablePart*> getSplittableParts() const;
    QList<MatedSystem*>& getMatedPartsRef() {return matedList;}
    QMap<QString,Propulsion*>& getPropMapRef();
    QList<QPair<QString,double> > getMoveList() const;
    Servo* getServoByLoc(const QString& loc) const;
    QStringList getEnvironmentals() const;
    QList<EnvTypes> getEnvList() const;
    QMap<QString,ECM*>& getECMMapRef() {return ecmMap;}
    MVerniers* getVerniersPtr() {return verniers;}
    double getMultipliers() const;
    int getHydraulics() const {return hydraulics;}
    double getHydraulicsMult() const {return (hydraulics > 0) ? hydraulics*0.1 : hydraulics * 0.05;}
    double getHydraulicsSpace() const {return (hydraulics > -1) ? hydraulics : (hydraulics == -2) ? -1 : 0;}
    double getHydraulicsBonus() const {return (hydraulics > 0) ? hydraulics : 0;}
    const PowerPlant* getPowerPlant() const {return plant;}
    const Remotes* getConstRemotePtr() const {return remotes;}
    Remotes* getRemotes() {return remotes;}

    QList<SplittablePart*> getMateableSystems();

    bool addMatedSystem(MatedSystem* newSys);
    void remMatedSystem(const QString& ms);

    double getPoolMultiplier() const;
    int getPoolBonus() const {return controls.getBonus() - ((plant->getCharge() < 0 && plant->isHot()) ? 1 : 0);}
    int getVernierRating() const {return verniers->getRating();}
    int getACE() const {return ACE;}
    double getACEMult() const {return (ACE * 0.05) + (ACE == 3 ? 0.05 : 0);}
    double getACEBonus() const {return ACE * 0.333;}

    // Cloaking stuff
    int getCloakLevel() const {return cloakLevel;}
    bool hasStealth() const {return stealth;}
    bool hasPulseRefract() const {return pulseRefract;}
    bool hasMagneticRefract() const {return magRefract;}
    bool hasBeamRefract() const {return beamRefract;}
    bool hasFireControl() const {return fireControl;}
    bool hasCombatCloak() const {return combatCloak;}

    // Transformations
    const Transformation& getForm(const QString& form) {if (forms.contains(form)) return *forms.value(form); else return *defaultForm;}
    bool hasForm(const QString& form) {return forms.contains(form);}
    void addForm(const Transformation& form) {if (!hasForm(form.getName()))
        {Transformation* tf = new Transformation(form); forms.insert(form.getName(),tf); if (!defaultForm) defaultForm=forms.value(form.getName());
         if (hasForm("Humanoid")) defaultForm=forms.value("Humanoid"); }}
    void remForm(const QString& form);
    QStringList getFormList() const;
    QString getDefaultFormName() const {return (defaultForm) ? defaultForm->getName() : QString("Humanoid");}
    double getTFTotalCost() const;

    ECM* getECMPtr(const QString& ecm) {return ecmMap[ecm];}
    ECM* getBiggestECMPtr() const;

    bool usesFuel() const;

    QStringList getComplications() const;

    // Setters
    void setBaseFuel(int newVal) {fuelWeight = newVal;}
    void setWheelSize(int newVal) {wheelSize = newVal; cleanLocs();}
    void setTreadSize(int newVal) {treadSize = newVal; cleanLocs();}
    void setTransKills(const QString& type,const QString& loc, int kills);
    void setName(const QString& newVal) {name = newVal;}
    void setEnvironmental(EnvTypes env, bool has);
    void setWeightEff(int newVal) {wtEff = newVal;}
    void setCockpitControls(const CockpitControls& newVal) {controls = newVal;}
    void setManeuverVerniers(int newVal) {verniers->setRating(newVal);}
    void setECM(const QString& name, ECM* ecm) {ecmMap[name] = ecm;}
    void setHydraulics(int newVal) {hydraulics = newVal;}
    void setPowerPlant(PowerPlant* newVal) {delete plant; plant = newVal;}
    void setACE(int newVal) {ACE = newVal;}
    void setRemotes(Remotes* newVal) {delete remotes; remotes = newVal;}
    void setCloakLevel(int newVal) {cloakLevel = newVal;}
    void setPulseRefract(bool newVal) {pulseRefract = newVal;}
    void setMagneticRefract(bool newVal) {magRefract = newVal;}
    void setBeamRefract(bool newVal) {beamRefract = newVal;}
    void setFireControl(bool newVal) {fireControl = newVal;}
    void setCombatCloak(bool newVal) {combatCloak = newVal;}
    void setStealth(bool newVal) {stealth = newVal;}
    void setDefaultForm(const QString& newVal) {if (hasForm(newVal)) defaultForm = forms.value(newVal);}

    void cleanLocs();

signals:

public slots:

private:
    QString name;

    QList<Servo*> servoList;
    QList<Weapon*> weaponList;
    QList<Ammo*> ammoList;
    QList<EnergyPool*> poolList;
    QList<Cockpit*> cockpitList;
    QList<Sensor*> sensorList;
    QMap<QString,Propulsion*> propMap;
    QList<Fuel*> fuelList;
    QList<Option*> optionList;
    QList<Shield*> shieldList;
    QList<Reflector*> refList;
    QMap<EnvTypes,bool> envMap;
    QMap<QString,ECM*> ecmMap;

    CockpitControls controls;
    MVerniers* verniers;

    int wheelSize;
    int treadSize;

    QMap<QString,int> wheelKills;
    QMap<QString,int> treadKills;

    int fuelWeight;
    int wtEff;
    int hydraulics;
    int ACE;

    PowerPlant* plant;
    Remotes* remotes;

    // Weapon mating stuff
    QList<MatedSystem*> matedList;

    // Cloaking Stuff
    int cloakLevel;
    bool pulseRefract;
    bool magRefract;
    bool beamRefract;
    bool fireControl;
    bool combatCloak;
    bool stealth;

    QMap<QString,Transformation*> forms;
    Transformation* defaultForm;

    qreal getServoCost() const;
    qreal getServoWeight() const;
    qreal getWeaponWeight() const;
    qreal getWeaponCost() const;
    qreal getAmmoCost() const;
    qreal getAmmoWeight() const;
    qreal getControlCost() const;
    qreal getControlWeight() const;
    qreal getOptionCost() const;
    qreal getShieldCost() const;
    qreal getReflectCost() const;
    qreal getShieldWeight() const;
    int getClumsyPenalty() const;
    void cleanAmmo(const QString& weapon);
    void cleanServoLocs(const QString& loc);
    bool locExists(const QString& loc) const;
};

inline QList<Servo*>& Mecha::getServoRef()
{
    return servoList;
}

inline QList<Weapon*>& Mecha::getWeaponsRef()
{
    return weaponList;
}

inline QList<Ammo*>& Mecha::getAmmoRef()
{
    return ammoList;
}

inline QList<Cockpit*>& Mecha::getCockpitRef()
{
    return cockpitList;
}

inline QList<Sensor*>& Mecha::getSensorRef()
{
    return sensorList;
}

inline QList<Fuel*>& Mecha::getFuelRef()
{
    return fuelList;
}

inline QList<Option*>& Mecha::getOptionRef()
{
    return optionList;
}

inline QMap<QString,Propulsion*>& Mecha::getPropMapRef()
{
    return propMap;
}

inline void Mecha::addServo(Servo* servo)
{
    servoList.push_back(servo);
}

inline void Mecha::addAmmo(Ammo* ammo)
{
    ammoList.push_back(ammo);
}

inline void Mecha::addCockpit(Cockpit* cp)
{
    cockpitList.push_back(cp);
}

inline void Mecha::addSensor(Sensor* s)
{
    sensorList.push_back(s);
}

inline void Mecha::addFuel(Fuel* f)
{
    fuelList.push_back(f);
}

inline void Mecha::addOption(Option* o)
{
    optionList.push_back(o);
}

inline QStringList Mecha::getFormList() const
{
    QStringList formList;

    Transformation* tf;

    foreach (tf,forms)
        formList.append(tf->getName());

    if (forms.empty())
        formList.append("Humanoid");

    return formList;
}

#endif // MECHA_H
