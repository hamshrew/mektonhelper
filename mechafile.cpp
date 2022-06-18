#include "mechafile.h"
#include "mecha.h"
#include "servo.h"
#include "weapon.h"
#include "fuel.h"
#include "propulsion.h"
#include "ammo.h"
#include "option.h"
#include "cockpit.h"
#include "sensor.h"
#include "recon.h"
#include "shield.h"
#include "reflector.h"
#include "ecm.h"
#include "powerplant.h"
#include "energypool.h"
#include "drone.h"
#include "transformation.h"
#include "matedsystem.h"

#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QMap>
#include <QStringList>

MechaFile::MechaFile():
    mecha(0)
{
}

bool MechaFile::parseFile(QFile *file)
{
    if (!file)
        return false;

    if (!file->isOpen())
    {
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(file))
    {
        file->close();
        return false;
    }

    // reset the Mecha
    if (mecha)
        delete mecha;

    mecha = new Mecha();

    mecha->clear();

    parseDomTree(doc);

    // Add a default form if none is available
    QString lst = mecha->getDefaultFormName();
    if (!mecha->hasForm(lst))
    {
        Transformation tf("Humanoid");
        mecha->addForm(tf);
    }

    file->close();
    return true;
}

bool MechaFile::writeFile(const QString &filename)
{
    // nothing to write!
    if (!mecha)
        return false;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QDomDocument doc;

    doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-16\""));
    QDomElement top = doc.createElement("MektonMecha");
    top.setAttribute("name",mecha->getName());
    doc.appendChild(top);

    QDomElement node;

    QList<Servo*>& servos = mecha->getServoRef();

    if (mecha->hasWheels())
    {
        // Wheel Size
        node = doc.createElement("Wheels");
        node.setAttribute("size",mecha->getWheelSize());

        // Wheel Kills
        Servo* s;
        foreach (s,servos)
        {
            if (!mecha->getTransKills("Wheels",s->getDesignation()))
                continue;
            QDomElement newNode =doc.createElement("Servo");
            newNode.setAttribute("name",s->getDesignation());
            newNode.setAttribute("kills",mecha->getTransKills("Wheels",s->getDesignation()));
            node.appendChild(newNode);
        }

        top.appendChild(node);
    }

    if (mecha->hasTreads())
    {
        // Tread Size
        node = doc.createElement("Treads");
        node.setAttribute("size",mecha->getWheelSize());

        // Tread Kills
        Servo* s;
        foreach (s,servos)
        {
            if (!mecha->getTransKills("Treads",s->getDesignation()))
                continue;
            QDomElement newNode =doc.createElement("Servo");
            newNode.setAttribute("name",s->getDesignation());
            newNode.setAttribute("kills",mecha->getTransKills("Treads",s->getDesignation()));
            node.appendChild(newNode);
        }

        top.appendChild(node);
    }

    // Servos
    Servo *s;

    node = doc.createElement("Servos");

    foreach (s,servos)
    {
        QDomElement newNode = doc.createElement("Servo");
        newNode.setAttribute("name",s->getDesignation());
        newNode.setAttribute("type",s->getType());
        newNode.setAttribute("size",s->getSize());
        newNode.setAttribute("armorsize",s->getArmorSize());
        newNode.setAttribute("armortype",s->getArmorType());
        newNode.setAttribute("ram",s->getRAM());
        newNode.setAttribute("reinforce",s->getReinforce());
        newNode.setAttribute("mounts",s->getMounts());
        node.appendChild(newNode);
    }

    top.appendChild(node);

    // Weight Efficiency

    node = doc.createElement("Weight_Eff");
    node.setAttribute("value",mecha->getWtEff());
    top.appendChild(node);

    // Weapons
    node = doc.createElement("Weapons");
    QList<Weapon*>& wList = mecha->getWeaponsRef();
    Weapon* w;
    foreach (w,wList)
    {
        QDomNode newNode = makeWeaponNode(doc,w);
        makeSplitNodes(doc,newNode,w);
        makeLinkNodes(doc,newNode,w);
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Mated Weapons
    node = doc.createElement("Mated");
    QList<MatedSystem*>& msList = mecha->getMatedPartsRef();
    foreach (MatedSystem* ms,msList)
    {
        // Stuff
        QDomElement newNode = doc.createElement("MatedWeapon");
        newNode.setAttribute("name",ms->getName());
        newNode.setAttribute("loc",ms->getLoc());
        if (ms->getPart(0))
            newNode.setAttribute("first",ms->getPart(0)->getName());
        if (ms->getPart(1))
            newNode.setAttribute("second",ms->getPart(1)->getName());
        makeSplitNodes(doc,newNode,ms);
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Ammo
    node = doc.createElement("Ammunition");
    QList<Ammo*>& aList = mecha->getAmmoRef();
    Ammo* a;
    foreach (a,aList)
    {
        QDomElement newNode = doc.createElement("Ammo");
        newNode.setAttribute("weapon",a->getWeaponName());
        newNode.setAttribute("isClip",a->isClip());
        newNode.setAttribute("damage",a->getDamage());
        newNode.setAttribute("acc",a->getAcc());
        newNode.setAttribute("shots",a->getShots());
        newNode.setAttribute("clips",a->getClips());
        newNode.setAttribute("cost",a->getCost() - a->getEff()/2.0);
        newNode.setAttribute("space",a->getBaseSpace());
        newNode.setAttribute("kills",a->getKills());
        newNode.setAttribute("eff",a->getEff());
        newNode.setAttribute("loc",a->getLoc());
        newNode.setAttribute("notes",a->getAllNotes());
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Energy Pools
    node = doc.createElement("Energy_Pools");
    QList<EnergyPool*>& poolList = mecha->getPoolRef();
    EnergyPool* ep;
    foreach (ep,poolList)
    {
        // Set up pool
        QDomElement newNode = doc.createElement("Pool");
        newNode.setAttribute("name",ep->getName());
        newNode.setAttribute("loc",ep->getLoc());
        newNode.setAttribute("eff",ep->getEff());
        newNode.setAttribute("rating",ep->getPowerAvailable()/5);
        newNode.setAttribute("portfolio",ep->getPortfolio());
        newNode.setAttribute("morphable",ep->isMorphable());

        // Set up weapons
        QList<Weapon*>& poolWeapons = ep->getWeaponsRef();
        Weapon* wp;
        foreach (wp,poolWeapons)
        {
            QDomNode nn = makeWeaponNode(doc,wp);
            newNode.appendChild(nn);
        }

        // Set up Shields
        QList<Shield*>& poolShields = ep->getShieldsRef();
        Shield* sh;
        foreach (sh,poolShields)
        {
            QDomNode nn = makeShieldNode(doc,sh);
            newNode.appendChild(nn);
        }

        makeSplitNodes(doc,newNode,ep);
        node.appendChild(newNode);
    }

    top.appendChild(node);

    // Fuel Weight
    node = doc.createElement("Fuel");
    node.setAttribute("baseweight",mecha->getBaseFuel());

    // Fuel
    QList<Fuel*>& fList = mecha->getFuelRef();
    Fuel* f;
    foreach(f,fList)
    {
        QDomElement newNode = doc.createElement("Tank");
        newNode.setAttribute("external",f->isExternal());
        newNode.setAttribute("num",f->getNumTanks());
        newNode.setAttribute("loc",f->getLoc());
        newNode.setAttribute("eff",f->getEff());
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Propulsion
    node = doc.createElement("Propulsion");
    QMap<QString,Propulsion*>& propMap = mecha->getPropMapRef();
    Propulsion* p;
    foreach (p,propMap)
    {
        if (!p)
            continue;

        QDomElement newNode = doc.createElement("System");
        newNode.setAttribute("type",p->getType());
        newNode.setAttribute("rating",p->getRating());
        newNode.setAttribute("eff",p->getEff());

        foreach (s,servos)
        {
            if (!p->getSpaces(s->getDesignation()))
                continue;

            QDomElement nn = doc.createElement("Alloc");
            nn.setAttribute("loc",s->getDesignation());
            nn.setAttribute("space",p->getSpaces(s->getDesignation()));
            newNode.appendChild(nn);
        }
        node.appendChild(newNode);
    }

    top.appendChild(node);

    // Cockpits
    node = doc.createElement("Control");
    QList<Cockpit*>& cList = mecha->getCockpitRef();
    Cockpit* cp;
    foreach (cp,cList)
    {
        QDomElement newNode = doc.createElement("Cockpit");
        newNode.setAttribute("name",cp->getName());
        newNode.setAttribute("loc",cp->getLoc());
        newNode.setAttribute("crew",cp->getCrew());
        newNode.setAttribute("pass",cp->getPassengers());
        newNode.setAttribute("enc",cp->getEnc());
        newNode.setAttribute("eff",cp->getEff());
        makeSplitNodes(doc,newNode,cp);
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Sensors
    node = doc.createElement("Sensors");
    QList<Sensor*>& sList = mecha->getSensorRef();
    Sensor* sen;
    foreach (sen,sList)
    {
        QDomElement newNode = doc.createElement("Sensor");
        newNode.setAttribute("name",sen->getName());
        newNode.setAttribute("size",sen->getClass());
        newNode.setAttribute("loc",sen->getLoc());
        newNode.setAttribute("eff",sen->getEff());
        const QMap<QString,Recon*>& rMap = sen->getReconMapCRef();
        Recon* rec;
        foreach (rec,rMap)
        {
            QDomElement nn = doc.createElement("Recon");
            nn.setAttribute("name",rec->getName());
            nn.setAttribute("hasLevels",rec->isHasLevels());
            nn.setAttribute("levels",rec->getLevels());
            nn.setAttribute("cost",rec->getCost());
            nn.setAttribute("space",rec->getSpace());
            nn.setAttribute("kills",rec->getKills());
            newNode.appendChild(nn);
        }
        makeSplitNodes(doc,newNode,sen);
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Options
    node = doc.createElement("Options");
    QList<Option*>& oList = mecha->getOptionRef();
    Option* o;
    foreach (o,oList)
    {
        QDomElement newNode = doc.createElement("System");
        newNode.setAttribute("name",o->getName());
        newNode.setAttribute("num",o->getNumber());
        newNode.setAttribute("loc",o->getLoc());
        newNode.setAttribute("notes",o->getNotes());
        newNode.setAttribute("eff",o->getEff());
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Remotes
    node = doc.createElement("Remotes");
    node.setAttribute("class",mecha->getConstRemotePtr()->getClass());
    node.setAttribute("range",mecha->getConstRemotePtr()->getControlRange());
    node.setAttribute("wired",mecha->getConstRemotePtr()->isWired());
    node.setAttribute("stored",mecha->getConstRemotePtr()->isStoring());
    node.setAttribute("loc",mecha->getConstRemotePtr()->getLoc());
    node.setAttribute("eff",mecha->getConstRemotePtr()->getEff());

    QList<Drone*>& droneList = mecha->getRemotes()->getDronesRef();
    Drone* drone;
    foreach (drone,droneList)
    {
        QDomElement newNode = doc.createElement("Drone");
        newNode.setAttribute("name",drone->getName());
        newNode.setAttribute("cost",drone->getCost());
        newNode.setAttribute("weight",drone->getWeight());
    }

    top.appendChild(node);

    // Shields
    node = doc.createElement("Shields");
    QList<Shield*>& shList = mecha->getShieldRef();
    Shield* sh;
    foreach (sh,shList)
    {
        QDomNode newNode = makeShieldNode(doc,sh);
        makeSplitNodes(doc,newNode,sh);
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Reflectors
    node = doc.createElement("Reflectors");
    QList<Reflector*>& refList = mecha->getReflectRef();
    Reflector* ref;
    foreach (ref,refList)
    {
        QDomElement newNode = doc.createElement("Reflector");
        newNode.setAttribute("name",ref->getName());
        newNode.setAttribute("loc",ref->getLoc());
        newNode.setAttribute("value",ref->getValue());
        newNode.setAttribute("eff",ref->getEff());
        makeSplitNodes(doc,newNode,ref);
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // ECM/ECCM
    node = doc.createElement("ECM");
    QMap<QString,ECM*>& ecmMap = mecha->getECMMapRef();
    ECM* ecm;
    foreach (ecm,ecmMap)
    {
        QDomElement newNode = doc.createElement("System");
        newNode.setAttribute("name",ecm->getName());
        newNode.setAttribute("loc",ecm->getLoc());
        newNode.setAttribute("value",ecm->getValue());
        newNode.setAttribute("radius",ecm->getRadius());
        newNode.setAttribute("beam",ecm->getBeaming());
        newNode.setAttribute("eff",ecm->getEff());
        makeSplitNodes(doc,newNode,ecm);
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Multipliers

    // Environmentals
    node = doc.createElement("Environmentals");
    QList<EnvTypes> envList = mecha->getEnvList();
    EnvTypes env;
    foreach (env,envList)
    {
        QDomElement newNode = doc.createElement("Prot");
        newNode.setAttribute("type",(int)env);
        node.appendChild(newNode);
    }
    top.appendChild(node);

    // Cloaking
    node = doc.createElement("Cloaking");
    node.setAttribute("level",mecha->getCloakLevel());
    node.setAttribute("combat",mecha->hasCombatCloak());
    node.setAttribute("stealth",mecha->hasStealth());
    node.setAttribute("fire",mecha->hasFireControl());
    node.setAttribute("beam",mecha->hasBeamRefract());
    node.setAttribute("magnetic",mecha->hasMagneticRefract());
    node.setAttribute("pulse",mecha->hasPulseRefract());
    top.appendChild(node);

    // Cockpit Controls
    node = doc.createElement("Cockpit_Controls");
    node.setAttribute("name",mecha->getCockpitControls().getName());
    node.setAttribute("cost",mecha->getCockpitControls().getCostMult());
    node.setAttribute("bonus",mecha->getCockpitControls().getBonus());
    node.setAttribute("mult",mecha->getCockpitControls().getBonusMult());
    top.appendChild(node);

    // Maneuver Verniers
    if (mecha->getVernierRating())
    {
        node = doc.createElement("Verniers");
        node.setAttribute("rating",mecha->getVernierRating());
        makeSplitNodes(doc,node,mecha->getVerniersPtr());
        top.appendChild(node);
    }

    // Hydraulics
    node = doc.createElement("Hydraulics");
    node.setAttribute("value",mecha->getHydraulics());
    top.appendChild(node);

    // ACE
    node = doc.createElement("ACE");
    node.setAttribute("rank",mecha->getACE());
    top.appendChild(node);

    // Power Plant
    node = doc.createElement("Power_Plant");
    node.setAttribute("charge",mecha->getPowerPlant()->getCharge());
    node.setAttribute("source",mecha->getPowerPlant()->getSource());
    node.setAttribute("hot",mecha->getPowerPlant()->isHot());
    top.appendChild(node);

    // Transformations
    node = doc.createElement("Transformations");
    node.setAttribute("default",mecha->getDefaultFormName());
    QStringList formList = mecha->getFormList();
    Transformation tf;
    QString form;
    foreach (form,formList)
    {
        tf = mecha->getForm(form);
        QDomElement newNode = doc.createElement("Form");
        newNode.setAttribute("name",tf.getName());
        newNode.setAttribute("cost",tf.getCost());
        newNode.setAttribute("special",tf.getSpecial());
        newNode.setAttribute("hardpoints",tf.getHardPoints());
        newNode.setAttribute("MV",tf.getMV());

        QStringList props = tf.getProp();
        QString prop;

        foreach(prop,props)
        {
            QDomElement nn = doc.createElement("Prop");
            nn.setAttribute("name",prop);
            newNode.appendChild(nn);
        }

        props = tf.getMoveList();
        foreach (prop,props)
        {
            QDomElement nn = doc.createElement("Move");
            nn.setAttribute("name",prop);
            nn.setAttribute("MABonus",tf.getMABonus(prop));
            nn.setAttribute("MAMult",tf.getMAMult(prop));
            newNode.appendChild(nn);
        }

        node.appendChild(newNode);
    }
    top.appendChild(node);

    QTextStream stream(&file);
    stream.setCodec("UTF-16");
    stream << doc.toString();

    file.close();

    return true;
}

QDomNode MechaFile::makeWeaponNode(QDomDocument &doc, Weapon *wp)
{
    QDomElement node = doc.createElement("Weapon");
    node.setAttribute("name",wp->getName());
    node.setAttribute("loc",wp->getLoc());
    node.setAttribute("acc",wp->getAcc());
    node.setAttribute("range",wp->getRange());
    node.setAttribute("damage",wp->getDamage());
    node.setAttribute("shots",wp->getShots());
    node.setAttribute("shotPrep",wp->getShotPrepend());
    node.setAttribute("kills",wp->getKills());
    node.setAttribute("cost",wp->getBaseCost());
    node.setAttribute("space",wp->getSpace() + wp->getEff());
    node.setAttribute("eff",wp->getEff());
    node.setAttribute("melee",wp->isMelee());
    node.setAttribute("clumsy",wp->isClumsy());
    node.setAttribute("handy",wp->isHandy());
    node.setAttribute("useAmmo",wp->isUseAmmo());
    node.setAttribute("useClips",wp->isUseClips());
    node.setAttribute("type",wp->getType());
    node.setAttribute("notes",wp->getAllNotes());

    QDomText text = doc.createTextNode(wp->getComments());
    node.appendChild(text);

    return node;
}

QDomNode MechaFile::makeShieldNode(QDomDocument &doc, Shield* sh)
{
    QDomElement node = doc.createElement("Shield");
    node.setAttribute("name",sh->getName());
    node.setAttribute("loc",sh->getLoc());
    node.setAttribute("type",(int)sh->getType());
    node.setAttribute("size",(int)sh->getSize());
    node.setAttribute("armor",(int)sh->getArmor());
    node.setAttribute("binder",sh->getBinderMul());
    node.setAttribute("opt",sh->getOptMul());
    node.setAttribute("notes",sh->getAllNotes());
    node.setAttribute("eff",sh->getEff());
    return node;
}

bool MechaFile::setMecha(Mecha *mecha)
{
    if (!mecha)
        return false;

    if (this->mecha)
        delete this->mecha;

    this->mecha = mecha;

    return true;
}

void MechaFile::parseDomTree(const QDomDocument &doc)
{
    QDomElement top = doc.documentElement();

    mecha->setName(top.attribute("name","None"));

    QDomNode node = top.firstChild();

    QDomNode wheelNode;
    QDomNode treadNode;
    QDomNode propNode;
    while (!node.isNull())
    {
        if (node.isElement())
        {
            QString nodeName(node.toElement().nodeName());

            if (nodeName == "Wheels")
                wheelNode = node;
            else if (nodeName == "Treads")
                treadNode = node;
            else if (nodeName == "Servos")
                addServos(node);
            else if (nodeName == "Weapons")
                addWeapons(node);
            else if (nodeName == "Mated")
                addMated(node);
            else if (nodeName == "Ammunition")
                addAmmo(node);
            else if (nodeName == "Fuel")
                addFuel(node);
            else if (nodeName == "Propulsion")
                propNode = node;
            else if (nodeName == "Control")
                addControl(node);
            else if (nodeName == "Sensors")
                addSensors(node);
            else if (nodeName == "Options")
                addOptions(node);
            else if (nodeName == "Shields")
                addShields(node);
            else if (nodeName == "Reflectors")
                addReflectors(node);
            else if (nodeName == "Environmentals")
                addEnvironmentals(node);
            else if (nodeName == "Weight_Eff")
                mecha->setWeightEff(node.toElement().attribute("value","0").toInt());
            else if (nodeName == "Cockpit_Controls")
                addCockpitControls(node);
            else if (nodeName == "Hydraulics")
                mecha->setHydraulics(node.toElement().attribute("value","0").toInt());
            else if (nodeName == "ACE")
                mecha->setACE(node.toElement().attribute("rank","0").toInt());
            else if (nodeName == "ECM")
                addECM(node);
            else if (nodeName == "Verniers")
            {
                mecha->setManeuverVerniers(node.toElement().attribute("rating","0").toInt());
                addSplits(node,mecha->getVerniersPtr());
            }
            else if (nodeName == "Power_Plant")
                addPlant(node);
            else if (nodeName == "Remotes")
                addRemotes(node);
            else if (nodeName == "Energy_Pools")
                addPools(node);
            else if (nodeName == "Cloaking")
                addCloaking(node);
            else if (nodeName == "Transformations")
            {
                addForms(node);
                mecha->setDefaultForm(node.toElement().attribute("default","Humanoid"));
            }
        }

        node = node.nextSibling();
    }

    addWheels(wheelNode);
    addTreads(treadNode);
    addPropulsion(propNode);
}

void MechaFile::makeSplitNodes(QDomDocument &doc, QDomNode &node, SplittablePart *part)
{
    if (!part->isSplit())
        return;

    QStringList spots = part->getSplitLocs();
    QString loc;

    QDomElement newNode = doc.createElement("Split");

    foreach (loc,spots)
    {
        if (!part->getAllocSpace(loc))
            continue;

        QDomElement nn = doc.createElement("Alloc");
        nn.setAttribute("loc",loc);
        nn.setAttribute("space",part->getAllocSpace(loc));
        newNode.appendChild(nn);
    }
    node.appendChild(newNode);
}

void MechaFile::makeLinkNodes(QDomDocument &doc, QDomNode &node, Weapon *wp)
{
    if (!wp->getLinkCost())
        return;

    QPair<Weapon*,bool> pr;

    const QList<QPair<Weapon*,bool> >& lst = wp->getLinkList();

    QDomElement newNode = doc.createElement("Links");

    foreach(pr,lst)
    {
        QDomElement nn = doc.createElement("Linkage");
        nn.setAttribute("name",pr.first->getName());
        nn.setAttribute("cross",(int)pr.second);
        newNode.appendChild(nn);
    }
    node.appendChild(newNode);
}

void MechaFile::addWheels(const QDomNode& node)
{
    mecha->setWheelSize(node.toElement().attribute("size","0").toInt());

    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Servo"){
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();
            mecha->setTransKills("Wheels",el.attribute("name"),el.attribute("kills").toInt());
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addTreads(const QDomNode& node)
{
    mecha->setTreadSize(node.toElement().attribute("size","0").toInt());

    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Servo")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();
            mecha->setTransKills("Treads",el.attribute("name"),el.attribute("kills").toInt());
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addServos(const QDomNode& node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Servo")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Servo* s = new Servo();

            s->setDesignation(el.attribute("name","Bad Data"));
            s->setType((ServoTypes)el.attribute("type","0").toInt());
            s->setSize((ServoSizes)el.attribute("size","0").toInt());
            s->setArmorSize((ServoSizes)el.attribute("armorsize","0").toInt());
            s->setArmorType((ArmorTypes)el.attribute("armortype","0").toInt());
            s->setRAM((RAMTypes)el.attribute("ram","0").toInt());
            s->setReinforce(el.attribute("reinforce","0").toInt());
            s->setMounts(el.attribute("mounts","0").toInt());

            mecha->addServo(s);
        }

        nn = nn.nextSibling();
    }
}

Weapon* MechaFile::parseWeapon(const QDomElement &el)
{
    Weapon* wp = new Weapon();
    wp->setName(el.attribute("name","Bad Data"));
    wp->setLoc(el.attribute("loc","None"));
    wp->setAcc(el.attribute("acc","0").toInt());
    wp->setDamage(el.attribute("damage","1").toUInt());
    wp->setRange(el.attribute("range","1").toUInt());
    wp->setKills(el.attribute("kills","1").toUInt());
    wp->setShotPrepend(el.attribute("shotPrepend",""));
    wp->setShots(el.attribute("shots","Bad"));
    wp->setCost(el.attribute("cost","0.0").toDouble());
    wp->setType(el.attribute("type",""));
    wp->setSpace(el.attribute("space","0.0").toDouble());
    wp->setEff(el.attribute("eff","0.0").toDouble());
    wp->setUseAmmo(el.attribute("useAmmo","0").toInt());
    wp->setUseClips(el.attribute("useClips","0").toInt());
    wp->setClumsy(el.attribute("clumsy","0").toInt());
    wp->setMelee(el.attribute("melee","0").toInt());
    wp->setHandy(el.attribute("handy","0").toInt());

    wp->addNote(el.attribute("notes","Bad Data"));

    addSplits(el,wp);

    return wp;
}

void MechaFile::addWeapons(const QDomNode& node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            QDomElement el = nn.toElement();

            if (nn.nodeName() != "Weapon"){
                nn = nn.nextSibling();
                continue;
            }
            Weapon* wp = parseWeapon(el);

            // Check for text
            QDomNode newNode = nn.firstChild();

            while (!newNode.isNull())
            {
                if (newNode.isText())
                    wp->setComments(newNode.toText().data());

                newNode = newNode.nextSibling();
            }

            mecha->addWeapon(wp);
        }

        nn = nn.nextSibling();
    }

    // Now go through AGAIN to add Linkage. Sigh.
    nn = node.firstChild();

    int wpNum = 0;

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            QDomElement el = nn.toElement();

            if (nn.nodeName() != "Weapon"){
                nn = nn.nextSibling();
                continue;
            }
            Weapon* wp = mecha->getWeaponsRef()[wpNum];

            addLinks(nn,wp);

            ++wpNum;
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addMated(const QDomNode &node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "MatedWeapon")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            MatedSystem* ms = new MatedSystem();

            ms->setName(el.attribute("name","Bad Data"));
            ms->setLoc(el.attribute("loc","None"));
            QString first = el.attribute("first","None");
            QString second = el.attribute("second","None");

            // Get weapon pointers
            QList<Weapon*>& wpList = mecha->getWeaponsRef();
            foreach (Weapon* wp,wpList)
            {
                if (wp->getName() == first)
                {
                    ms->addPart(wp);
                    break;
                }
            }

            foreach (Weapon* wp,wpList)
            {
                if (wp->getName() == second)
                {
                    ms->addPart(wp);
                    break;
                }
            }


            addSplits(nn,ms);

            mecha->addMatedSystem(ms);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addPools(const QDomNode &node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Pool")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            EnergyPool* ep = new EnergyPool();

            ep->setName(el.attribute("name","Bad Data"));
            ep->setLoc(el.attribute("loc","None"));
            ep->setRating(el.attribute("rating","0").toInt());
            ep->setPortfolio(el.attribute("portfolio","3").toInt());
            ep->setMorphable(el.attribute("morphable","0").toInt());
            ep->setEff(el.attribute("eff","0").toDouble());

            QDomNode newNode = nn.firstChild();

            while (!newNode.isNull())
            {
                if (newNode.isElement())
                {
                    if (newNode.nodeName() == "Weapon")
                    {
                        Weapon* wp = parseWeapon(newNode.toElement());
                        ep->addWeapon(wp);
                    }
                    else if (newNode.nodeName() == "Shield")
                    {
                        Shield* sh = parseShield(newNode.toElement());
                        ep->addShield(sh);
                    }
                }
                newNode = newNode.nextSibling();
            }

            addSplits(nn,ep);

            mecha->addPool(ep);;
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addControl(const QDomNode& node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Cockpit")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Cockpit* cp = new Cockpit();

            cp->setName(el.attribute("name","Main"));
            cp->setLoc(el.attribute("loc","None"));
            cp->setCrew(el.attribute("crew","1").toInt());
            cp->setPassengers(el.attribute("pass","0").toInt());
            cp->setEnc(el.attribute("enc","Armored"));
            cp->setEff(el.attribute("eff","0").toDouble());

            addSplits(nn,cp);

            mecha->addCockpit(cp);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addForms(const QDomNode &node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Form")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Transformation tf;

            tf.setName(el.attribute("name","Bad Data"));
            tf.setCost(el.attribute("cost","0").toDouble());
            tf.setSpecial(el.attribute("special","Bad Data"));
            tf.setHardpoints(el.attribute("hardpoints","Bad Data"));
            tf.setMVBonus(el.attribute("MV","0").toInt());

            // Get the propulsion modifiers
            QDomNode newNode = nn.firstChild();

            while (!newNode.isNull())
            {
                if (newNode.isElement())
                {
                    if (newNode.nodeName() != "Prop" && newNode.nodeName() != "Move")
                    {
                        newNode = newNode.nextSibling();
                        continue;
                    }

                    QDomElement elem = newNode.toElement();

                    QString propName = elem.attribute("name","Bad Data");

                    if (newNode.nodeName() == "Prop")
                        tf.addProp(propName);
                    else
                    {
                        tf.addMABonus(propName,elem.attribute("MABonus","0").toInt());
                        tf.addMAMult(propName,elem.attribute("MAMult","0").toDouble());
                    }
                }

                newNode = newNode.nextSibling();
            }

            mecha->addForm(tf);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addSensors(const QDomNode& node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Sensor")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Sensor* s = new Sensor();

            s->setName(el.attribute("name","Main"));
            s->setLoc(el.attribute("loc","None"));
            s->setClass(el.attribute("size","Superlight"));

            QDomNode newNode = nn.firstChild();

            while (!newNode.isNull())
            {
                if (newNode.nodeName() != "Recon")
                {
                    newNode = newNode.nextSibling();
                    continue;
                }

                QDomElement el = newNode.toElement();

                Recon* r = new Recon();

                r->setName(el.attribute("name","Bad Data"));
                r->setHasLevels(el.attribute("hasLevels","0").toInt());
                r->setLevels(el.attribute("levels","1").toInt());
                r->setBaseCost(el.attribute("cost","1").toDouble() / r->getLevels());
                r->setSpace(el.attribute("space","0").toInt());
                r->setKills(el.attribute("kills","0").toInt());

                s->addRecon(r);

                newNode = newNode.nextSibling();
            }

            s->setEff(el.attribute("eff","0").toDouble());

            addSplits(nn,s);

            mecha->addSensor(s);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addAmmo(const QDomNode& node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Ammo")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Ammo* a = new Ammo();

            a->setWeaponName(el.attribute("weapon","Bad Data"));
            a->setIsClip(el.attribute("isClip","0").toInt());
            a->setDamage(el.attribute("damage","1").toInt());
            a->setAcc(el.attribute("acc","0").toInt());
            a->setShots(el.attribute("shots","1").toInt());
            a->setClips(el.attribute("clips","1").toInt());
            a->setCost(el.attribute("cost","0").toFloat());
            a->setSpace(el.attribute("space","0").toFloat());
            a->setKills(el.attribute("kills","0").toInt());
            a->setEff(el.attribute("eff","0").toDouble());
            a->setLoc(el.attribute("loc","None"));
            QStringList noteList;
            noteList.push_back(el.attribute("notes",""));
            a->setNotes(noteList);

            mecha->addAmmo(a);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addFuel(const QDomNode& node)
{
    mecha->setBaseFuel(node.toElement().attribute("baseweight","0").toInt());

    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Tank")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Fuel* f = new Fuel();

            f->setExternal(el.attribute("external","0").toInt());
            f->setNumTanks(el.attribute("num","1").toInt());
            f->setLoc(el.attribute("loc","None"));
            f->setEff(el.attribute("eff","0").toDouble());

            mecha->addFuel(f);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addPropulsion(const QDomNode& node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "System")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Propulsion* p = new Propulsion(el.attribute("type","Thrusters"));

            p->setRating(el.attribute("rating","0").toInt());

            QDomNode newNode = nn.firstChild();

            while (!newNode.isNull())
            {
                if (newNode.nodeName() != "Alloc")
                {
                    newNode = newNode.nextSibling();
                    continue;
                }

                QDomElement el = newNode.toElement();

                p->setSpaces(el.attribute("loc","None"),el.attribute("space","0").toDouble());

                newNode = newNode.nextSibling();
            }

            p->setEffForced(el.attribute("eff","0").toDouble());

            mecha->getPropMapRef()[p->getType()] = p;
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addOptions(const QDomNode& node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "System")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Option* o = new Option();

            o->setName(el.attribute("name","Bad Data"));
            o->setNotes(el.attribute("notes",""));
            o->setNumber(el.attribute("num","1").toInt());
            o->setLoc(el.attribute("loc","None"));
            o->setEff(el.attribute("eff","0").toDouble());

            mecha->addOption(o);
        }

        nn = nn.nextSibling();
    }
}

Shield* MechaFile::parseShield(const QDomElement &el)
{
    Shield* sh = new Shield();
    sh->setName(el.attribute("name","Bad Data"));
    sh->addNote(el.attribute("notes",""));
    sh->setSize((ServoSizes)el.attribute("size","0").toInt());
    sh->setType((ShieldTypes)el.attribute("type","0").toInt());
    sh->setBinderMul(el.attribute("binder","0").toDouble());
    sh->setLoc(el.attribute("loc","None"));
    sh->setOptMul(el.attribute("opt","1").toDouble());
    sh->setArmor((ArmorTypes)el.attribute("armor","1").toInt());
    sh->setEff(el.attribute("eff","0").toDouble());

    addSplits(el,sh);

    return sh;
}

void MechaFile::addShields(const QDomNode &node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Shield")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Shield* sh = parseShield(el);

            mecha->addShield(sh);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addReflectors(const QDomNode &node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Reflector")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Reflector* ref = new Reflector();

            ref->setName(el.attribute("name","Bad Data"));
            ref->setLoc(el.attribute("loc","None"));
            ref->setValue(el.attribute("value","1").toUInt());
            ref->setEff(el.attribute("eff","0").toDouble());

            addSplits(nn,ref);

            mecha->addReflector(ref);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addECM(const QDomNode &node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "System")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            QString name = el.attribute("name","Bad Data");

            if (name == "Bad Data" || mecha->getECMPtr(name) == 0)
            {
                nn = nn.nextSibling();
                continue;
            }

            mecha->getECMPtr(name)->setLoc(el.attribute("loc","None"));
            mecha->getECMPtr(name)->setValue(el.attribute("value","0").toInt());
            mecha->getECMPtr(name)->setRadius(el.attribute("radius","0").toInt());
            mecha->getECMPtr(name)->setBeaming(el.attribute("beam","0").toInt());
            mecha->getECMPtr(name)->setEff(el.attribute("eff","0").toDouble());

            addSplits(nn,mecha->getECMPtr(name));
        }

        nn = nn.nextSibling();
    }

    // Clean bad entries
    QMap<QString,ECM*>& ecmMap = mecha->getECMMapRef();
    QMap<QString,ECM*>::iterator it = ecmMap.begin();
    while (it != ecmMap.end())
    {
        if (it.value() == 0)
        {
            it = ecmMap.erase(it);
            continue;
        }
        ++it;
    }
}

void MechaFile::addEnvironmentals(const QDomNode &node)
{
    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Prot")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            mecha->setEnvironmental((EnvTypes)el.attribute("type","0").toInt(),true);
        }

        nn = nn.nextSibling();
    }
}

void MechaFile::addCloaking(const QDomNode &node)
{
    QDomElement el = node.toElement();
    mecha->setCloakLevel(el.attribute("level","0").toInt());
    mecha->setCombatCloak(el.attribute("combat","0").toInt());
    mecha->setFireControl(el.attribute("fire","0").toInt());
    mecha->setStealth(el.attribute("stealth","0").toInt());
    mecha->setBeamRefract(el.attribute("beam","0").toInt());
    mecha->setMagneticRefract(el.attribute("magnetic","0").toInt());
    mecha->setPulseRefract(el.attribute("pulse","0").toInt());
}

void MechaFile::addCockpitControls(const QDomNode &node)
{
    CockpitControls cont;

    QDomElement el = node.toElement();

    cont.setName(el.attribute("name","Screen"));
    cont.setBonus(el.attribute("bonus","0").toInt());
    cont.setBonusMult(el.attribute("mult","0").toDouble());
    cont.setCostMult(el.attribute("cost","0").toDouble());

    mecha->setCockpitControls(cont);
}

void MechaFile::addPlant(const QDomNode &node)
{
    PowerPlant* pp = new PowerPlant();

    QDomElement el = node.toElement();

    pp->setCharge(el.attribute("charge","0").toInt());
    pp->setSource((PlantSources)el.attribute("source","0").toUInt());
    pp->setHot(el.attribute("hot","0").toInt());

    mecha->setPowerPlant(pp);
}

void MechaFile::addRemotes(const QDomNode &node)
{
    Remotes* rem = new Remotes();
    QDomElement el = node.toElement();

    rem->setClass(el.attribute("class","0").toUInt());
    rem->setLoc(el.attribute("loc","None"));
    rem->setRange(el.attribute("range","0").toUInt());
    rem->setWired(el.attribute("wired","0").toInt());
    rem->setStoresRemotes(el.attribute("stored","0").toInt());
    rem->setEff(el.attribute("eff","0").toDouble());

    QDomNode nn = node.firstChild();

    while (!nn.isNull())
    {
        if (nn.isElement())
        {
            if (nn.nodeName() != "Drone")
            {
                nn = nn.nextSibling();
                continue;
            }

            QDomElement el = nn.toElement();

            Drone* drone = new Drone();

            drone->setName(el.attribute("name","Bad Data"));
            drone->setCost(el.attribute("cost","0").toDouble());
            drone->setWeight(el.attribute("weight","0").toDouble());

            rem->addDrone(drone);
        }

        nn = nn.nextSibling();
    }

    mecha->setRemotes(rem);
}

void MechaFile::addSplits(const QDomNode &node, SplittablePart *part)
{
    if (!part->isSplit())
        return;

    QDomNode newNode = node.firstChild();

    // Advance to Split
    while (!newNode.isNull())
    {
        if (newNode.isElement() && newNode.nodeName() == "Split")
            break;

        newNode = newNode.nextSibling();
    }

    // Check if it was found
    if (newNode.nodeName() != "Split")
        return;

    QDomNode nn = newNode.firstChild();

    while (!nn.isNull())
    {
        if (nn.nodeName() != "Alloc")
        {
            nn = nn.nextSibling();
            continue;
        }

        QDomElement el = nn.toElement();
        part->setAlloc(el.attribute("loc","None"),el.attribute("space","0").toDouble());

        nn = nn.nextSibling();
    }
}

void MechaFile::addLinks(const QDomNode &node, Weapon *wp)
{
    QDomNode newNode = node.firstChild();

    // Advance to Split
    while (!newNode.isNull())
    {
        if (newNode.isElement() && newNode.nodeName() == "Links")
            break;

        newNode = newNode.nextSibling();
    }

    // Check if it was found
    if (newNode.nodeName() != "Links")
        return;

    QDomNode nn = newNode.firstChild();

    while (!nn.isNull())
    {
        if (nn.nodeName() != "Linkage")
        {
            nn = nn.nextSibling();
            continue;
        }

        QDomElement el = nn.toElement();

        QString name = el.attribute("name","");

        // Find weapon
        Weapon* lnwp = 0;

        for (QList<Weapon*>::iterator it = mecha->getWeaponsRef().begin();
                it != mecha->getWeaponsRef().end();++it)
        {
            if ((*it)->getName() == name)
            {
                lnwp = *it;
                break;
            }
        }

        if (!lnwp)
        {
            nn = nn.nextSibling();
            continue;
        }

        wp->addLink(lnwp,el.attribute("cross","0").toInt());

        nn = nn.nextSibling();
    }
}
