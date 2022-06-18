#include "emwcomp.h"

EMWComp::EMWComp()
{
    accMulMap[-2] = 0.6;
    accMulMap[-1] = 0.8;
    accMulMap[0] = 0.9;
    accMulMap[1] = 1.0;
    accMulMap[2] = 1.5;
    accMulMap[3] = 2.0;

    tiuMulMap["1"] = 0.3;
    tiuMulMap["2"] = 0.4;
    tiuMulMap["3"] = 0.5;
    tiuMulMap["4"] = 0.6;
    tiuMulMap["5"] = 0.7;
    tiuMulMap["7"] = 0.8;
    tiuMulMap["10"] = 0.9;
    tiuMulMap[QString(QChar(0x221e))] = 1.0;
}

EMWComp::~EMWComp()
{

}

EMWComp* EMWComp::instance()
{
    static EMWComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new EMWComp();
    return _instance;
}
