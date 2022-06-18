#ifndef SHIELD_H
#define SHIELD_H

#include "mekton_defs.h"
#include "mektonfuncs.h"
#include "servocomp.h"
#include "splittablepart.h"

#include <QString>
#include <QStringList>

enum ShieldTypes {
    STANDARD,
    ACTIVE,
    REACTIVE
};

class Shield : public SplittablePart
{
public:
    Shield();

    ServoSizes getSize() const {return size;}
    QString getSizeString() const {return ServoComp::instance()->getSizeName(size);}
    ShieldTypes getType() const {return type;}
    QString getTypeString() const {return typeName(type);}
    int getBinderSpace() const {return qRound(((int)size+5)*binderMult)*2;}
    int getSP() const {return ((int)size + 5) - qRound(((int)size+5)*binderMult);}
    double getBaseCost() const {return ((int)size+5)*((2+(type*type))*0.5)*calcBinderMult() *optMult * ServoComp::instance()->getArmorMult(armor);}
    double getTotalCost() const {return getCost();}
    double getBaseSpace() const {return type ? (int)type*0.5 * getBaseCost():1;}
    ArmorTypes getArmor() const {return armor;}
    QString getArmorString() const {return ServoComp::instance()->getArmorTypeName(armor);}
    int getDC() const {return ServoComp::instance()->getArmorDC(armor);}
    double getBinderMul() const {return binderMult;}
    double getOptMul() const {return optMult;}
    QStringList getNotes() const {return notes;}
    QString getAllNotes() const {return notes.join(", ");}

    void setLoc(const QString& newVal) {((newVal == "Split") && !(type == REACTIVE)) ? loc = "None" : loc = newVal;}
    void setArmor(ArmorTypes newVal) {armor = newVal;}
    void setSize(ServoSizes newVal) {size = newVal; resetEff();}
    void setType(ShieldTypes newVal) {type = newVal; resetEff();}
    void setBinderMul(double newVal) {binderMult = newVal; resetEff();}
    void setOptMul(double newVal) {optMult = newVal;  resetEff();}
    void addNote(const QString& newVal) {notes.push_back(newVal);}

    static QString typeName(ShieldTypes t);
    static QStringList listTypes() {return QStringList(QString("Standard:Active:Reactive").split(':'));}

private:
    ServoSizes size;
    ShieldTypes type;
    ArmorTypes armor;
    double binderMult;
    double optMult;
    QStringList notes;

    double calcBinderMult() const;
};

#endif // SHIELD_H
