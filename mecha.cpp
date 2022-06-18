#include "mecha.h"
#include "servo.h"
#include "weapon.h"
#include "ammo.h"
#include "cockpit.h"
#include "sensor.h"
#include "propulsion.h"
#include "fuel.h"
#include "option.h"
#include "shield.h"
#include "reflector.h"
#include "splittablepart.h"
#include "ecm.h"
#include "powerplant.h"
#include "energypool.h"
#include "matedsystem.h"
#include <math.h>
#include <QRegExp>
#include <QHash>

Mecha::Mecha(QObject *parent) :
    QObject(parent),
    name(""),
    wheelSize(0),
    treadSize(0),
    fuelWeight(0),
    wtEff(0),
    hydraulics(0),
    ACE(0),
    cloakLevel(0),
    pulseRefract(false),
    magRefract(false),
    beamRefract(false),
    fireControl(false),
    combatCloak(false),
    stealth(false)
{
    // Let's give them a generic Mecha
    Servo* myServo = new Servo();

    myServo->setDesignation("T");
    myServo->setType(TORSO);
    myServo->setSize(STRIKER);
    myServo->setArmorSize(STRIKER);
    myServo->setArmorType(NORMAL);

    servoList.push_back(myServo);

    myServo = new Servo();

    myServo->setDesignation("H");
    myServo->setType(HEAD);
    myServo->setSize(STRIKER);
    myServo->setArmorSize(STRIKER);
    myServo->setArmorType(NORMAL);

    servoList.push_back(myServo);

    myServo = new Servo();

    myServo->setDesignation("RL");
    myServo->setType(LEG);
    myServo->setSize(STRIKER);
    myServo->setArmorSize(STRIKER);
    myServo->setArmorType(NORMAL);

    servoList.push_back(myServo);

    myServo = new Servo();

    myServo->setDesignation("LL");
    myServo->setType(LEG);
    myServo->setSize(STRIKER);
    myServo->setArmorSize(STRIKER);
    myServo->setArmorType(NORMAL);

    servoList.push_back(myServo);

    myServo = new Servo();

    myServo->setDesignation("RA");
    myServo->setType(ARM);
    myServo->setSize(STRIKER);
    myServo->setArmorSize(STRIKER);
    myServo->setArmorType(NORMAL);

    servoList.push_back(myServo);

    myServo = new Servo();

    myServo->setDesignation("LA");
    myServo->setType(ARM);
    myServo->setSize(STRIKER);
    myServo->setArmorSize(STRIKER);
    myServo->setArmorType(NORMAL);

    servoList.push_back(myServo);

    // Don't forget the cockpit!
    Cockpit* cp = new Cockpit();
    cp->setLoc("T");
    cp->setName("Main Cockpit");
    cockpitList.push_back(cp);

    // Sensors...
    Sensor* s = new Sensor();
    s->setLoc("H");
    s->setName("Main");
    s->setClass("Striker");
    s->setEff(0);
    sensorList.push_back(s);

    // Verniers
    verniers = new MVerniers();

    // ECM
    ecmMap["Sensor Jamming"] = new ECM("Sensor Jamming");
    ecmMap["Missile Jamming"] = new ECM("Missile Jamming");
    ecmMap["Radar Jamming"] = new ECM("Radar Jamming");
    ecmMap["ECCM"] = new ECM("ECCM");

    // Power Plant
    plant = new PowerPlant();

    // Remotes
    remotes = new Remotes();

    // Forms
    Transformation* tf = new Transformation("Humanoid");
    tf->addProp("Legs");
    tf->addProp("GES");
    tf->addProp("Thrusters");
    tf->addProp("Gravitics");
    tf->addProp("Hydrojets");
    tf->addProp("Wheels");
    tf->addProp("Treads");
    forms.insert("Humanoid",tf);
    defaultForm = tf;
}

Mecha::~Mecha()
{
    this->clear();
    delete verniers;

    delete ecmMap["Sensor Jamming"];
    delete ecmMap["Missile Jamming"];
    delete ecmMap["Radar Jamming"];
    delete ecmMap["ECCM"];

    delete plant;

    delete remotes;
}

// Delete everything!
void Mecha::clear()
{
    wheelKills.clear();
    treadKills.clear();

    wtEff = 0;
    fuelWeight = 0;
    hydraulics = 0;
    ACE = 0;

    cloakLevel = 0;
    pulseRefract = false;
    magRefract = false;
    beamRefract = false;
    fireControl = false;
    combatCloak = false;
    stealth = false;

    wheelSize = 0;
    treadSize = 0;

    Servo* s;
    foreach (s,servoList)
        delete s;

    servoList.clear();

    Weapon* wp;
    foreach (wp,weaponList)
        delete wp;

    weaponList.clear();

    Ammo* a;
    foreach (a,ammoList)
        delete a;

    ammoList.clear();

    Cockpit* cp;
    Sensor* sen;
    Propulsion* p;
    Fuel* f;
    Option* o;

    foreach (cp,cockpitList)
        delete cp;

    cockpitList.clear();

    foreach (sen,sensorList)
        delete sen;

    sensorList.clear();

    foreach (f,fuelList)
        delete f;

    fuelList.clear();

    foreach (o,optionList)
        delete o;

    optionList.clear();

    foreach (p,propMap)
    {
        if (p)
            delete p;
    }

    propMap.clear();

    Shield* sh;
    foreach (sh,shieldList)
        delete sh;

    shieldList.clear();

    Reflector* ref;
    foreach (ref,refList)
        delete ref;

    refList.clear();

    EnergyPool* pool;
    foreach (pool,poolList)
        delete pool;

    poolList.clear();

    envMap.clear();

    controls.setName("Screen");
    controls.setBonus(0);
    controls.setBonusMult(0);
    controls.setCostMult(0);

    verniers->setRating(0);
    verniers->normalize(getServoNames());

    // ECM
    delete ecmMap["Sensor Jamming"];
    delete ecmMap["Missile Jamming"];
    delete ecmMap["Radar Jamming"];
    delete ecmMap["ECCM"];
    ecmMap["Sensor Jamming"] = new ECM("Sensor Jamming");
    ecmMap["Missile Jamming"] = new ECM("Missile Jamming");
    ecmMap["Radar Jamming"] = new ECM("Radar Jamming");
    ecmMap["ECCM"] = new ECM("ECCM");

    delete plant;
    plant = new PowerPlant();

    delete remotes;
    remotes = new Remotes();

    Transformation* tf;
    foreach (tf,forms)
        delete tf;

    forms.clear();
    defaultForm = 0;
}

qreal Mecha::getBaseCost() const
{
    return getServoCost() + getWeaponCost() + getAmmoCost() + getControlCost()
            + getFuelCost() + getPropCost() + getOptionCost() + getShieldCost()
            + getReflectCost() + getECMCost();
}

qreal Mecha::getCost() const
{
    return getBaseCost() * (1 + getMultipliers()) + (wtEff > 0 ? 2*wtEff : 0);
}

qreal Mecha::getBaseWeight() const
{
    return getFrameWeight() + getWeaponWeight() + getAmmoWeight() + getControlWeight() + getGroundPropWeight() + getShieldWeight();
}

qreal Mecha::getWtEffMod() const
{
    qreal base = getBaseWeight();

    if (wtEff > 0)
    {
        if (wtEff > base)
            return - (base - 1);
        else
            return - wtEff;
    }
    else if (wtEff < 0)
    {
        return - wtEff * base;
    }

    return 0;
}

qreal Mecha::getWeight() const
{
    return getBaseWeight() + getWtEffMod() + getFuelWeight() + (remotes->getClass() ? remotes->getWeight() : 0);
}

qreal Mecha::getFrameWeight() const
{
    return getServoWeight();
}

qreal Mecha::getServoCost() const
{
    qreal cost = 0.0;

    Servo* servo;

    foreach (servo,servoList)
    {
        cost+=servo->getTotalCost();
    }

    return cost;
}

qreal Mecha::getServoWeight() const
{
    qreal wt = 0.0;

    Servo* servo;

    foreach (servo,servoList)
    {
        wt+=servo->getTotalWeight();
    }

    return wt;
}

qreal Mecha::getShieldCost() const
{
    qreal cost = 0.0;

    Shield* sh;

    foreach (sh,shieldList)
    {
        cost += sh->getTotalCost();
    }

    return cost;
}

qreal Mecha::getShieldWeight() const
{
    qreal wt = 0.0;

    Shield* sh;

    foreach(sh,shieldList)
    {
        wt+= sh->getSP() / 2.0;
    }

    return wt;
}

void Mecha::addWeapon(Weapon *weapon)
{
    QString baseName = weapon->getName();
    QRegExp rx("^" + baseName + "\\s?(\\d{0,4})$");

    int count = 0;

    Weapon* wp;
    foreach (wp, weaponList)
    {
        if (rx.exactMatch(wp->getName()))
            ++count;

        if (rx.cap(1).toInt() > count)
            count = rx.cap(1).toInt();
    }

    if (count > 0)
        weapon->setName(weapon->getName() + " " + QString::number(count + 1));

    weaponList.push_back(weapon);
}

void Mecha::addPool(EnergyPool* pool)
{
    QString baseName = pool->getName();
    QRegExp rx("^" + baseName + "\\s?(\\d{0,4})$");

    int count = 0;

    EnergyPool* p;
    foreach (p, poolList)
    {
        if (rx.exactMatch(p->getName()))
            ++count;

        if (rx.cap(1).toInt() > count)
            count = rx.cap(1).toInt();
    }

    if (count > 0)
        pool->setName(pool->getName() + " " + QString::number(count + 1));

    poolList.push_back(pool);
}

QStringList Mecha::getServoNames() const
{
    QStringList names;
    Servo* servo;
    foreach (servo,servoList)
    {
        names.push_back(servo->getDesignation());
    }
    return names;
}

int Mecha::getWalkSpeed() const
{
    qreal wt = getWeight();

    if (wt > 80.0)
    {
        wt = 80.0;
    }

    int pen = getClumsyPenalty();

    return 6 - (int)trunc(wt/20.0) - pen + plant->getCharge();
}

int Mecha::getMV() const
{
    qreal wt = getWeight();

    if (wt > 100.0)
    {
        wt = 100.0;
    }
    else if (wt < 10.0)
    {
        wt = 10.0;
    }

    int mv = -(int)trunc(wt/10.0);

    mv += verniers->getRating() + plant->getCharge();

    return (mv > 0) ? 0 : mv;
}

ECM* Mecha::getBiggestECMPtr() const
{
    QString biggest = "Sensor Jamming";

    ECM* ecm;
    foreach (ecm,ecmMap)
    {
        if (ecm->getBaseCost() > ecmMap[biggest]->getBaseCost())
            biggest = ecm->getName();
    }

    return ecmMap[biggest];
}

qreal Mecha::getECMCost() const
{
    qreal cost = 0;

    QString biggest = "Sensor Jamming";

    ECM* ecm;
    foreach (ecm,ecmMap)
    {
        if (ecm->getBaseCost() > ecmMap[biggest]->getBaseCost())
            biggest = ecm->getName();
    }

    foreach (ecm,ecmMap)
    {
        if (ecm->getName() == biggest)
            cost += ecm->getCost();
        else cost += ecm->getBaseCost() / 2.0;
    }

    return cost;
}

void Mecha::removeServo(int index)
{
    QString name;
    if (index >= 0 && index < servoList.size())
    {
        Servo* s = servoList[index];
        name = s->getDesignation();
        delete s;
        servoList.removeAt(index);
    }

    cleanServoLocs(name);
}

void Mecha::removeAmmo(int index)
{
    if (index >= 0 && index < ammoList.size())
    {
        Ammo* a = ammoList[index];
        delete a;
        ammoList.removeAt(index);
    }
}

void Mecha::removeCockpit(int index)
{
    if (index >= 0 && index < cockpitList.size())
    {
        Cockpit* cp = cockpitList[index];
        delete cp;
        cockpitList.removeAt(index);
    }
}

void Mecha::removeSensor(int index)
{
    if (index >= 0 && index < sensorList.size())
    {
        Sensor* s = sensorList[index];
        delete s;
        sensorList.removeAt(index);
    }
}

void Mecha::removeFuel(int index)
{
    if (index >= 0 && index < fuelList.size())
    {
        Fuel* f = fuelList[index];
        delete f;
        fuelList.removeAt(index);
    }
}

void Mecha::removeOption(int index)
{
    if (index >= 0 && index < optionList.size())
    {
        Option* o = optionList[index];
        delete o;
        optionList.removeAt(index);
    }
}

void Mecha::removeShield(int index)
{
    if (index >= 0 && index < shieldList.size())
    {
        Shield* s = shieldList[index];
        delete s;
        shieldList.removeAt(index);
    }
}

void Mecha::removeReflector(int index)
{
    if (index >= 0 && index < refList.size())
    {
        Reflector* r = refList[index];
        delete r;
        refList.removeAt(index);
    }
}

void Mecha::removePool(int index)
{
    if (index >= 0 && index < poolList.size())
    {
        EnergyPool* p = poolList[index];
        delete p;
        poolList.removeAt(index);
    }
}

void Mecha::removeWeapon(int index)
{
    if (index >= 0 && index < weaponList.size())
    {
        Weapon* w = weaponList[index];
        QString name = w->getName();
        delete w;
        weaponList.removeAt(index);
        foreach (w,weaponList)
        {
            if (w->getName() == name)
                return;
        }

        // only remove ammo if no other weapons of that type are there.
        cleanAmmo(name);
    }
}

int Mecha::getBestDamageBonus() const
{
    Servo* s;
    int topDam = 0;

    foreach (s,servoList)
    {
        if (s->getType() == ARM && s->getDamageBonus() > topDam)
            topDam = s->getDamageBonus();
    }

    return topDam;
}

qreal Mecha::getWeaponCost() const
{
    qreal cost = 0.0;

    Weapon* wp;

    foreach (wp,weaponList)
    {
        cost+=wp->getCost();
        cost+=wp->getLinkCost();
    }

    EnergyPool* pool;

    foreach (pool, poolList)
    {
        cost+= pool->getCost();
    }

    return cost;
}

qreal Mecha::getWeaponWeight() const
{
    qreal wt = 0.0;

    Weapon* wp;

    foreach (wp,weaponList)
    {
        wt+=wp->getWeight();
    }

    EnergyPool* pool;

    foreach (pool, poolList)
    {
        wt+= pool->getKills() /2;
    }

    return wt;
}

qreal Mecha::getAmmoCost() const
{
    qreal cost = 0.0;

    Ammo* a;

    foreach (a,ammoList)
    {
        cost+=a->getCost();
    }

    return cost;
}

qreal Mecha::getReflectCost() const
{
    qreal cost = 0.0;

    Reflector* r;

    foreach (r,refList)
    {
        cost+=r->getCost();
    }

    return cost;
}

qreal Mecha::getAmmoWeight() const
{
    qreal wt = 0.0;

    Ammo* a;

    foreach (a,ammoList)
    {
        wt+=a->getWeight();
    }

    return wt;
}

qreal Mecha::getControlCost() const
{
    qreal cost = 0.0;

    Cockpit* cp;

    foreach (cp,cockpitList)
    {
        cost+=cp->getCost();
    }

    Sensor* s;

    foreach (s,sensorList)
    {
        cost += s->getCost();
    }

    return cost;
}

qreal Mecha::getControlWeight() const
{
    qreal wt = 0.0;

    Sensor* s;

    foreach (s,sensorList)
    {
        wt+=s->getKills();
    }

    return wt/2.0;
}


int Mecha::getClumsyPenalty() const
{
    int biggest = 0;

    Weapon* wp;

    foreach (wp,weaponList)
    {
        if (wp->isClumsy())
        {
            if ((wp->getDamage() / 3) > static_cast<unsigned int>(biggest))
                biggest = wp->getDamage() / 3;
        }
    }

    return biggest;
}

QStringList Mecha::getComplications() const
{
    QStringList comps;
    Weapon* wp;

    foreach (wp,weaponList)
    {
        if (wp->getLoc() == "None")
        {
            comps.push_back("Weapon: " + wp->getName() + " has no valid Location.");
            continue;
        }

        Servo* s = getServoByLoc(wp->getLoc());
        if (wp->isClumsy() && ((s == NULL) || (s->getType() != LEG)))
        {
            comps.push_back("Weapon: " + wp->getName() + " is designated Clumsy but not mounted on a Leg.");
        }
    }

    return comps;
}

Servo* Mecha::getServoByLoc(const QString &loc) const
{
    Servo* s;

    foreach (s,servoList)
    {
        if (s->getDesignation() == loc)
        {
            return s;
        }
    }

    return NULL;
}

void Mecha::cleanAmmo(const QString& weapon)
{
    QList<Ammo*>::iterator it = ammoList.begin();


    while (it != ammoList.end())
    {
        if ((*it)->getWeaponName() == weapon)
        {
            delete *it;
            it = ammoList.erase(it);
            continue;
        }
        ++it;
    }
}

// Makes sure every part has a valid Location ID
void Mecha::cleanServoLocs(const QString &loc)
{
    Weapon* wp;

    foreach (wp,weaponList)
    {
        if (wp->getLoc() == loc)
            wp->setLoc("None");
        wp->normalize(getServoNames());
    }

    Ammo* ammo;

    foreach (ammo,ammoList)
    {
        if (ammo->getLoc() == loc)
            ammo->setLoc("None");
    }

    Cockpit* cp;

    foreach (cp,cockpitList)
    {
        if (cp->getLoc() == loc)
            cp->setLoc("None");
    }

    Sensor* s;
    foreach (s,sensorList)
    {
        if (s->getLoc() == loc)
            s->setLoc("None");
    }

    Fuel* fuel;
    foreach (fuel,fuelList)
    {
        if (fuel->getLoc() == loc)
            fuel->setLoc("None");
    }

    Option* o;
    foreach (o,optionList)
    {
        if (o->getLoc() == loc)
            o->setLoc("None");
    }

    cleanLocs();

    Propulsion* prop;

    foreach (prop,propMap)
    {
        if (!prop)
            continue;
        prop->cleanLocs(getServoNames());
    }

    Reflector* ref;

    foreach (ref,refList)
    {
        if (ref->getLoc() == loc)
            ref->setLoc("None");
    }

    EnergyPool* pool;

    foreach (pool, poolList)
    {
        if (pool->getLoc() == loc)
            pool->setLoc("None");
    }
}

QList<QPair<QString,double> > Mecha::getMoveList() const
{
    QList<QPair<QString,double> > moveList;

    QPair<QString,double> move;

    // Walking speed
    move.first = "Walk";
    move.second = getWalkSpeed();

    moveList.push_back(move);

    // Ground speed
    move.first = "Ground";
    move.second = 0;

    Propulsion* prop;

    foreach (prop,propMap)
    {
        if (!prop)
            continue;
        if (move.first == prop->getMode())
        {
            move.second += prop->getRating();
        }
    }

    if ((move.second < getWalkSpeed()+2) && (hasWheels()))
        move.second = getWalkSpeed() + 2;

    if (move.second)
        moveList.push_back(move);

    // Air speed
    move.first = "Air";
    move.second = 0;

    foreach (prop,propMap)
    {
        if (!prop)
            continue;
        if (move.first == prop->getMode())
        {
            move.second += prop->getRating();
        }
    }

    if ((move.second < 8) && (!propMap["Gravitics"]))
        move.first = "Jump";
    else
    {
        // see if we have wings
        int wings = 0;
        Servo* s;

        foreach (s,servoList)
        {
            if (s->getType() == WING)
                ++wings;
        }

        if (wings > 1)
            move.second += 2;
    }

    if (move.second)
        moveList.push_back(move);

    // Water speed
    move.first = "Water";
    move.second = 0;

    foreach (prop,propMap)
    {
        if (!prop)
            continue;
        if (move.first == prop->getMode() || prop->getType() == "Gravitics")
        {
            move.second += prop->getRating();
        }
    }

    move.second /= 3;

    if (move.second)
        moveList.push_back(move);

    return moveList;
}

bool Mecha::usesFuel() const
{
    Propulsion* prop;

    foreach (prop,propMap)
    {
        if (prop && prop->usesFuel())
            return true;
    }

    return false;
}

int Mecha::getFuelPercent() const
{
    int total = fuelWeight;

    Fuel* fuel;

    foreach (fuel,fuelList)
    {
        total+= fuel->getWeightPercent();
    }

    return total;
}

int Mecha::getMaxFlight() const
{
    int max = fuelWeight *100;

    Fuel* fuel;

    foreach (fuel,fuelList)
    {
        max += fuel->getFlight();
    }

    return max;
}

qreal Mecha::getFuelCost() const
{
    double cost = 0;

    Fuel* fuel;

    foreach (fuel,fuelList)
    {
        cost += fuel->getCost();
    }

    return cost;
}

qreal Mecha::getOptionCost() const
{
    double cost = 0;

    Option* opt;

    foreach (opt,optionList)
    {
        cost += opt->getCost(getWeight());
    }

    cost += remotes->getCost();

    return cost;
}

qreal Mecha::getPropCost() const
{
    double cost = 0;

    Propulsion* prop;

    foreach (prop,propMap)
    {
        if (!prop)
            continue;
        cost += prop->getCost(getWeight());
    }

    cost += getWheelSize();
    if (getWheelSize() == 11)
        ++cost;

    cost += getTreadSize() * 2;

    return cost;
}

qreal Mecha::getGroundPropWeight() const
{
    qreal wt = 0;

    wt += getWheelSize();
    if (getWheelSize() == 11)
        wt+=1.0;

    wt *=0.5;

    wt += getTreadSize();

    return wt;
}

int Mecha::getTransKills(const QString &type, const QString &loc) const
{
    if (type != "Wheels" && type != "Treads")
    {
        return 0;
    }

    if (type == "Wheels")
    {
        if (!wheelKills.isEmpty() && wheelKills.contains(loc))
        {
            return wheelKills[loc];
        }
        else return 0;
    }

    if (!treadKills.isEmpty() && treadKills.contains(loc))
    {
        return treadKills[loc];
    }

    return 0;
}

double Mecha::getSpacesUsed(const QString &loc) const
{
    double space = 0;

    Weapon* w;

    QHash<QString,Weapon*> wpHash;

    foreach (w,weaponList)
    {
        wpHash[w->getName()] = w;
        if (!w->isMated())
            space += w->getSpace(loc);
    }

    Ammo* a;

    QRegExp re("^\\d+H");

    foreach (a,ammoList)
    {
        if (a->getLoc() == loc && (!re.exactMatch(wpHash[a->getWeaponName()]->getLoc()) || re.exactMatch(loc)))
            space += a->getSpace();
    }

    Cockpit* cp;

    foreach (cp,cockpitList)
    {
        space += cp->getSpace(loc);
    }

    Sensor* s;

    foreach (s,sensorList)
    {
        if (!s->isMated())
            space += s->getSpace(loc);
    }

    Propulsion* p;

    foreach (p,propMap)
    {
        if (!p)
            continue;
        space += p->getSpaces(loc);
        if (loc == "None")
            space += p->getTotalSpace(getWeight()) - p->getAllocSpace();
    }

    Fuel* f;

    foreach (f,fuelList)
    {
        if (f->getLoc() == loc)
            space += f->getSpace();
    }

    Option* o;

    foreach (o,optionList)
    {
        if (o->getLoc() == loc)
            space += o->getSpace(getWeight());
    }

    Shield* sh;

    foreach (sh,shieldList)
    {
        if (!sh->isMated())
            space += sh->getSpace(loc);
    }

    Reflector* ref;
    foreach (ref,refList)
    {
        if (!ref->isMated())
            space += ref->getSpace(loc);
    }

    EnergyPool* pool;

    foreach (pool, poolList)
    {
        if (!pool->isMated())
            space+= pool->getSpace(loc);
    }

    space += verniers->getSpace(loc);

    space += getBiggestECMPtr()->getSpace(loc);

    if (remotes->getLoc() == loc)
        space += remotes->getSpace();

    MatedSystem* ms;

    foreach (ms,matedList)
    {
        space += ms->getSpace(loc);
    }

    return space;
}

int Mecha::getTransKills(const QString &type) const
{
    QStringList servos(getServoNames());
    QString s;

    int kills = 0;

    foreach (s,servos)
    {
        kills += getTransKills(type,s);
    }
    return kills;
}

int Mecha::getTotalTransKills(const QString &type) const
{
    if (type == "Wheels")
    {
        return getWheelSize() + (getWheelSize() == 11 ? 1:0);
    }

    if (type == "Treads")
    {
        return getTreadSize() * 2;
    }
    return 0;
}

void Mecha::setTransKills(const QString &type, const QString &loc, int kills)
{
    if (type != "Wheels" && type != "Treads")
    {
        return;
    }

    if (type == "Wheels")
    {
        if (getServoNames().contains(loc))
        {
            wheelKills[loc] = kills;
            cleanLocs();
        }
        return;
    }

    if (getServoNames().contains(loc))
    {
        treadKills[loc] = kills;
        cleanLocs();
    }
}

void Mecha::cleanLocs()
{
    QStringList servos(getServoNames());
    QStringList remove;

    QString s;

    int kills = 0;

    foreach (s,wheelKills.keys())
    {
        if (servos.contains(s))
        {
            if ((wheelKills[s] + kills) > (getWheelSize() + (getWheelSize() == 11 ? 1:0)))
            {
                wheelKills[s] = (getWheelSize() + (getWheelSize() == 11 ? 1:0)) - kills;
            }
            kills += wheelKills[s];
        }
        else remove.push_back(s);
    }

    foreach (s,remove)
    {
        wheelKills.remove(s);
    }

    remove.clear();
    kills = 0;

    foreach (s,treadKills.keys())
    {
        if (servos.contains(s))
        {
            if ((treadKills[s] + kills) > (getTreadSize()*2))
            {
                treadKills[s] = (getTreadSize()*2) - kills;
            }
            kills += treadKills[s];
        }
        else remove.push_back(s);
    }

    foreach (s,remove)
        treadKills.remove(s);

    remove.clear();
}

int Mecha::getNumHands() const
{
    int num = 0;

    Weapon* wp;

    foreach (wp,weaponList)
    {
        if (wp->isHandy() && (getServoNames().contains(wp->getLoc())) && (getServoByLoc(wp->getLoc())->getType() == ARM))
            ++num;
    }

    return num;
}

int Mecha::getMaxHandSpace(int numHands) const
{
    QList<int> spaces;

    for (int i = 0;i < numHands;++i)
    {
        spaces.push_back(0);
    }

    Weapon* wp;

    foreach (wp,weaponList)
    {
        if (wp->isHandy() && (getServoNames().contains(wp->getLoc())) && (getServoByLoc(wp->getLoc())->getType() == ARM))
        {
            for (QList<int>::iterator it = spaces.begin();it != spaces.end();++it)
            {
                if (*it < (getServoByLoc(wp->getLoc())->getSize()+2))
                {
                    *it = (getServoByLoc(wp->getLoc())->getSize()+2);
                }
            }
        }
    }

    int total = 0;

    for (QList<int>::iterator it = spaces.begin();it != spaces.end();++it)
    {
        total += *it;
    }

    return total;
}

double Mecha::getHeldWeaponSpace(int numHands) const
{
    QString handStr(QString::number(numHands) + "H");

    double biggest = 0;

    Weapon* wp;

    foreach (wp,weaponList)
    {
        if (wp->isMated())
            continue;
        if ((wp->getLoc() == handStr) && (wp->getSpace() > biggest))
        {
            // Get ammo space
            double ammoSp = 0;
            Ammo* a;
            foreach (a,ammoList)
            {
                if (a->getWeaponName() == wp->getName() && a->getLoc() == handStr)
                    ammoSp += a->getSpace();
            }

            biggest = wp->getSpace() + ammoSp;
        }
    }

    EnergyPool* pool;

    foreach (pool,poolList)
    {
        if ((pool->getLoc() == handStr) && (pool->getSpace() > biggest))
        {
            biggest = pool->getSpace();
        }
    }

    foreach (MatedSystem* ms,matedList)
    {
        if ((ms->getLoc() == handStr) && (ms->getSpace() > biggest))
        {
            // Get ammo space
            double ammoSp = 0;
            Ammo* a;
            foreach (a,ammoList)
            {
                if ((a->getWeaponName() == ms->getPart(0)->getName() || a->getWeaponName() == ms->getPart(0)->getName())
                    && a->getLoc() == handStr)
                    ammoSp += a->getSpace();
            }

            biggest = ms->getSpace() + ammoSp;
        }
    }

    return biggest;
}

double Mecha::getBMod() const
{
    double mod = 0;
    Fuel* f;

    foreach (f,fuelList)
    {
        mod += f->getBMod();
    }

    return mod;
}

void Mecha::cleanup()
{
    QMap<QString,Propulsion*>::iterator it = propMap.begin();
    while (it != propMap.end())
    {
        if (!(*it))
        {
            it = propMap.erase(it);
            continue;
        }

        ++it;
    }

    Weapon* wp;

    foreach (wp,weaponList)
    {
        if (!locExists(wp->getLoc()))
            wp->setLoc("None");
    }
}

bool Mecha::locExists(const QString &loc) const
{
    if (loc == "None" || loc == "Split")
        return true;

    QStringList servos = getServoNames();

    if (servos.contains(loc))
        return true;

    QRegExp reg("(^\\d+)H");

    if (reg.exactMatch(loc))
    {
        if (reg.cap(1).toInt() <= getNumHands())
            return true;
    }

    reg.setPattern("^(.+) Mount ([1-2])");

    if (reg.exactMatch(loc) && servos.contains(reg.cap(1)) && (reg.cap(2).toInt() <= getServoByLoc(reg.cap(1))->getMounts()))
        return true;

    reg.setPattern("^(.+) Mount");

    if (reg.exactMatch(loc) && servos.contains(reg.cap(1)) && getServoByLoc(reg.cap(1))->getMounts())
        return true;

    if (getBinderSpace(loc))
        return true;

    return false;
}

QStringList Mecha::getShieldNames() const
{
    QStringList shields;
    Shield* sh;

    foreach (sh,shieldList)
        shields.push_back(sh->getName());

    return shields;
}

int Mecha::getBinderSpace(const QString &bind) const
{
    Shield* sh;
    foreach (sh,shieldList)
    {
        if (QString(sh->getName() + " Binder") == bind)
            return sh->getBinderSpace();
    }

    return 0;
}

QList<SplittablePart*> Mecha::getSplittableParts() const
{
    QList<SplittablePart*> splitList;
    Weapon* wp;
    foreach (wp,weaponList)
    {
        if (wp->isMated())
            continue;
        if (wp->isSplit())
            splitList.push_back(wp);
    }

    Cockpit* cp;
    Sensor* sen;
    Shield* sh;

    foreach (cp,cockpitList)
    {
        if (cp->isSplit())
            splitList.push_back(cp);
    }

    foreach (sen,sensorList)
    {
        if (sen->isSplit())
            splitList.push_back(sen);
    }

    foreach (sh,shieldList)
    {
        if (sh->isSplit())
            splitList.push_back(sh);
    }

    Reflector* ref;

    foreach (ref,refList)
    {
        if (ref->isSplit())
            splitList.push_back(ref);
    }

    EnergyPool* pool;

    foreach (pool,poolList)
    {
        if (pool->isSplit())
            splitList.push_back(pool);
    }

    if (verniers->getRating())
        splitList.push_back(verniers);

    if (getBiggestECMPtr()->getSpace() && getBiggestECMPtr()->isSplit())
        splitList.push_back(getBiggestECMPtr());

    foreach (MatedSystem* ms,matedList)
    {
        splitList.push_back(ms);
    }

    return splitList;
}

double Mecha::getUnallocatedSpace() const
{
    double un = 0;
    un += getSpacesUsed("None");

    // Find unused Splits
    QList<SplittablePart*> parts = this->getSplittableParts();
    SplittablePart* p;

    foreach (p,parts)
    {
        if (!p->isMated())
            un += p->getSpace() - p->getAllocSpace();
    }

    return un;
}

double Mecha::getMultipliers() const
{
    double tot = 0;

    // Environmentals
    for (QMap<EnvTypes,bool>::const_iterator it = envMap.constBegin();it != envMap.constEnd();++it)
    {
        if (it.value())
        {
            tot += 0.05;
            if (it.key() == EM || it.key() == RE_ENTRY)
                tot += 0.05;
        }
    }

    // Weight Inefficiency
    if (wtEff < 0)
        tot += wtEff * 0.2;

    // Cockpit Controls
    tot += controls.getCostMult();

    // Verniers
    tot += verniers->getCostMult();

    tot += getHydraulicsMult();

    tot += plant->getCostMult();

    tot += getACEMult();

    // Cloaking
    tot += cloakLevel * 0.15;
    tot += (pulseRefract + magRefract + beamRefract) * 0.1;
    tot += fireControl ? 0.3 : 0;
    tot += combatCloak ? 0.2 : 0;
    tot += stealth ? 0.2 : 0;

    tot += getTFTotalCost();

    return tot;
}

void Mecha::setEnvironmental(EnvTypes env, bool has)
{
    envMap[env] = has;
}

QStringList Mecha::getEnvironmentals() const
{
    QStringList envs;

    for (QMap<EnvTypes,bool>::const_iterator it = envMap.constBegin();it != envMap.constEnd();++it)
    {
        if (it.value())
        {
            switch (it.key())
            {
            case ARCTIC:
                envs.push_back("Arctic Environmental Protection");
                break;
            case DESERT:
                envs.push_back("Desert Environmental Protection");
                break;
            case UNDERWATER:
                envs.push_back("Underwater Environmental Protection");
                break;
            case HIGH_PRESSURE:
                envs.push_back("High Pressure Environmental Protection");
                break;
            case SPACE:
                envs.push_back("Space Environmental Protection");
                break;
            case EM:
                envs.push_back("EM Environmental Protection");
                break;
            case RE_ENTRY:
                envs.push_back("Re-Entry Environmental Protection");
                break;
            }
        }
    }

    return envs;
}

QList<EnvTypes> Mecha::getEnvList() const
{
    QList<EnvTypes> lis;

    for (QMap<EnvTypes,bool>::const_iterator it = envMap.constBegin();it != envMap.constEnd();++it)
    {
        if (it.value())
        {
            lis.push_back(it.key());
        }
    }

    return lis;
}

double Mecha::getPoolMultiplier() const
{
    double mult = 0;

    if (propMap.contains("Gravitics") && propMap["Gravitics"]->getRating() > 0)
    {
        mult += 0.333;
    }

    mult += controls.getBonusMult();

    if (plant->getCharge() > 0 && plant->isHot())
    {
        mult += plant->getCharge() * 0.333;
    }

    mult += getACEBonus();

    return qRound(mult*100);
}

double Mecha::getTFTotalCost() const
{
    double mult = 0;

    Transformation* tf;
    foreach (tf,forms)
    {
        if (tf != defaultForm)
            mult += tf->getCost();
    }

    return mult;
}

void Mecha::remForm(const QString& form)
{
    if (hasForm(form))
    {
        if (defaultForm == forms.value(form))
        {
            defaultForm = 0;
        }

        delete forms.value(form);
        forms.remove(form);
    }

    if (!defaultForm)
    {
        if (forms.size() > 0)
            defaultForm = *forms.begin();
        else
        {
            Transformation* tf = new Transformation("Humanoid");
            defaultForm = tf;
            forms.insert("Humanoid",tf);
        }
    }
}

QList<SplittablePart*> Mecha::getMateableSystems()
{
    QList<SplittablePart*> result;

    foreach(Weapon* wp,weaponList)
    {
        if (!wp->isMated())
            result.push_back(wp);
    }

    return result;
}

bool Mecha::addMatedSystem(MatedSystem *newSys)
{
    foreach(MatedSystem* ms,matedList)
    {
        if (ms->getName() == newSys->getName())
        {
            return false;
        }
    }

    matedList.push_back(newSys);
    return true;
}

void Mecha::remMatedSystem(const QString &msName)
{
    foreach(MatedSystem* ms,matedList)
    {
        if (ms->getName() == msName)
        {
            matedList.removeOne(ms);
            delete ms;
        }
    }
}
