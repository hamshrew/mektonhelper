#include "projcomp.h"

bool ProjComp::numericalStringLessThan(const QString &s1, const QString &s2)
{
    return s1.section('%',0,0).toUInt() < s2.section('%',0,0).toUInt();
}

ProjComp::ProjComp()
{
    accMulMap[-2] = 0.6;
    accMulMap[-1] = 0.8;
    accMulMap[0] = 1.0;
    accMulMap[1] = 1.5;
    accMulMap[2] = 2.0;

    rangeMulMap["0%"] = 0.5;
    rangeMulMap["25%"] = 0.62;
    rangeMulMap["50%"] = 0.75;
    rangeMulMap["75%"] = 0.88;
    rangeMulMap["100%"] = 1.0;
    rangeMulMap["125%"] = 1.12;
    rangeMulMap["150%"] = 1.25;
    rangeMulMap["175%"] = 1.38;
    rangeMulMap["200%"] = 1.5;
    rangeMulMap["250%"] = 1.75;
    rangeMulMap["300%"] = 2.0;

    damRangeMap[1] = 3;
    damRangeMap[2] = 4;
    damRangeMap[3] = 5;
    damRangeMap[4] = 6;
    damRangeMap[5] = 7;
    damRangeMap[6] = 7;
    damRangeMap[7] = 8;
    damRangeMap[8] = 8;
    damRangeMap[9] = 9;
    damRangeMap[10] = 9;
    damRangeMap[11] = 10;
    damRangeMap[12] = 10;
    damRangeMap[13] = 11;
    damRangeMap[14] = 11;
    damRangeMap[15] = 12;
    damRangeMap[16] = 12;
    damRangeMap[17] = 12;
    damRangeMap[18] = 13;
    damRangeMap[19] = 13;
    damRangeMap[20] = 13;
}

ProjComp::~ProjComp()
{

}

ProjComp* ProjComp::instance()
{
    static ProjComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new ProjComp();
    return _instance;
}

QStringList ProjComp::getRangeList() const
{
    QStringList sl;

    foreach (QString s,rangeMulMap.keys())
    {
        sl.push_back(s);
    }
    qSort(sl.begin(),sl.end(),numericalStringLessThan);
    return sl;
}

float ProjComp::getVariableMul(bool phalanx, bool antiPerson, bool antiMech) const
{
    if (phalanx)
    {
        if (antiMech)
        {
            if (antiPerson)
                return 2.6;
            else return 1.4;
        }
        else if (antiPerson)
            return 1.6;

        return 0.5;
    }
    else if (antiPerson && antiMech)
        return 1.8;

    return 1.0;
}
