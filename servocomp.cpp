#include "servocomp.h"
#include <cmath>

ServoComp::ServoComp()
{
    sizeMap[SUPERLIGHT] = "Superlight";
    sizeMap[LIGHT_WEIGHT] = "Light Weight";
    sizeMap[STRIKER] = "Striker";
    sizeMap[MED_STRIKER] = "Medium Striker";
    sizeMap[HVY_STRIKER] = "Heavy Striker";
    sizeMap[MED_WT] = "Medium Weight";
    sizeMap[LT_HVY] = "Light Heavy";
    sizeMap[MED_HVY] = "Medium Heavy";
    sizeMap[AR_HVY] = "Armored Heavy";
    sizeMap[SUP_HVY] = "Super Heavy";
    sizeMap[MEGA_HVY] = "Mega Heavy";

    typeMap[TORSO] = "Torso";
    typeMap[HEAD] = "Head";
    typeMap[ARM] = "Arm";
    typeMap[LEG] = "Leg";
    typeMap[POD] = "Pod";
    typeMap[WING] = "Wing";
    typeMap[TAIL] = "Tail";

    baseMap[TORSO] = 1;
    baseMap[HEAD] = 1;
    baseMap[ARM] = 2;
    baseMap[LEG] = 2;
    baseMap[POD] = 1;
    baseMap[WING] = 1;
    baseMap[TAIL] = 1;

    costMap[TORSO] = 2;
    costMap[HEAD] = 1;
    costMap[ARM] = 1;
    costMap[LEG] = 1;
    costMap[POD] = 1;
    costMap[WING] = 1;
    costMap[TAIL] = 1;

    spaceMap[TORSO] = 2;
    spaceMap[HEAD] = 1;
    spaceMap[ARM] = 1;
    spaceMap[LEG] = 1;
    spaceMap[POD] = 2;
    spaceMap[WING] = 1;
    spaceMap[TAIL] = 1;

    killMap[TORSO] = 2;
    killMap[HEAD] = 1;
    killMap[ARM] = 1;
    killMap[LEG] = 1;
    killMap[POD] = 0;
    killMap[WING] = 1;
    killMap[TAIL] = 1;

    armorTypeMap[ABLATIVE] = "Ablative";
    armorTypeMap[NORMAL] = "Normal";
    armorTypeMap[ALPHA] = "Alpha";
    armorTypeMap[BETA] = "Beta";
    armorTypeMap[GAMMA] = "Gamma";

    armorMultMap[ABLATIVE] = 0.5;
    armorMultMap[NORMAL] = 1.0;
    armorMultMap[ALPHA] = 1.25;
    armorMultMap[BETA] = 1.5;
    armorMultMap[GAMMA] = 2.0;

    armorDCMap[ABLATIVE] = 0;
    armorDCMap[NORMAL] = 1;
    armorDCMap[ALPHA] = 2;
    armorDCMap[BETA] = 4;
    armorDCMap[GAMMA] = 8;

    ramMap[NONE] = "None";
    ramMap[FIFTH] = "1/5th";
    ramMap[FOURTH] = "1/4th";
    ramMap[THIRD] = "1/3rd";
    ramMap[HALF] = "1/2";

    ramCostMap[NONE] = 1.0;
    ramCostMap[FIFTH] = 1.5;
    ramCostMap[FOURTH] = 1.8;
    ramCostMap[THIRD] = 2.2;
    ramCostMap[HALF] = 2.5;

    ramReduceMap[NONE] = 1.0;
    ramReduceMap[FIFTH] = 1.0;
    ramReduceMap[FOURTH] = 0.8;
    ramReduceMap[THIRD] = 0.75;
    ramReduceMap[HALF] = 0.66;
}

ServoComp::~ServoComp()
{

}

ServoComp* ServoComp::instance()
{
    static ServoComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new ServoComp();
    return _instance;
}

QString ServoComp::getSizeName(ServoSizes size) const
{
    return sizeMap[size];
}

QString ServoComp::getTypeName(ServoTypes type) const
{
    return typeMap[type];
}

QString ServoComp::getRAMName(RAMTypes type) const
{
    return ramMap[type];
}

ServoSizes ServoComp::getSizeByString(const QString sizeName) const
{
    ServoSizes found = SUPERLIGHT;

    ServoSizes look;

    foreach (look,sizeMap.keys())
    {
        if (sizeMap[look] == sizeName)
        {
            found = look;
            break;
        }
    }

    return found;
}

QStringList ServoComp::getSizes() const
{
    QStringList sizeList;

    foreach (ServoSizes i,sizeMap.keys()) {
        sizeList.push_back(sizeMap[i]);
    }

    return sizeList;
}

QStringList ServoComp::getTypes() const
{
    QStringList typeList;

    foreach (ServoTypes i,typeMap.keys()) {
        typeList.push_back(typeMap[i]);
    }

    return typeList;
}

QStringList ServoComp::getRAMTypes() const
{
    QStringList typeList;

    foreach (RAMTypes i,ramMap.keys()) {
        typeList.push_back(ramMap[i]);
    }

    return typeList;
}

int ServoComp::getCost(ServoSizes size,ServoTypes type) const
{
    return (size + baseMap[type]) * costMap[type];
}

int ServoComp::getSpace(ServoSizes size,ServoTypes type) const
{
    return (size + baseMap[type]) * spaceMap[type];
}

int ServoComp::getKills(ServoSizes size,ServoTypes type) const
{
    return (size + baseMap[type]) * killMap[type];
}

qreal ServoComp::getWeight(ServoSizes size,ServoTypes type) const
{
    return (getKills(size,type) / 2.0);
}

qreal ServoComp::getArmorCost(ServoSizes size, ArmorTypes type, RAMTypes ram) const
{
    return getCost(size,HEAD) * armorMultMap[type] * ramCostMap[ram];
}

QString ServoComp::getArmorTypeName(ArmorTypes type) const
{
    return armorTypeMap[type];
}

QStringList ServoComp::getArmorTypes() const
{
    QStringList typeList;

    foreach (ArmorTypes i,armorTypeMap.keys()) {
        typeList.push_back(armorTypeMap[i]);
    }

    return typeList;
}

int ServoComp::getArmorDC(ArmorTypes type) const
{
    return armorDCMap[type];
}

int ServoComp::getArmorSP(ServoSizes size, RAMTypes ram) const
{
    return (int)(round(getKills(size,HEAD) * ramReduceMap[ram]));
}
