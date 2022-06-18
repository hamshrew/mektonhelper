#ifndef MECHAFILE_H
#define MECHAFILE_H

#include "mecha.h"

#include <QList>
#include <QDomNode>
#include <QString>

class QFile;
class QDomDocument;
class SplittablePart;
class Weapon;
class Shield;

class MechaFile
{
public:
    MechaFile();

    bool parseFile(QFile* file);
    bool writeFile(const QString& filename);
    bool setMecha(Mecha* mecha);
    Mecha* getMecha() const {return mecha;}

private:
    Mecha* mecha;

    void parseDomTree(const QDomDocument& doc);
    Weapon* parseWeapon(const QDomElement& el);
    Shield* parseShield(const QDomElement& el);
    void makeSplitNodes(QDomDocument& doc,QDomNode& node,SplittablePart* part);
    void makeLinkNodes(QDomDocument& doc,QDomNode& node,Weapon* wp);
    void addSplits(const QDomNode& node,SplittablePart* part);
    void addLinks(const QDomNode& node,Weapon* wp);
    QDomNode makeWeaponNode(QDomDocument& doc,Weapon* wp);
    QDomNode makeShieldNode(QDomDocument& doc,Shield* sh);

    void addWheels(const QDomNode& node);
    void addTreads(const QDomNode& node);
    void addServos(const QDomNode& node);
    void addWeapons(const QDomNode& node);
    void addMated(const QDomNode& node);
    void addControl(const QDomNode& node);
    void addSensors(const QDomNode& node);
    void addAmmo(const QDomNode& node);
    void addPools(const QDomNode& node);
    void addFuel(const QDomNode& node);
    void addPropulsion(const QDomNode& node);
    void addOptions(const QDomNode& node);
    void addShields(const QDomNode& node);
    void addReflectors(const QDomNode& node);
    void addECM(const QDomNode& node);
    void addCockpitControls(const QDomNode& node);
    void addEnvironmentals(const QDomNode& node);
    void addPlant(const QDomNode& node);
    void addRemotes(const QDomNode& node);
    void addCloaking(const QDomNode& node);
    void addForms(const QDomNode& node);
};

#endif // MECHAFILE_H
