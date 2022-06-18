#ifndef MELEECOMP_H
#define MELEECOMP_H

#include <QMap>
#include <QString>

class MeleeComp
{
public:
    static MeleeComp* instance();

    float calcDamCost(int damage) {return damage / 2.0;}

    float getAccMul(int acc) {return accMulMap[acc];}
    float getShockMul(bool shock, bool add) {return (shock && add) ? 3.0 : (shock ? 2.0 : 1.0);}
    float getClumsyMul(bool clumsy) {return clumsy ? 0.5 : 1.0;}
    float getHandyMul(bool handy) {return handy ? 1.5 : 1.0;}
    float getEntangleMul(bool entangle) {return entangle ? 1.25 : 1.0;}
    float getAPMul(bool ap) {return ap ? 2.0 : 1.0;}
    float getDisruptorMul(bool dis) {return dis ? 2.0 : 1.0;}
    float getQuickMul(bool quick) {return quick ? 2.0 : 1.0;}
    float getThrownMul(bool thrown,bool returns) {return thrown ? (returns ? 1.5 : 1.2) : 1.0;}
private:
    QMap<int,float> accMulMap;

    MeleeComp();
    ~MeleeComp();
    MeleeComp(const MeleeComp &);             // intentionally undefined
    MeleeComp & operator=(const MeleeComp &); // intentionally undefined
};

#endif // MELEECOMP_H
