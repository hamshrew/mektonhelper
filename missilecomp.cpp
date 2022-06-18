#include "missilecomp.h"

bool numericalStringLessThan(const QString &s1, const QString &s2)
{
    return s1.section('%',0,0).toUInt() < s2.section('%',0,0).toUInt();
}

MissileComp::MissileComp()
{
    accMulMap[-2] = 0.6;
    accMulMap[-1] = 0.8;
    accMulMap[0] = 1.0;
    accMulMap[1] = 1.3;
    accMulMap[2] = 1.6;
    accMulMap[3] = 2.0;

    rangeMulMap["0%"] = 0.5;
    rangeMulMap["25%"] = 0.62;
    rangeMulMap["50%"] = 0.75;
    rangeMulMap["75%"] = 0.88;
    rangeMulMap["100%"] = 1.0;
    rangeMulMap["125%"] = 1.12;
    rangeMulMap["150%"] = 1.25;
    rangeMulMap["175%"] = 1.38;
    rangeMulMap["200%"] = 1.5;
    rangeMulMap["500%"] = 3.0;
    rangeMulMap["1000%"] = 5.5;
    rangeMulMap["3000%"] = 15.5;
    rangeMulMap["5000%"] = 25.5;

    damRangeMap[1] = 4;
    damRangeMap[2] = 5;
    damRangeMap[3] = 6;
    damRangeMap[4] = 7;
    damRangeMap[5] = 8;
    damRangeMap[6] = 9;
    damRangeMap[7] = 9;
    damRangeMap[8] = 10;
    damRangeMap[9] = 11;
    damRangeMap[10] = 11;
    damRangeMap[11] = 12;
    damRangeMap[12] = 12;
    damRangeMap[13] = 13;
    damRangeMap[14] = 13;
    damRangeMap[15] = 14;
    damRangeMap[16] = 14;
    damRangeMap[17] = 14;
    damRangeMap[18] = 15;
    damRangeMap[19] = 15;
    damRangeMap[20] = 16;

    skillMulMap["06+"] = 1.0;
    skillMulMap["09+"] = 1.3;
    skillMulMap["12+"] = 1.6;
    skillMulMap["15+"] = 1.9;
    skillMulMap["18+"] = 2.2;
    skillMulMap["20+"] = 2.5;

    BRMulMap[0] = 1.0;
    BRMulMap[1] = 3.0;
    BRMulMap[2] = 4.0;
    BRMulMap[3] = 5.0;
    BRMulMap[4] = 6.0;
    BRMulMap[5] = 7.0;
    BRMulMap[6] = 7.5;
    BRMulMap[7] = 8.0;
    BRMulMap[8] = 8.5;
    BRMulMap[9] = 9.0;
    BRMulMap[10] = 10.0;
    BRMulMap[20] = 20.0;
}

MissileComp::~MissileComp()
{

}

MissileComp* MissileComp::instance()
{
    static MissileComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new MissileComp();
    return _instance;
}

QStringList MissileComp::getRangeList() const
{
    QStringList sl;

    foreach (QString s,rangeMulMap.keys())
    {
        sl.push_back(s);
    }

    qSort(sl.begin(),sl.end(),numericalStringLessThan);
    return sl;
}

QStringList MissileComp::getBRList() const
{
    QStringList sl;

    int i;

    foreach (i,BRMulMap.keys())
    {
        sl.push_back(QString::number(i));
    }
    return sl;
}
