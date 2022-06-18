#ifndef EMWCOMP_H
#define EMWCOMP_H

#include <QMap>
#include <QString>

class EMWComp
{
public:
    static EMWComp* instance();

    float calcDamCost(int damage) {return damage;}

    float getAccMul(int acc) {return accMulMap[acc];}
    float getTIUMul(const QString& turns) {return tiuMulMap[turns];}
    float getAFMul(int af) {return 1.0 + (af * 0.5);}
    float getShieldMul(bool shield,bool var) {return (shield && var) ? 2.0 : (shield ? 1.5 : 1.0);}
    float getRechargeMul(bool recharge) {return recharge ? 1.1 : 1.0;}
    float getHyperMul(bool hyper) {return hyper ? 7.5 : 1.0;}
    float getQuickMul(bool quick) {return quick ? 2.0 : 1.0;}
    float getThrownMul(bool thrown) {return thrown ? 1.2 : 1.0;}
private:
    QMap<int,float> accMulMap;
    QMap<QString,float> tiuMulMap;

    EMWComp();
    ~EMWComp();
    EMWComp(const EMWComp &);             // intentionally undefined
    EMWComp & operator=(const EMWComp &); // intentionally undefined
};

#endif // EMWCOMP_H
