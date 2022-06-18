#ifndef BEAMCOMP_H
#define BEAMCOMP_H

#include <QMap>
#include <QString>

class BeamComp
{
public:
    static BeamComp* instance();

    float calcDamCost(int damage);
    int calcRange(int damage,int rangeMod);

    float getRangeModMul(int rangeMod);
    float getAccMul(int acc);
    float getWarmUpMul(int warmUp);
    float getShotsMul(const QString& shots);
    float getWideAngleMul(const QString& wideAngle);
    float getBVMul(const QString& BV);
    float getClipMul(bool clipFed);
    float getVarMul(bool aMiss,bool aPer,bool aMech);
    float getLRMul(bool LR) {return LR ? 1.33 : 1.0;}
    float getHydroMul(bool hydro) {return hydro ? 0.2 : 1.0;}
    float getMegaBeamMul(bool mega) {return mega ? 10.0 : 1.0;}
    float getDisMul(bool dis) {return dis ? 2.0 : 1.0;}

private:
    QMap<int,int> damRangeMap;
    QMap<int,float> rangeModMap;
    QMap<int,float> rangeModMulMap;
    QMap<int,float> accMulMap;
    QMap<int,float> warmUpMulMap;
    QMap<QString,float> shotsMulMap;
    QMap<QString,float> wideAngleMulMap;
    QMap<QString,float> BVMulMap;

    BeamComp();
    ~BeamComp();
    BeamComp(const BeamComp &);             // intentionally undefined
    BeamComp & operator=(const BeamComp &); // intentionally undefined
};

#endif // BEAMCOMP_H
