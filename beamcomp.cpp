#include "beamcomp.h"

#include <cmath>

BeamComp::BeamComp()
{
    damRangeMap[1] = 4;
    damRangeMap[2] = 6;
    damRangeMap[3] = 7;
    damRangeMap[4] = 8;
    damRangeMap[5] = 9;
    damRangeMap[6] = 10;
    damRangeMap[7] = 11;
    damRangeMap[8] = 11;
    damRangeMap[9] = 12;
    damRangeMap[10] = 13;
    damRangeMap[11] = 13;
    damRangeMap[12] = 14;
    damRangeMap[13] = 14;
    damRangeMap[14] = 15;
    damRangeMap[15] = 15;
    damRangeMap[16] = 16;
    damRangeMap[17] = 16;
    damRangeMap[18] = 17;
    damRangeMap[19] = 17;
    damRangeMap[20] = 18;

    rangeModMap[0] = 0.25;
    rangeModMap[1] = 0.5;
    rangeModMap[2] = 0.75;
    rangeModMap[3] = 1.0;
    rangeModMap[4] = 1.25;
    rangeModMap[5] = 1.5;
    rangeModMap[6] = 1.75;
    rangeModMap[7] = 2.0;
    rangeModMap[8] = 2.5;
    rangeModMap[9] = 3.0;

    rangeModMulMap[0] = 0.62;
    rangeModMulMap[1] = 0.75;
    rangeModMulMap[2] = 0.88;
    rangeModMulMap[3] = 1.0;
    rangeModMulMap[4] = 1.12;
    rangeModMulMap[5] = 1.25;
    rangeModMulMap[6] = 1.38;
    rangeModMulMap[7] = 1.5;
    rangeModMulMap[8] = 1.75;
    rangeModMulMap[9] = 2.0;

    accMulMap[-2] = 0.6;
    accMulMap[-1] = 0.8;
    accMulMap[0] = 0.9;
    accMulMap[1] = 1.0;
    accMulMap[2] = 1.5;
    accMulMap[3] = 2.0;

    warmUpMulMap[0] = 1.0;
    warmUpMulMap[1] = 0.9;
    warmUpMulMap[2] = 0.7;
    warmUpMulMap[3] = 0.6;

    shotsMulMap["0"] = 0.33;
    shotsMulMap["1"] = 0.5;
    shotsMulMap["2"] = 0.6;
    shotsMulMap["3"] = 0.7;
    shotsMulMap["5"] = 0.8;
    shotsMulMap["10"] = 0.9;
    shotsMulMap[QString(QChar(0x221e))] = 1.0;

    wideAngleMulMap["None"] = 1.0;
    wideAngleMulMap["Hex"] = 2.0;
    wideAngleMulMap["60"] = 3.0;
    wideAngleMulMap["180"] = 5.0;
    wideAngleMulMap["300"] = 7.0;
    wideAngleMulMap["360"] = 9.0;

    BVMulMap["None"] = 1.0;
    BVMulMap["2"] = 1.5;
    BVMulMap["3"] = 2.0;
    BVMulMap["4"] = 2.5;
    BVMulMap["5"] = 3.0;
    BVMulMap["6"] = 3.5;
    BVMulMap["7"] = 4.0;
    BVMulMap["8"] = 4.5;
    BVMulMap[QString(QChar(0x221e))] = 5.0;
}

BeamComp::~BeamComp()
{

}

BeamComp* BeamComp::instance()
{
    static BeamComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new BeamComp();
    return _instance;
}

float BeamComp::calcDamCost(int damage)
{
    return damage*1.5;
}

int BeamComp::calcRange(int damage, int rangeMod)
{
    return (int)round(damRangeMap[damage] * rangeModMap[rangeMod]);
}

float BeamComp::getRangeModMul(int rangeMod)
{
    return rangeModMulMap[rangeMod];
}

float BeamComp::getAccMul(int acc)
{
    return accMulMap[acc];
}

float BeamComp::getWarmUpMul(int warmUp)
{
    return warmUpMulMap[warmUp];
}

float BeamComp::getShotsMul(const QString& shots)
{
    return shotsMulMap[shots];
}

float BeamComp::getWideAngleMul(const QString& wideAngle)
{
    return wideAngleMulMap[wideAngle];
}

float BeamComp::getBVMul(const QString& BV)
{
    return BVMulMap[BV];
}

float BeamComp::getClipMul(bool clipFed)
{
    return clipFed? 0.9 : 1.0;
}

float BeamComp::getVarMul(bool aMiss,bool aPer,bool aMech)
{
    int sum = aMiss + aPer + aMech;
    --sum;
    return 1.0 + (sum * 0.8);
}
