#ifndef SERVOCOMP_H
#define SERVOCOMP_H

#include "mekton_defs.h"
#include <QStringList>

class ServoComp
{
public:
    static ServoComp* instance();

    QString getSizeName(ServoSizes size) const;
    QString getTypeName(ServoTypes type) const;
    QString getArmorTypeName(ArmorTypes type) const;
    QString getRAMName(RAMTypes type) const;
    ServoSizes getSizeByString(const QString sizeName) const;

    QStringList getSizes() const;
    QStringList getTypes() const;
    QStringList getArmorTypes() const;
    QStringList getRAMTypes() const;

    int getCost(ServoSizes size,ServoTypes type) const;
    int getSpace(ServoSizes size,ServoTypes type) const;
    int getKills(ServoSizes size,ServoTypes type) const;
    qreal getWeight(ServoSizes size,ServoTypes type) const;
    qreal getArmorCost(ServoSizes size, ArmorTypes type, RAMTypes ram) const;
    int getArmorDC(ArmorTypes type) const;
    int getArmorSP(ServoSizes size,RAMTypes ram) const;
    qreal getArmorMult(ArmorTypes type) const {return armorMultMap[type];}


private:
    QMap<ServoSizes,QString> sizeMap;
    QMap<ServoTypes,QString> typeMap;
    QMap<ArmorTypes,QString> armorTypeMap;
    QMap<RAMTypes,QString> ramMap;

    QMap<ServoTypes,int> baseMap;
    QMap<ServoTypes,int> costMap;
    QMap<ServoTypes,int> spaceMap;
    QMap<ServoTypes,int> killMap;
    QMap<ArmorTypes,qreal> armorMultMap;
    QMap<ArmorTypes,int> armorDCMap;
    QMap<RAMTypes,qreal> ramCostMap;
    QMap<RAMTypes,qreal> ramReduceMap;

    ServoComp();
    ~ServoComp();
    ServoComp(const ServoComp &);             // intentionally undefined
    ServoComp & operator=(const ServoComp &); // intentionally undefined
};

#endif // SERVOCOMP_H
