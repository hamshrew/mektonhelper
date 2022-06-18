#include "htmlconverter.h"
#include "mecha.h"
#include "cockpit.h"
#include "option.h"
#include "transformation.h"
#include "weapon.h"
#include "matedsystem.h"
#include "energypool.h"
#include "servo.h"
#include "sensor.h"
#include "ammo.h"
#include "shield.h"
#include "reflector.h"
#include "remotes.h"
#include "drone.h"
#include "ecm.h"
#include "propulsion.h"
#include "fuel.h"

bool HtmlConverter::htmlize(Mecha *mecha, QString& html)
{
    // header
    html.append("<!DOCTYPE html>\n<html>\n<body>\n");

    // Styles
    html.append("<style>\n");
    html.append("* {font-size:10px;}");
    html.append(".invertCol {\n background-color:black;\n color: white;\n}\n");
    html.append(".halfpage {\n width:50%\n}");
    html.append("table {padding:1px;border-collapse:collapse;border-width:1px;border-style:solid;}");

    html.append("</style>\n");
    // Finished styles

    // Profile
    html.append("<div><table style=\"float:left\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"3\" class=\"invertCol\">");
    html.append("MECHA PROFILE</th>\n  </tr>\n  <tr>\n    <td width=\"100%\" rowspan=\"2\"><b>Name</b><br>");
    html.append(mecha->getName() + "</td>\n    <td><b>Weight\n</b></td>\n    ");
    html.append("<td><b>Cost</b>\n</td>\n  <tr>\n    <td>" + QString::number(mecha->getWeight()));
    html.append("</td>\n    <td>"+ QString::number(mecha->getCost()) + "</td>\n");
    html.append("  </tr>\n</table>\n");

    // Subassemblies
    html.append("<table style=\"float:right\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"4\" class=\"invertCol\">");
    html.append("SUBASSEMBLIES</th>\n  </tr>\n  <tr>\n    <td><b>Cockpit</b>\n");
    html.append("</td>\n    <td><b>#Crew\n</b></td>\n    ");
    html.append("<td><b>Loc</b>\n</td>\n    <td><b>Enc</b></td>\n  </tr>");

    {
    QList<Cockpit*> cockpitList = mecha->getCockpitRef();

    foreach (Cockpit* cp,cockpitList)
    {
        html.append("<tr><td>" + cp->getName() + "</td><td>" + QString::number(cp->getCrew())
                    + "c/" + QString::number(cp->getPassengers()) + "p</td>");
        html.append("<td>");
        if (cp->getLoc() == "Split")
        {
            QStringList locs = cp->getSplitLocs();
            QStringList::iterator it = locs.begin();
            while (it != locs.end())
            {
                html.append(QString::number(cp->getAllocSpace(*it)) + " " + *it);
                ++it;
                if (it != locs.end())
                    html.append("/");
            }
        }
        else html.append(cp->getLoc());

        html.append("</td><td>" + cp->getEnc() + "</td></tr>");
    }
    }

    html.append("<tr><td colspan=\"4\"><b>Control Scheme:</b> " + mecha->getCockpitControls().getName() + "</td></tr>");
    html.append("<tr><td colspan=\"2\"><b>Subassemblies</b></td><td><b>Loc</b></td><td><b>#</b></td></tr>");

    {
    QList<Option*> optList = mecha->getOptionRef();

    foreach (Option* opt,optList)
    {
        html.append("<tr><td colspan=\"2\">");
        if (opt->getNumber() > 1)
            html.append(QString::number(opt->getNumber()) + "x ");

        html.append(opt->getName());

        if (!opt->getNotes().isEmpty())
            html.append(" (" + opt->getNotes() + ")");

        html.append("</td><td>");
        html.append(opt->getLoc() + "</td><td>" + QString::number(opt->getNumber()) + "</td></tr>");
    }

    }

    html.append("\n</table>\n");

    // Configurations
    html.append("<table style=\"float:left\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"3\" class=\"invertCol\">");
    html.append("CONFIGURATIONS</th>\n  </tr>\n  <tr>\n    <td><b>Configuration</b>");
    html.append("</td>\n    <td><b>MV</b></td>\n    ");
    html.append("<td><b>Propulsion MA</b></td>\n");

    {
    QStringList formList = mecha->getFormList();

    foreach (QString form,formList)
    {
        const Transformation& tf = mecha->getForm(form);

        html.append("<tr><td>" + tf.getName() + "</td>");
        html.append("<td>" + QString::number(mecha->getMV() + tf.getMV()) + "</td>");
        html.append("<td>");

        QList<QPair<QString,double> > moveList = mecha->getMoveList();

        QList<QPair<QString,double> >::iterator it = moveList.begin();

        while (it != moveList.end())
        {
            double newMove = it->second + tf.getMABonus(it->first);

            if (tf.getMAMult(it->first))
                newMove *= tf.getMAMult(it->first);

            html.append(it->first + " " + QString::number(static_cast<int>(newMove)));
            ++it;
            if (it != moveList.end())
                html.append("/");
        }

        html.append("</td></tr>");
    }
    }

    html.append("  </tr>\n</table>\n");

    // Add shields
    if (!mecha->getShieldRef().isEmpty())
    {

        html.append("<table style=\"float:right\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"7\" class=\"invertCol\">");
        html.append("SHIELDING</th>\n  </tr>");
        html.append("<tr><td><b>Name</b></td><td><b>Type</b></td><td><b>SP</b></td><td><b>DC</b></td>");
        html.append("<td><b>Binder</b></td><td><b>Loc</b></td><td><b>Notes</b></td></tr>");

        QList<Shield*>& shieldList = mecha->getShieldRef();

        foreach (Shield* shield,shieldList)
        {
            html.append("<tr>");
            html.append("<td>" + shield->getName() + "</td>");
            html.append("<td>" + shield->getTypeString() + "</td>");
            html.append("<td>" + QString::number(shield->getSP()) + "</td>");
            html.append("<td>" + QString::number(shield->getDC()) + "</td>");
            html.append("<td>" + QString::number(shield->getBinderSpace()) + "</td>");
            html.append("<td>" + htmlLocs(shield) + "</td>");
            html.append("<td>" + shield->getAllNotes() + "</td>");

            html.append("</tr>");
        }

        html.append("</table>\n");

    }

    if (!mecha->getReflectRef().isEmpty())
    {

        html.append("<table style=\"float:right\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"3\" class=\"invertCol\">");
        html.append("REFLECTORS</th>\n  </tr>");
        html.append("<tr><td><b>Name</b></td><td><b>Quality</b></td>");
        html.append("<td><b>Loc</b></td></tr>");

        QList<Reflector*>& refList = mecha->getReflectRef();

        foreach (Reflector* ref,refList)
        {
            html.append("<tr>");
            html.append("<td>" + ref->getName() + "</td>");
            html.append("<td>" + QString::number(ref->getValue()) + "</td>");
            html.append("<td>" + htmlLocs(ref) + "</td>");

            html.append("</tr>");
        }

        html.append("</table>\n");

    }

    // Sensors
    html.append("<table style=\"float:right\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"6\" class=\"invertCol\">");
    html.append("SENSORS</th>\n  </tr>\n  <tr>\n    <td><b>Name</b>\n");
    html.append("</td>\n    <td><b>Loc</b></td>\n    ");
    html.append("<td><b>Range</b>\n</td>\n    <td><b>Comms</b></td>\n    <td><b>Kills</b></td><td><b>Systems</b></td>");
    html.append("  </tr>");

    QList<Sensor*>& senList = mecha->getSensorRef();

    foreach (Sensor* sen,senList)
    {
        html.append("<tr>");

        html.append("<td>" + sen->getName() + "</td>");
        html.append("<td>" + htmlLocs(sen) + "</td>");
        html.append("<td>" + sen->getSenseRange() + "</td>");
        html.append("<td>" + sen->getCommRange() + "</td>");
        html.append("<td>" + QString::number(sen->getKills()) + "K </td>");
        html.append("<td>" + sen->getReconList().join(", ") + "</td>");

        html.append("</tr>");
    }

    html.append("</table>");

    // ECM
    if (mecha->getBiggestECMPtr()->getSpace())
    {
        html.append("<table style=\"float:right\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"4\" class=\"invertCol\">");
        html.append("ELECTRONIC WARFARE</th>\n  </tr>\n  <tr>\n    <td>Location:\n");
        html.append("</td>\n    <td colspan=\"3\">" + htmlLocs(mecha->getBiggestECMPtr()) + "</td></tr>");
        html.append("<tr><td><b>Type</b>\n</td>\n    <td><b>Rating</b></td>\n    <td><b>Radius</b></td><td><b>Beam</b></td>");
        html.append("  </tr>");

        QMap<QString,ECM*>& ecmMap = mecha->getECMMapRef();

        foreach (QString ecm,ecmMap.keys())
        {
            if (!ecmMap.value(ecm)->getValue())
                continue;

            html.append("<tr>");

            html.append("<td>" + ecm + "</td>");
            html.append("<td>" + QString::number(ecmMap.value(ecm)->getValue()) + "</td>");
            html.append("<td>" + QString::number(ecmMap.value(ecm)->getRadius()) + "</td>");
            html.append("<td>" + QString::number(ecmMap.value(ecm)->getBeaming()) + "</td>");

            html.append("</tr>");
        }

        html.append("</table>");
    }


    // Servos
    html.append("<table style=\"float:left\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"6\" class=\"invertCol\">");
    html.append("SERVOS & ARMOR</th>\n  </tr>\n  <tr>\n    <td><b>Servo</b>\n");
    html.append("</td>\n    <td><b>Space\n</b></td>\n    ");
    html.append("<td><b>DC</b>\n</td>\n    <td><b>AbCo</b></td>\n    <td><b>Kills</b></td><td><b>Add+</b></td>");
    html.append("  </tr>");
    {
    QList<Servo*> serList = mecha->getServoRef();

    foreach (Servo* ser,serList)
    {
        html.append("<tr>");

        html.append("<td>" + ser->getDesignation() + "</td>");
        html.append("<td>" + QString::number(ser->getSpace()) + "</td>");
        html.append("<td>" + QString::number(ser->getArmorProt()) + "</td>");
        html.append("<td>" + ser->getRAMString() + "</td>");
        html.append("<td>" + QString::number(ser->getKills()) + "</td>");
        html.append("<td>+" + QString::number(ser->getDamageBonus()) + "K</td>");

        html.append("</tr>");
    }
    }
    html.append("</table>");

    // Remotes
    if (mecha->getRemotes()->getClass())
    {
        html.append("<table style=\"float:right\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"5\" class=\"invertCol\">");
        html.append("REMOTE INFORMATION</th>\n  </tr>\n  <tr>\n    <td><b>CM:</b> ");
        if (mecha->getRemotes()->getClass() == 1)
            html.append("0.5");
        else html.append(QString::number(mecha->getRemotes()->getClass() - 1));
        html.append("</td>\n    <td><b>Control:\n</b> " + QString::number(mecha->getRemotes()->getControlRange()) + "</td>\n");
        html.append("<td><b>Operation: </b>" + QString::number(mecha->getRemotes()->getOperationRange()) + "</td>");
        html.append("<td><b>Loc: </b>" + mecha->getRemotes()->getLoc() + "</td>");
        html.append("<td>" + (mecha->getRemotes()->isWired() ? QString("Wired") : QString()) + "</td>");
        html.append("  </tr>");
        html.append("<tr><td colspan=\"3\"><b>Drone Name</b></td><td colspan=\"2\"><b>Weight</b></td></tr>");

        QList<Drone*>& remList = mecha->getRemotes()->getDronesRef();

        foreach (Drone* rem,remList)
        {
            html.append("<tr>");

            html.append("<td colspan=\"3\">" + rem->getName() + "</td>");
            html.append("<td>");
            if (mecha->getRemotes()->isStoring())
                html.append(QString::number(rem->getWeight()) + " tons</td>");
            else html.append("None</td>");

            html.append("</tr>");
        }

        html.append("</table>");
    }

    // Powerplant and Options
    html.append("<table style=\"float:left\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th class=\"invertCol\">");
    html.append("POWER PLANT</th><th>");
    html.append(mecha->getPowerPlant()->getSourceName() + (mecha->getPowerPlant()->isHot() ? "(Hot)" : "") + "</th></tr>");
    html.append("<tr><td class=\"invertCol\">XS</td><td>" + QString(mecha->getPowerPlant()->isHot() ? "5" : "1") + "</td></tr>");
    html.append("<tr><td class=\"invertCol\">Charge</td><td>" + mecha->getPowerPlant()->getChargeName() + "</td></tr>");
    if (mecha->getBMod())
        html.append("<tr><td class=\"invertCol\">Balance Mod</td><td>" + QString::number(mecha->getBMod()) + "</td></tr>");

    QString hydraulics;

    switch (mecha->getHydraulics())
    {
    case -2:
        hydraulics = "Space";
        break;
    case -1:
        hydraulics = "Marine";
        break;
    case 0:
        hydraulics = "Standard";
        break;
    case 1:
        hydraulics = "Heavy";
        break;
    case 2:
        hydraulics = "Superheavy";
        break;
    default:
        hydraulics = "Unknown";
        break;
    }

    html.append("<tr><td class=\"invertCol\">Hydraulics</td><td>" + hydraulics + "</td></tr>");
    html.append("<tr><th colspan=\"2\" class=\"invertCol\">OPTIONS</th></tr>");

    QStringList envList = mecha->getEnvironmentals();

    int counter = 0;

    foreach (QString env,envList)
    {
        if (!(counter % 2))
            html.append("<tr>");

        html.append("<td>" + env + "</td>");

        if (counter % 2)
            html.append("</tr>");

        ++counter;
    }

    if (mecha->getACE())
    {
        if (!(counter % 2))
            html.append("<tr>");
        html.append("<td>+" + QString::number(static_cast<int>(mecha->getACEBonus() * 100)) + "%</td>");

        if (counter % 2)
            html.append("</tr>");

        ++counter;
    }

    if (mecha->getVernierRating())
    {
        if (!(counter % 2))
            html.append("<tr>");
        html.append("<td>Verniers +" + QString::number(mecha->getVernierRating()) + "</td>");

        if (counter % 2)
            html.append("</tr>");

        ++counter;
    }

    if (mecha->hasStealth())
    {
        if (!(counter % 2))
            html.append("<tr>");

        html.append("<td>Stealth</td>");

        if (counter % 2)
            html.append("</tr>");

        ++counter;
    }

    if (mecha->getCloakLevel())
    {
        if (!(counter % 2))
            html.append("<tr>");

        html.append("<td>" + QString(mecha->getCloakLevel() == 1 ? "Basic" : "Active") + " Cloak");

        if (mecha->hasBeamRefract() || mecha->hasCombatCloak() || mecha->hasFireControl()
                || mecha->hasMagneticRefract() || mecha->hasPulseRefract())
        {
            html.append(" with ");

            if (mecha->hasFireControl())
                html.append("Fire Control");

            if (mecha->hasCombatCloak())
            {
                if (mecha->hasFireControl())
                    html.append(", ");

                html.append("Combat Cloak");
            }

            if (mecha->hasBeamRefract())
            {
                if (mecha->hasFireControl() || mecha->hasCombatCloak())
                    html.append(", ");

                html.append("Beam Refract");
            }
            if (mecha->hasPulseRefract())
            {
                if (mecha->hasFireControl() || mecha->hasCombatCloak()
                        || mecha->hasBeamRefract())
                    html.append(", ");

                html.append("Pulse Refract");
            }
            if (mecha->hasMagneticRefract())
            {
                if (mecha->hasFireControl() || mecha->hasCombatCloak()
                        || mecha->hasBeamRefract() || mecha->hasPulseRefract())
                    html.append(", ");

                html.append("Magnetic Refract");
            }
        }

        html.append("</td>");

        if (counter % 2)
            html.append("</tr>");

        ++counter;
    }

    if (!(counter %2))
        html.append("</tr>");

    html.append("</table>");

    // Movement
    html.append("<table style=\"float:left\" width=\"50%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"3\" class=\"invertCol\">");
    html.append("MOVEMENT SYSTEMS</th>\n  </tr>\n  <tr>\n    <td><b>System</b>\n");
    html.append("</td>\n    <td><b>Location</b></td>\n    ");
    html.append("<td><b>Kills</b>\n</td>");
    html.append("  </tr>");

    QMap<QString,Propulsion*>& propList = mecha->getPropMapRef();

    foreach (QString prop,propList.keys())
    {
        if (!propList.value(prop)->getRating())
            continue;

        html.append("<tr>");

        html.append("<td>" + prop + "</td>");
        html.append("<td>");

        // Now we need to loop through Servos. Why isn't this just a splittable part?!
        QList<Servo*> serList = mecha->getServoRef();

        QList<Servo*>::iterator it = serList.begin();

        bool marked = false;

        while (it != serList.end())
        {
            if (!propList.value(prop)->getSpaces((*it)->getDesignation()))
            {
                ++it;
                continue;
            }

            if (marked)
                html.append("/");
            marked = true;
            html.append((*it)->getDesignation() + " " + QString::number(propList.value(prop)->getSpaces((*it)->getDesignation())));
            ++it;
        }

        html.append("</td>");
        html.append("<td>0</td>");

        html.append("</tr>");
    }

    if (mecha->getWheelSize())
    {
        html.append("<tr>");

        html.append("<td>Wheels</td>");
        html.append("<td>-</td>");

        html.append("<td>");

        QList<Servo*> serList = mecha->getServoRef();

        QList<Servo*>::iterator it = serList.begin();

        bool marked = false;

        while (it != serList.end())
        {
            if (!mecha->getTransKills("Wheels",(*it)->getDesignation()))
            {
                ++it;
                continue;
            }

            if (marked)
                html.append("/");
            marked = true;
            html.append((*it)->getDesignation() + " " +
                        QString::number(mecha->getTransKills("Wheels",(*it)->getDesignation())));
            ++it;
        }

        html.append("</td>");
        html.append("</tr>");
    }

    if (mecha->getTreadSize())
    {
        html.append("<tr>");

        html.append("<td>Treads</td>");
        html.append("<td>-</td>");

        html.append("<td>");

        QList<Servo*> serList = mecha->getServoRef();

        QList<Servo*>::iterator it = serList.begin();

        bool marked = false;

        while (it != serList.end())
        {
            if (!mecha->getTransKills("Treads",(*it)->getDesignation()))
            {
                ++it;
                continue;
            }

            if (marked)
                html.append("/");
            marked = true;
            html.append((*it)->getDesignation() + " " +
                        QString::number(mecha->getTransKills("Treads",(*it)->getDesignation())));
            ++it;
        }

        html.append("</td>");
        html.append("</tr>");
    }

    if (mecha->getFuelPercent())
    {
        html.append("<tr><td colspan=\"2\" class=\"invertCol\">Maximum Range</td><td>"
                    + QString::number(mecha->getMaxFlight()) + "</td></tr>");
    }

    QList<Fuel*> fuelList = mecha->getFuelRef();

    if (!fuelList.isEmpty())
    {
        html.append("<tr><th colspan=\"2\" class=\"invertCol\">Fuel Tank</th><th class=\"invertCol\">Loc</th></tr>");
        foreach (Fuel* fuel, fuelList)
        {
            html.append("<tr><td>"
                        + (fuel->getNumTanks() > 1 ? QString(QString::number(fuel->getNumTanks()) + "x ") : QString(""))
                        + QString(fuel->isExternal() ? "External" : "Internal"));
            html.append("</td><td>" + fuel->getLoc() + "</td></tr>");
        }
    }

    html.append("</table>");

    html.append("</div><br>");

    html.append("<div align=\"center\">");

    // Weapons
    html.append("<table style=\"float:left\" width=\"100%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"8\" class=\"invertCol\">");
    html.append("ARMAMENT</th>\n  </tr>\n  <tr>\n    <td><b>Weapon</b>\n");
    html.append("</td>\n    <td><b>WA</b></td>\n    ");
    html.append("<td><b>Range</b></td>\n    <td><b>Damage</b></td>\n    <td><b>Shots</b></td>\n");
    html.append("<td><b>Kills</b></td><td><b>Loc</b></td><td><b>Notes</b></td></tr>");

    QList<MatedSystem*>& mateList = mecha->getMatedPartsRef();

    foreach (MatedSystem* mated,mateList)
    {
        html.append("<tr><td colspan=\"6\">" + mated->getName() + "</td>");
        html.append("<td>" + htmlLocs(mated) + "</td>");
        html.append("<td>Mated System</td></tr>");

        html.append("<tr>");

        Weapon* wp = dynamic_cast<Weapon*>(mated->getPart(0));

        html.append("<td>-\\" + wp->getName() + "</td>");
        html.append("<td>" + QString::number(wp->getAcc()) + "</td>");
        html.append("<td>" + QString::number(wp->getRange()) + "</td>");
        html.append("<td>" + QString::number(wp->getDamage()) + "K</td>");
        html.append("<td>" + wp->getShotPrepend() + wp->getShots() + "</td>");
        html.append("<td>" + QString::number(wp->getKills()) + "</td>");
        html.append("<td>");
        html.append("-");
        html.append("</td>");
        html.append("<td>" + wp->getAllNotes());
        if (!wp->getLinks().isEmpty())
            html.append(", " + wp->getLinks());
        html.append("</td>");

        html.append("</tr>");

        html.append("<tr>");

        wp = dynamic_cast<Weapon*>(mated->getPart(1));

        html.append("<td>-\\" + wp->getName() + "</td>");
        html.append("<td>" + QString::number(wp->getAcc()) + "</td>");
        html.append("<td>" + QString::number(wp->getRange()) + "</td>");
        html.append("<td>" + QString::number(wp->getDamage()) + "K</td>");
        html.append("<td>" + wp->getShotPrepend() + wp->getShots() + "</td>");
        html.append("<td>" + QString::number(wp->getKills()) + "</td>");
        html.append("<td>-</td>");
        html.append("<td>" + wp->getAllNotes());
        if (!wp->getLinks().isEmpty())
            html.append(", " + wp->getLinks());
        html.append("</td>");

        html.append("</tr>");
    }

    QList<Weapon*>& wpList = mecha->getWeaponsRef();

    foreach (Weapon* wp, wpList)
    {
        if (wp->isMated())
            continue;

        html.append("<tr>");

        html.append("<td>" + wp->getName() + "</td>");
        html.append("<td>" + QString::number(wp->getAcc()) + "</td>");
        html.append("<td>" + QString::number(wp->getRange()) + "</td>");
        html.append("<td>" + QString::number(wp->getDamage()) + "K</td>");
        html.append("<td>" + wp->getShotPrepend() + wp->getShots() + "</td>");
        html.append("<td>" + QString::number(wp->getKills()) + "</td>");
        html.append("<td>" + htmlLocs(wp) + "</td>");
        html.append("<td>" + wp->getAllNotes());
        if (!wp->getLinks().isEmpty())
            html.append(", " + wp->getLinks());
        html.append("</td>");

        html.append("</tr>");
    }

    html.append("</table>\n");

    // Ammo
    if (!mecha->getAmmoRef().isEmpty())
    {
        html.append("<table style=\"float:left\" width=\"100%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"8\" class=\"invertCol\">");
        html.append("AMMUNITION</th>\n  </tr>\n  <tr>\n    <td><b>Weapon</b>\n");
        html.append("</td>\n    <td><b>WA</b></td>\n    ");
        html.append("<td><b>Number</b></td>\n    <td><b>Damage</b></td>\n    <td><b>Shots</b></td>\n");
        html.append("<td><b>Kills</b></td><td><b>Loc</b></td><td><b>Notes</b></td></tr>");

        {
            QList<Ammo*>& ammoList = mecha->getAmmoRef();

            foreach (Ammo* ammo, ammoList)
            {
                html.append("<tr>");

                html.append("<td>" + ammo->getWeaponName() + "</td>");
                html.append("<td>" + QString::number(ammo->getAcc()) + "</td>");
                html.append("<td>" + QString::number(ammo->getClips()) + "</td>");
                html.append("<td>" + QString::number(ammo->getDamage()) + "K</td>");
                html.append("<td>" + QString::number(ammo->getShots()) + " x"
                            + QString::number(ammo->getClips()) + "</td>");
                html.append("<td>" + QString::number(ammo->getKills()) +
                            " x" + QString::number(ammo->getClips()) + "</td>");
                html.append("<td>" + ammo->getLoc() + "</td>");
                html.append("<td>" + ammo->getAllNotes());
                html.append("</td>");

                html.append("</tr>");
            }
        }
        html.append("</table>\n");
    }

    if (!mecha->getPoolRef().isEmpty())
    {
        QList<EnergyPool*>& poolList = mecha->getPoolRef();

        html.append("<table style=\"float:left\" width=\"100%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"7\" class=\"invertCol\">");
        html.append("ENERGY POOLS");
        html.append("</th>\n  </tr>\n");
        html.append("<tr>\n    <td><b>Name</b>\n");
        html.append("</td>\n    <td><b>Loc</b></td>\n    ");
        html.append("<td><b>Avail. Power</b></td>\n    <td><b>Max Power</b></td>\n    <td><b>Portfolio</b></td>\n");
        html.append("<td><b>Kills</b></td><td><b>Morphable</b></td></tr>");

        foreach (EnergyPool* ep,poolList)
        {
            html.append("<tr>");

            html.append("<td>" + ep->getName() + "</td>");
            html.append("<td>" + htmlLocs(ep) + "</td>");
            html.append("<td>" + QString::number(ep->getPowerAvailable()) + "</td>");
            html.append("<td>" + QString::number(ep->getMaxPower()) + "</td>");
            html.append("<td>" + QString::number(ep->getPortfolio()) + "</td>");
            html.append("<td>" + QString::number(ep->getKills()) + "</td>");
            html.append("<td>" + (ep->isMorphable() ? QString("Yes") : QString("No")) + "</td>");

            html.append("</tr>");
        }

        html.append("</table>");

        html.append("<table style=\"float:left\" width=\"100%\" border=\"1\" cellspacing=\"1\">\n  <tr>\n    <th colspan=\"7\" class=\"invertCol\">");
        html.append("ENERGY POOL PORTFOLIOS");
        html.append("</th>\n  </tr>\n");
        html.append("<tr><td><b>Item</b>");
        html.append("</td>\n    <td><b>WA</b></td>\n    ");
        html.append("<td><b>Number</b></td>\n    <td><b>Damage</b></td>\n    <td><b>Shots</b></td>\n");
        html.append("<td><b>Kills</b></td><td><b>Notes</b></td></tr>");

        foreach (EnergyPool* ep,poolList)
        {
            html.append("<tr><td colspan=\"7\">" + ep->getName() + "</td></tr>");

            QList<Weapon*>& wpList = ep->getWeaponsRef();

            foreach (Weapon* wp,wpList)
            {
                html.append("<tr>");

                html.append("<td>* " + wp->getName() + "</td>");
                html.append("<td>" + QString::number(wp->getAcc()) + "</td>");
                html.append("<td>" + QString::number(wp->getRange()) + "</td>");
                html.append("<td>" + QString::number(wp->getDamage()) + "K</td>");
                html.append("<td>" + wp->getShotPrepend() + wp->getShots() + "</td>");
                html.append("<td>" + QString::number(wp->getKills()) + "</td>");
                html.append("<td>" + wp->getAllNotes());
                if (!wp->getLinks().isEmpty())
                    html.append(", " + wp->getLinks());
                html.append("</td>");

                html.append("</tr>");
            }

            QList<Shield*>& shList = ep->getShieldsRef();

            foreach (Shield* shield,shList)
            {
                html.append("<tr>");
                html.append("<td>* Shield: " + shield->getName() + "</td>");
                html.append("<td>SP: " + QString::number(shield->getSP()) + "</td>");
                html.append("<td>DC: " + QString::number(shield->getDC()) + "</td>");
                html.append("<td colspan=\"2\">Type: " + shield->getTypeString() + "</td>");
                html.append("<td colspan=\"2\">" + shield->getAllNotes() + "</td>");

                html.append("</tr>");
            }
        }

        html.append("</table>");
    }

    html.append("</div>");

    // close tags
    html.append("</body>\n</html>\n");

    return true;
}

QString HtmlConverter::htmlLocs(const SplittablePart* sp)
{
    QString out;
    if (sp->getLoc() == "Split")
    {
        QStringList locs = sp->getSplitLocs();
        QStringList::iterator it = locs.begin();
        while (it != locs.end())
        {
            out.append(QString::number(sp->getAllocSpace(*it)) + " " + *it);
            ++it;
            if (it != locs.end())
                out.append("/");
        }
    }
    else out = sp->getLoc();

    return out;
}
