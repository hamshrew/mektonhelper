#ifndef MISSILECOMP_H
#define MISSILECOMP_H

#include <QMap>
#include <QString>
#include <QStringList>

class MissileComp
{
public:
    static MissileComp* instance();

    float calcDamCost(int damage) {return damage / 10.0;}
    float calcRange(int damage) {return damRangeMap[damage];}

    float getAccMul(int acc) {return accMulMap[acc];}
    float getRangeMul(const QString& range) {return rangeMulMap[range];}
    float getSmartMul(int smart) {return smart ? 2.0 + (smart*0.5): 1.0;}
    float getSkillMul(const QString& skill) {return skillMulMap[skill];}
    float getBRMul(int br) {return BRMulMap[br];}

    QStringList getRangeList() const;
    QStringList getSkillList() const {return skillMulMap.keys();}
    QStringList getBRList() const;

    float getCounterMul(bool counter, bool var) {return (counter && var) ? 1.8 : 1.0;}
    float getLRMul(bool lr) {return lr ? 1.33 : 1.0;}
    float getHVMul(bool hv) {return hv ? 1.25 : 1.0;}
    float getFoamMul(bool foam) {return foam ? 1.33 : 1.0;}
    float getFlareMul(bool flare) {return flare ? 0.5 : 1.0;}
    float getFuseMul(bool fuse) {return fuse ? 1.1 : 1.0;}
    float getNuclearMul(bool nuke) {return nuke ? 1000.0 : 1.0;}
    float getSmokeScatterMul(bool smoke,bool scatter) {return ((smoke + scatter) == 1) ? 0.5 : 1.0;}
private:
    QMap<int,float> accMulMap;
    QMap<int,float> damRangeMap;
    QMap<QString,float> rangeMulMap;
    QMap<QString,float> skillMulMap;
    QMap<int,float> BRMulMap;

    MissileComp();
    ~MissileComp();
    MissileComp(const MissileComp &);             // intentionally undefined
    MissileComp & operator=(const MissileComp &); // intentionally undefined
};

#endif // MISSILECOMP_H
