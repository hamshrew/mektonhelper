#include "meleecomp.h"

MeleeComp::MeleeComp()
{
    accMulMap[-2] = 0.6;
    accMulMap[-1] = 0.8;
    accMulMap[0] = 1.0;
    accMulMap[1] = 1.5;
    accMulMap[2] = 2.0;
}

MeleeComp::~MeleeComp()
{

}

MeleeComp* MeleeComp::instance()
{
    static MeleeComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new MeleeComp();
    return _instance;
}
