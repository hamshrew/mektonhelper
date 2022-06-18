#include "cockpitcomp.h"

CockpitComp::CockpitComp()
{
    encTypes.push_back("Armored");
    encTypes.push_back("Canopy");
    encTypes.push_back("Saddle");
}

CockpitComp::~CockpitComp()
{

}

CockpitComp* CockpitComp::instance()
{
    static CockpitComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new CockpitComp();
    return _instance;
}
