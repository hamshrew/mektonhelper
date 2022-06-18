#ifndef SERVO_H
#define SERVO_H

#include <QString>
#include <cmath>
#include "mekton_defs.h"
#include "servocomp.h"

class Servo
{
public:
    Servo();
    ~Servo();

    // Accessors
    const QString& getDesignation() const;
    ServoTypes getType() const;
    ServoSizes getSize() const;
    QString getTypeString() const;
    QString getSizeString() const;
    ServoSizes getArmorSize() const;
    ArmorTypes getArmorType() const;
    QString getArmorString() const;
    QString getArmorTypeString() const;
    int getArmorProt() const;
    qreal getArmorWeight() const;
    RAMTypes getRAM() const;
    QString getRAMString() const;
    int getReinforce() const;
    int getCost() const;
    qreal getArmorCost() const;
    qreal getTotalCost() const;
    int getSpace() const;
    int getKills() const;
    int getMounts() const {return mounts;}
    int getDamageBonus() const {return trunc(getSize() / 3.0);}

    qreal getWeight() const;
    qreal getTotalWeight() const;

    // Set methods
    void setDesignation(const QString& newVal);
    void setType(ServoTypes newVal);
    void setSize(ServoSizes newVal);
    void setArmorSize(ServoSizes newVal);
    void setArmorType(ArmorTypes newVal);
    void setRAM(RAMTypes newVal);
    void setReinforce(int newVal);
    void setMounts(int newVal);

private:
    QString designation;
    ServoTypes type;
    ServoSizes size;
    ServoSizes armorSize;
    ArmorTypes armorType;
    RAMTypes ram;
    int mounts;
    int reinforce;
};

inline const QString& Servo::getDesignation() const
{
    return designation;
}

inline QString Servo::getTypeString() const
{
    return ServoComp::instance()->getTypeName(type);
}

inline QString Servo::getSizeString() const
{
    return ServoComp::instance()->getSizeName(size);
}

inline ServoTypes Servo::getType() const
{
    return type;
}

inline ServoSizes Servo::getSize() const
{
    return size;
}

inline ServoSizes Servo::getArmorSize() const
{
    return armorSize;
}

inline QString Servo::getArmorString() const
{
    return ServoComp::instance()->getSizeName(armorSize);
}

inline QString Servo::getArmorTypeString() const
{
    return ServoComp::instance()->getArmorTypeName(armorType);
}

inline int Servo::getArmorProt() const
{
    return ServoComp::instance()->getArmorSP(armorSize,ram);
}

inline int Servo::getCost() const
{
    if (reinforce <=0)
        return ServoComp::instance()->getCost(size,type) + (mounts*2);
    return ServoComp::instance()->getCost(size,type) + (reinforce*2) + (mounts*2);
}

inline qreal Servo::getArmorCost() const
{
    return ServoComp::instance()->getArmorCost(armorSize,armorType,ram);
}

inline int Servo::getSpace() const
{
    return ServoComp::instance()->getSpace(size,type) - (reinforce * 2);
}

inline int Servo::getKills() const
{
    return ServoComp::instance()->getKills(size,type) + reinforce;
}

inline qreal Servo::getWeight() const
{
    return ServoComp::instance()->getWeight(size,type) + (reinforce * 0.5);
}

inline qreal Servo::getArmorWeight() const
{
    return ServoComp::instance()->getWeight(armorSize,HEAD);
}

inline ArmorTypes Servo::getArmorType() const
{
    return armorType;
}

inline RAMTypes Servo::getRAM() const
{
    return ram;
}

inline QString Servo::getRAMString() const
{
    return ServoComp::instance()->getRAMName(ram);
}

inline int Servo::getReinforce() const
{
    return reinforce;
}

inline qreal Servo::getTotalWeight() const
{
    return getWeight() + getArmorWeight();
}

inline qreal Servo::getTotalCost() const
{
    return getCost() + getArmorCost();
}

inline void Servo::setDesignation(const QString &newVal)
{
    designation = newVal;
}

inline void Servo::setType(ServoTypes newVal)
{
    type = newVal;
    if (mounts > 1 && type != TORSO)
        mounts = 1;
    setReinforce(getReinforce());
}

inline void Servo::setSize(ServoSizes newVal)
{
    size = newVal;
    setReinforce(getReinforce());
}

inline void Servo::setArmorSize(ServoSizes newVal)
{
    armorSize = newVal;
}

inline void Servo::setArmorType(ArmorTypes newVal)
{
    armorType = newVal;
}

inline void Servo::setRAM(RAMTypes newVal)
{
    ram = newVal;
}

inline void Servo::setMounts(int newVal)
{
    if (getKills())
        mounts = (newVal > 1) ? ((type == TORSO) ? 2 : 1) : newVal;
}

#endif // SERVO_H
