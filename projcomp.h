#ifndef PROJCOMP_H
#define PROJCOMP_H

#include <QMap>
#include <QString>
#include <QStringList>

class ProjComp
{
public:
    static ProjComp* instance();

    float calcDamCost(int damage) {return damage;}
    float calcRange(int damage) {return damRangeMap[damage];}

    float getAccMul(int acc) {return accMulMap[acc];}
    float getRangeMul(const QString& range) {return rangeMulMap[range];}
    float getMultiMul(int multi) {return 1.0 + ((multi -1) * 0.2);}
    float getBVMul(int bv) {return (bv+1)*0.5;}

    QStringList getRangeList() const;

    float getVariableMul(bool phalanx, bool antiPerson, bool antiMech) const;
    float getLRMul(bool lr) const {return lr ? 1.33 : 1.0;}
    float getHVMul(bool hv) const {return hv ? 1.25 : 1.0;}

private:
    QMap<int,float> accMulMap;
    QMap<int,float> damRangeMap;
    QMap<QString,float> rangeMulMap;

    static bool numericalStringLessThan(const QString &s1, const QString &s2);

    ProjComp();
    ~ProjComp();
    ProjComp(const ProjComp &);             // intentionally undefined
    ProjComp & operator=(const ProjComp &); // intentionally undefined
};
#endif // PROJCOMP_H
